#include "Password.h"

#include <iostream>
using namespace std;

//Private methods
int Password::getNumMatches(String* curr_word, String* word_guess)
{
	char* current = curr_word->getText();
	char* guess = word_guess->getText();
	int count = 0;
	for (int x = 0; x < curr_word->length(); x++)
	{
		if (current[x] == guess[x])
			count++;
	}
	return count;
	
}

//Public methods
Password::Password()
{
	viable_words = new ListArray<String>;
	all_words = new ListArray<String>;
	len = 0;
}

Password::~Password()
{
	ListArrayIterator<String>* iter = all_words->iterator();
	
	while (iter->hasNext())
	{
		String* word = iter->next();
		delete word;
	}
	
	delete iter;
	delete viable_words;
	delete all_words;
}

void Password::addWord(String* word)
{
	viable_words->add(word);
	all_words->add(word);
	
	if (len == 0)
	{
		len = word->length();
	}
}

void Password::guess(int try_password, int num_matches)
{
	int x = 1;
	if (0 < try_password && try_password <= all_words->size())
	{
		while (x <= viable_words->size())
		{
			if (num_matches != getNumMatches(all_words->get(try_password), viable_words->get(x)))
			{
				viable_words->remove(x);
			}
			else
				x++;
		}
	}
	
	
}

int Password::getNumberOfPasswordsLeft()
{
	return viable_words->size();
}
void Password::displayViableWords()
{
	ListArrayIterator<String>* iter = viable_words->iterator();
	String* password;
	while(iter->hasNext())
	{
		password = iter->next();
		password->displayString();
		cout << endl;
	}
	delete iter;
}
int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}

String* Password::getOriginalWord(int index)
{
	if (0 < index && index < all_words->size())
	return all_words->get(index);
}