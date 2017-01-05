#ifndef GAME_H
#define GAME_H

#define SIZE_OF_ALPHABET (26)

#ifdef GAME_CPP
#define extern 
#endif

// A struct to store words searched on the game board, in a singly linked list fashion
struct wordList
{
	char8_t *word;
	struct wordList *nextWord;
};
typedef struct wordList wordListT;

// A struct to store dictionary words in a Trie Tree data structure
struct trieNode
{
	trieNode* trieChildren[SIZE_OF_ALPHABET];
	uchar8_t wordIsPresent;
};

// A struct for maintaining a game board
struct GameBoard
{
	char8_t boardValues[NUM_ROWS][NUM_COLS];
};


extern wordListT* topOfWordList;
extern trieNode* rootOfTree;

void initGame();

void buildRandomBoard();
// Returns a pointer to a randomly generated game board
struct GameBoard* _buildRandomBoard();


void searchForWords();
// Search for words present in the parameter game board
void _searchForWords(GameBoard* gameBoard);

// Recursive algorithm for finding all words present on the board starting from the alphabet at the location (i,j) on the board
void _traverseStringPaths(int32_t i, int32_t j, GameBoard* gameBoard, trieNode*, char8_t* );

void printBoard();
// Display the parameter game board on the console window
void _printBoard(GameBoard* gameBoard);

void printWords();
// Prints all the words stored in the linked list on the console window
void _printWords();

void resetGame();
// Resets the game for repeated run
void _resetGame();

// Resets the 2D array of visited positions on the game board (Marks all positions unvisited)
void _clearAlreadySearched();

// Returns a pointer to a string buffer of the size same as the maximum possible string size
char8_t* GetStringBuffer();

int32_t rangedRandom(int32_t min, int32_t max);

#undef extern

#endif