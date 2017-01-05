#define DICTIONARY_CPP
#define  _CRT_SECURE_NO_WARNINGS
#define MINIMUM_DICTIONARY_WORD_SIZE 4


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "Dictionary.h"
#include "Boggle.h"
#include "game.h"


void parseDictionaryFile(char8_t *filename, int32_t *numWords)
{
	FILE* dictionaryFile;
	char8_t* readWord;
	int32_t wordReadSuccessful;
	int32_t wordCounter = -1, wordIsValid = 1;
	// Open the dictionary text file
	dictionaryFile = fopen(filename, "r");
	if (dictionaryFile == nullptr)
	{
		printf("Error: Dictionary file not found.");
		exit(0);
	}
	// Allocate memory to temporary variable to read strings
	readWord = (char8_t *)malloc(MAX_CHARS_IN_DICTIONARY_WORD + 1);
	do
	{
		if (wordIsValid) wordCounter++;
		wordReadSuccessful = fscanf(dictionaryFile, "%s", readWord);
		if (wordReadSuccessful != 1)
		{
			break;
		}
		// Add word to the Trie tree if it is valid
		wordIsValid = CheckWordValidity(readWord);
		if (wordIsValid)
		{
			AddWordToTrie(readWord);
		}
	} while (wordReadSuccessful == 1);
	*numWords = wordCounter;
	free(readWord);
}

int CheckWordValidity(char8_t* readWord)
{
	// Check if the word is longer than the minimum allowed size
	uint32_t wordLength;
	wordLength = strlen(readWord);
	if (wordLength < MINIMUM_DICTIONARY_WORD_SIZE)
	{
		return 0;
	}
	// Check if the word has any unacceptable symbols
	char8_t charInWord;
	for (int32_t i = 0; i < wordLength; i++)
	{
		charInWord = *(readWord + i);
		if (charInWord < 'A' || charInWord > 'Z')
		{
			return 0;
		}
	}
	// Case: Valid string
	return 1;
}

struct trieNode* GetTrieNode()
{
	// Create a new node
	struct trieNode* newNode;
	newNode = (trieNode*)malloc(sizeof(trieNode));
	// Initialize occurrence of word to 0 (set to 1 when a dictionary word ends at the node)
	newNode->wordIsPresent = 0;
	// Initialize the children nodes to NULL (set to something when a string with path to the children nodes is found)
	for (int32_t i = 0; i < SIZE_OF_ALPHABET; i++)
	{
		newNode->trieChildren[i] = NULL;
	}
	return newNode;
}

void AddWordToTrie(char8_t* readWord)
{
	// Check if the tree has a root node, add if needed
	if (rootOfTree == NULL)
	{
		rootOfTree = GetTrieNode();
	}
	// Initialize the node to traverse along the path for the read word
	trieNode* traverseNode = rootOfTree;

	int32_t wordLength = strlen(readWord);
	int32_t indexOfAlphabet;
	// For each alphabet in the word, we move along the depth of the tree to the node corresponding to the alphabet
	for (int32_t depthInTree = 0; depthInTree < wordLength; depthInTree++)
	{
		// Find the index of the child node to traverse to
		indexOfAlphabet = (int32_t)*(readWord + depthInTree);
		indexOfAlphabet = indexOfAlphabet - (int32_t)'A';
		// If the child node doesn't point to any node, generate a new node (Case: There have been no previous words along the same path)
		if (traverseNode->trieChildren[indexOfAlphabet] == NULL)
		{
			traverseNode->trieChildren[indexOfAlphabet] = GetTrieNode();
		}
		// Traverse along the child node
		traverseNode = traverseNode->trieChildren[indexOfAlphabet];
	// Repeated until the entire string has been read
	}
	// Mark the presence of a dictionary word at the destination node
	traverseNode->wordIsPresent = 1;
}

trieNode* TraverseAlongTrie(trieNode* currentNode, char8_t inputCharacter)
{
	currentNode = currentNode->trieChildren[inputCharacter-'A'];
	return currentNode;
}

void DisplayTrieTree()
{
	// Perpare a string buffer for storing Trie words
	char8_t* stringBuffer = (char8_t* )malloc(MAX_CHARS_IN_DICTIONARY_WORD+1);
	strcpy(stringBuffer, "");
	// Display the words in Trie
	RecursivelyPrintTrie(rootOfTree, stringBuffer);
	free(stringBuffer);
}

void RecursivelyPrintTrie(trieNode* treeNode, char8_t* stringBuffer)
{
	// If there is no node at the current tree node, return as no possible word could be present
	if (treeNode == NULL)	return;
	// Display the word if present
	if (treeNode->wordIsPresent)
	{
		printf("%s\n", stringBuffer);
	}
	// Check for words along all possible subsequent paths, i.e. all 26 alphabets
	char8_t* tempString = (char8_t*)malloc(MAX_CHARS_IN_DICTIONARY_WORD+1);
	for (int i = 0; i < 26; i++)
	{
		char8_t temp = (char8_t)((int32_t)'A' + i);
		strcpy(tempString, stringBuffer);
		strncat(tempString, &temp, 1);
		RecursivelyPrintTrie(treeNode->trieChildren[i], tempString);
	}
	free(tempString);
}

int SearchForWord(char8_t* searchWord)
{
	int32_t indexOfAlphabet;
	int32_t wordLength = strlen(searchWord);
	// Initialize the node to traverse along the path to search for the word
	trieNode* traverseNode = rootOfTree;
	// Loop to traverse along tree depth to reach the destination node (last alphabet of the word)
	for (int32_t depthInTree = 0; depthInTree < wordLength; depthInTree++)
	{
		// Find the index of the child node to traverse to
		indexOfAlphabet = (int32_t)*(searchWord + depthInTree);
		indexOfAlphabet = indexOfAlphabet - (int32_t)'A';
		// The word isn't present in the dictionary if we can't traverse to child node for any alphabet of the searchWord
		if (traverseNode->trieChildren[indexOfAlphabet] == NULL)
		{
			return 0;
		}
		traverseNode = traverseNode->trieChildren[indexOfAlphabet];
	}
	// If the occurrence of word is non-zero at the destination node, it means that the word is present in the dictionary
	if (traverseNode->wordIsPresent != 0)
	{
		return 1;
	}
	// Case: The traversed string is just an intermediate string to an actual word
	return 0;
}

void AddWordToList(char8_t* readWord)
{
	// Create a new node
	wordListT* listItem = (wordListT *)malloc(sizeof(wordListT));
	// Copy the string into the node
	listItem->word = (char8_t *)malloc(strlen(readWord) + 1);
	strncpy(listItem->word, readWord, strlen(readWord) + 1);
	// Case: The list is empty
	if (topOfWordList == NULL)
	{
		listItem->nextWord = NULL;
	}
	// Case the list isn't empty
	else
	{
		listItem->nextWord = topOfWordList;
	}
	// New node becomes the front of the list
	topOfWordList = listItem;
}

void ClearList()
{
	wordListT* tempListNode;
	wordListT* traverserListNode = topOfWordList;
	// Loop through list and remove all words
	while (traverserListNode != NULL)
	{
		tempListNode = traverserListNode;
		traverserListNode = traverserListNode->nextWord;
		free(tempListNode);
	}
	// Reset the top of word list
	topOfWordList = NULL;
}

int CheckIfWordIsInList(char8_t* wordToAdd)
{
	wordListT* listTraverser = topOfWordList;
	while (listTraverser != NULL)
	{
		if (!strcmp(listTraverser->word,wordToAdd))
		{
			return 1;
		}
		listTraverser = listTraverser->nextWord;
	}
	return 0;
}