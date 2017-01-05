#define _CRT_SECURE_NO_WARNINGS
#define GAME_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#include "types.h"
#include "Boggle.h"
#include "Dictionary.h"
#include "dice.h"
#include "game.h"

char8_t alreadySearched[NUM_ROWS][NUM_COLS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void resetGame()
{

}

void _resetGame()
{
	ClearList();
}

void initGame()
{

}

void buildRandomBoard()
{

}

struct GameBoard* _buildRandomBoard()
{
	int valuesAllocated = 0;
	GameBoard* gameBoard = (GameBoard*)malloc(sizeof(GameBoard));
	// Loop for all positions on the game board generated above
	for (uchar8_t i = 0; i < NUM_ROWS; i++)
	{
		for (uchar8_t j = 0; j < NUM_COLS; j++)
		{
			// Randomly picking up a die which hasn't been picked yet
			uchar8_t index = rand() % (NUM_DICE - valuesAllocated);
			// Randomly picking up the face of the die which will face up
			uchar8_t offset = rand() % 6;
			gameBoard->boardValues[i][j] = *(dieSet[index] + offset);
			// Swap the die picked with the last die in the list of die's not picked yet
			char8_t* tempForSwap = dieSet[index];
			dieSet[index] = dieSet[NUM_DICE - 1 - valuesAllocated];
			dieSet[NUM_DICE - 1 - valuesAllocated] = tempForSwap;

			valuesAllocated++;
		}
	}
	return gameBoard;
}

void printBoard()
{
#if DEBUG_PRINTING_ON

#endif
}

void _printBoard(GameBoard* gameBoard)
{
#if DEBUG_PRINTING_ON
	printf("\n************\nGAME BOARD\n************\n");
	// Loop through all the positions on the game board
	for (uchar8_t i = 0; i < NUM_ROWS; i++)
	{
		for (uchar8_t j = 0; j < NUM_COLS; j++)
		{
			// Display the alphabets till row is finished
			printf(" %c ", gameBoard->boardValues[i][j]);
			// Move to next row for displaying
			if (j == (NUM_COLS - 1))
			{
				printf("\n");
			}
		}
		printf("\n\n");
	}
#endif
}

void printWords()
{
#if DEBUG_PRINTING_ON

#endif
}

void _printWords()
{
	// If the list is empty then return
	wordListT* traversingNode = topOfWordList;
	if (topOfWordList == NULL)	return;
	// Traverse through the list while printing the words
	while (traversingNode->nextWord != NULL)
	{
		printf("%s\n", traversingNode->word);
		traversingNode = traversingNode->nextWord;
	}
	// Printing the last word
	printf("%s\n", traversingNode->word);
}

void searchForWords()
{
	//just taking some time here to make sure the timer is working properly
	float j = 0.0f;
	for (int i = 0; i < 100000; ++i)
		j += ((float)rand()) / 100000.0f;

	printf("j=%f", j);
}

void _searchForWords(GameBoard* gameBoard)
{
	//just taking some time here to make sure the timer is working properly
	float j = 0.0f;
	for (int32_t i = 0; i < 100000; ++i)
	{
		j += ((float)rand()) / 100000.0f;
	}
	printf("j=%f\n\n", j);
	// Loop to search for all words starting from each position on the board
	for (int32_t i = 0; i < NUM_ROWS; i++)
	{
		for (int32_t j = 0; j < NUM_COLS; j++)
		{
			// Mark the current position on board as visited before initiating the search
			alreadySearched[i][j] = 1;
			char8_t* stringBuffer = GetStringBuffer();
			strcpy(stringBuffer, "");

			// Debugging
			//printf("\nStarting alphabet for searching: Row = %d, Col = %d\n", i, j);
			
			// Call to recursive function to find all string starting from position (i,j) on board
			_traverseStringPaths(i, j, gameBoard, rootOfTree, stringBuffer);
			free(stringBuffer);
			_clearAlreadySearched();
		}
	}
}

void _clearAlreadySearched()
{
	for (int32_t i = 0; i < NUM_ROWS; i++)
	{
		for (int32_t j = 0; j < NUM_COLS; j++)
		{
			// Reset all positions to unvisited
			alreadySearched[i][j] = 0;
		}
	}
}

void _traverseStringPaths(int32_t rowNumber, int32_t colNumber, GameBoard* gameBoard, trieNode* currentTrieNode, char8_t* currentBuffer)
{
	// Traverse along the alphabet in the currently reached position
	trieNode* traverseTo = TraverseAlongTrie(currentTrieNode, gameBoard->boardValues[rowNumber][colNumber]);
	// If the tree terminates, return
	if (traverseTo == NULL) 
	{
		return;
	}
	// If the current alphabet is Q, further traverse along U.
	if (gameBoard->boardValues[rowNumber][colNumber] == 'Q')
	{
		// Case: If Q and U have to been added and space isn't left
		if (strlen(currentBuffer) > (MAX_CHARS_IN_DICTIONARY_WORD - 2))
		{
			return;
		}
		else
		{
			traverseTo = TraverseAlongTrie(traverseTo, 'U');
			if (traverseTo == NULL)
			{
				return;
			}
		}
	}

	// Debugging
	//printf("\nReached: %c on board, Row: %d, Col: %d\n", gameBoard->boardValues[rowNumber][colNumber], rowNumber+1, colNumber+1);
	
	// Add the character to string buffer
	char8_t* stringBuffer = GetStringBuffer();
	strcpy(stringBuffer, currentBuffer);
	stringBuffer = strncat(stringBuffer, &gameBoard->boardValues[rowNumber][colNumber], 1);
	// Add U to the string buffer if the alphabet to add was Q
	if (gameBoard->boardValues[rowNumber][colNumber] == 'Q')
	{
		strncat(stringBuffer, "U", 1);
	}

	// Check if a word was reached along after processing current node
	if (traverseTo->wordIsPresent)
	{
		// Debugging
		//printf("%s\n", stringBuffer);

		// If the word is present then add it to the linked list
		if (!CheckIfWordIsInList(stringBuffer))
		{
			AddWordToList(stringBuffer);
		}
	}
	// Make recursive calls for all the neighbors not traversed yet
	if (rowNumber > 0)
	{
		if (!alreadySearched[rowNumber - 1][colNumber])
		{
			alreadySearched[rowNumber - 1][colNumber] = 1;
			_traverseStringPaths(rowNumber - 1, colNumber, gameBoard, traverseTo, stringBuffer);
			alreadySearched[rowNumber - 1][colNumber] = 0;
		}
		if (colNumber < NUM_COLS-1)
		{
			if (!alreadySearched[rowNumber - 1][colNumber + 1])
			{
				alreadySearched[rowNumber - 1][colNumber + 1] = 1;
				_traverseStringPaths(rowNumber - 1, colNumber + 1, gameBoard, traverseTo, stringBuffer);
				alreadySearched[rowNumber - 1][colNumber + 1] = 0;
			}
		}
		if (colNumber > 0)
		{
			if (!alreadySearched[rowNumber - 1][colNumber - 1])
			{
				alreadySearched[rowNumber - 1][colNumber - 1] = 1;
				_traverseStringPaths(rowNumber - 1, colNumber - 1, gameBoard, traverseTo, stringBuffer);
				alreadySearched[rowNumber - 1][colNumber - 1] = 0;
			}
		}

	}
	if (rowNumber < NUM_ROWS-1)
	{
		if (!alreadySearched[rowNumber + 1][colNumber])
		{
			alreadySearched[rowNumber + 1][colNumber] = 1;
			_traverseStringPaths(rowNumber + 1, colNumber, gameBoard, traverseTo, stringBuffer);
			alreadySearched[rowNumber + 1][colNumber] = 0;
		}
		if (colNumber < NUM_COLS-1)
		{
			if (!alreadySearched[rowNumber + 1][colNumber + 1])
			{
				alreadySearched[rowNumber + 1][colNumber + 1] = 1;
				_traverseStringPaths(rowNumber + 1, colNumber + 1, gameBoard, traverseTo, stringBuffer);
				alreadySearched[rowNumber + 1][colNumber + 1] = 0;
			}
		}
		if (colNumber > 0)
		{
			if (!alreadySearched[rowNumber + 1][colNumber - 1])
			{
				alreadySearched[rowNumber + 1][colNumber - 1] = 1;
				_traverseStringPaths(rowNumber + 1, colNumber - 1, gameBoard, traverseTo, stringBuffer);
				alreadySearched[rowNumber + 1][colNumber - 1] = 0;
			}
		}
	}
	if (colNumber > 0)
	{
		if (!alreadySearched[rowNumber][colNumber - 1])
		{
			alreadySearched[rowNumber][colNumber - 1] = 1;
			_traverseStringPaths(rowNumber, colNumber - 1, gameBoard, traverseTo, stringBuffer);
			alreadySearched[rowNumber][colNumber - 1] = 0;
		}
	}
	if (colNumber < NUM_COLS-1)
	{
		if (!alreadySearched[rowNumber][colNumber + 1])
		{
			alreadySearched[rowNumber][colNumber + 1] = 1;
			_traverseStringPaths(rowNumber, colNumber + 1, gameBoard, traverseTo, stringBuffer);
			alreadySearched[rowNumber][colNumber + 1] = 0;
		}
	}
	free(stringBuffer);
}

char8_t* GetStringBuffer()
{
	char8_t* bufferString = (char8_t*)malloc(MAX_CHARS_IN_DICTIONARY_WORD+1);
	return bufferString;
}

int32_t rangedRandom(int32_t min, int32_t max)
{
	return 0;
}