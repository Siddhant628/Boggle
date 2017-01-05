// Adds the valid words from the dictionary file filename into the Trie tree and stores the number of valid words in numWords
void parseDictionaryFile(char8_t *filename, int32_t *numWords);

// Returns non-zero if the parameter string is a valid word to add to the word list
int CheckWordValidity(char8_t* readWord);

// Adds the parameter string to the front of word list in memory
void AddWordToList(char8_t* readWord);

// Returns an initialized node for the dictionary Trie
struct trieNode* GetTrieNode();

// Adds the parameter word to the dictionary Trie tree
void AddWordToTrie(char8_t* readWord);

// Returns non-zero if the searchWord is present in the trie tree
int SearchForWord(char8_t* searchWord);

// Returns non-zero if the parameter word is in the linked list
int CheckIfWordIsInList(char8_t* wordToAdd);

// Returns the node traversed to from a node in the Trie for a specific alphabet as input
trieNode* TraverseAlongTrie(trieNode* currentNode, char8_t inputCharacter);

// Display all the words present in the Trie tree
void DisplayTrieTree();

// Recursively prints all the words present in the dictionary maintaining tree
void RecursivelyPrintTrie(trieNode* treeNode, char8_t* stringBuffer);

// Deallocated all the memory used by the linked list and resets the topOfList
void ClearList();