#ifndef TREE_H
#define TREE_H

#include <locale.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

//-------------------------------------------------------------------------------
typedef wchar_t _data_t;
typedef _data_t* data_t;

enum Child
{
	left 	= 1,
	right 	= 2
};

struct Node
{
	data_t data 		= 0;
	Node* left_child 	= 0;
	Node* right_child 	= 0;
};

//--------------------------------------------------------------------------------

const int some = 111;
Node* const OCCUPATED = (Node*)(&some);

// error_t ret_err = -1; (int) Node

// Node *temp = TreeAddNode(...);
// if ((size_t) temp == -1)
// {
// 	error_actions...
// }

//--------------------------------------------------------------------------------

Node* TreeAddNode(Node* tnode, Child side, Error* err = nullptr);

Error TreeAddValueToNode(Node* tnode, data_t val, int n_symb = -1);

Node* TreeSearchNodeByValue(Node* tnode, data_t val);

Error GraphTree(Node* tnode, const char* file_name);

Error GraphTreePrivate(Node* tnode, FILE* dot = nullptr, int depth = 0);

Error TreeLoad(const char* file_name, Node* tnode);

Error TreeRemoveNode(Node* tnode);

int TreeLoadNode(Node* tnode, data_t conteiner);

int TreeCpy(Node* tnode, Node* tnode_copied, Error* err = nullptr);

int TreeSave(Node* tnode, const char* file_name, Error* err = nullptr);

int TreeSaveNode(Node* tnode, FILE* out, int depth = 0, Error* err = nullptr);

#endif // TREE_H
