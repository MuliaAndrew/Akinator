#include "tree.h"
#include <time.h>

const int USER_MSG_LEN = 100;

enum Modes
{
	NO_MODE  		= 0,
	DEFINITION_MODE	= 1,
	GUESSING_MODE 	= 2,
	VIEWING_MODE  	= 3,
};

Error AkiMaster(const char* file_name);

Error AddCaseToAkinatorTree(Node* tree, char* answ_seq, Node* last_qstn);

Error WordDefinition(Node* tree, char* answ_seq, Node* gueesed_word);

Error WordsComparison(Node* tree);