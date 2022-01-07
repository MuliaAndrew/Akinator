#include "tree.h"

Node* TreeAddNode(Node* tnode, Child side, Error* err)
{
	if (err != nullptr)
	{
		if (tnode == nullptr)
			return nullptr;
	}

	if (side == left)
	{
		if (tnode->left_child != nullptr)
			return OCCUPATED;

		tnode->left_child = (Node*)calloc(1, sizeof(Node));
		return tnode->left_child;
	}

	else if (side == right)
	{
		if (tnode->right_child != nullptr)
			return OCCUPATED;

		tnode->right_child = (Node*)calloc(1, sizeof(Node));
		return tnode->right_child;
	}

	else
		return nullptr;
}

Error TreeAddValueToNode(Node* tnode, data_t val, int n_symb)
{
	ptr_ver(tnode);
	ptr_ver(val);

	if (n_symb == -1)
		n_symb = wcslen(val);

	if (n_symb == 0)
	{
		tnode->data = (data_t)calloc(1, sizeof(_data_t));
		ptr_ver(tnode->data);

		wcscmp(tnode->data, L" ");
	}

	if (tnode->data != nullptr)
		free(tnode->data);

	tnode->data = (data_t)calloc(n_symb, sizeof(_data_t));
	ptr_ver(tnode->data);

	wcsncpy(tnode->data, val, n_symb);

	return NO_ERROR;
}

Node* TreeSearchNodeByValue(Node* tnode, data_t val)
{
	if (val == nullptr)
		return nullptr;

	if (tnode->data == nullptr)
		return nullptr;

	if (wcscmp(tnode->data, val) == 0)
		return tnode;

	Node* temp_left = nullptr;
	Node* temp_right = nullptr;

	if (tnode->left_child != nullptr)
		temp_left = TreeSearchNodeByValue(tnode->left_child, val);

	if (tnode->right_child != nullptr)
		temp_right = TreeSearchNodeByValue(tnode->right_child, val);

	if (temp_left != nullptr)
		return temp_left;

	else if (temp_right != nullptr)
		return temp_right;

	else
		return nullptr;
}

Error TreeRemoveNode(Node* tnode)
{
	ptr_ver(tnode);

	Error temp_err = NO_ERROR;

	if (tnode->left_child != nullptr)
		temp_err = TreeRemoveNode(tnode->left_child);

	if (tnode->right_child != nullptr)
		temp_err = TreeRemoveNode(tnode->right_child);

	free(tnode);
	tnode = nullptr;

	return temp_err;
}

Error GraphTree(Node* tnode, const char* file_name)
{
	ptr_ver(tnode);
	ptr_ver(file_name);

	FILE* dot = fopen(file_name, "w+");

	ptr_ver(dot);

	Error err = GraphTreePrivate(tnode, dot);

	int len_cmd_msg = strlen(file_name) + strlen("dot -Tpdf -O ");

	char* cmd_msg = (char*)calloc(len_cmd_msg, sizeof(char));

	ptr_ver(cmd_msg);

	strcpy(cmd_msg, "dot -Tpdf -O ");

	strcat(cmd_msg, file_name);

	system(cmd_msg);

	return err;
}

Error GraphTreePrivate(Node* tnode, FILE* dot, int depth)
{
	ptr_ver(tnode);

	ptr_ver(dot);

	if (depth == 0)
	{
		fwprintf(dot, 	L"digraph tree {\n"
						L"	graph [rankdir=\"UD\"];\n"
						L"	node [shape=record];\n");
	}

	fwprintf(dot, L"	struct%0X [label=\"{<ptr> %0X | <data> %s | {<left> %0X | <right> %0X}}\"];\n", tnode, tnode, tnode->data, tnode->left_child, tnode->right_child);

	Node* temp_left = tnode->left_child;
	Node* temp_right = tnode->right_child;

	Error temp_err = NO_ERROR;

	if (temp_left != nullptr)
		temp_err = GraphTreePrivate(temp_left, dot, depth + 1);

	if (temp_right != nullptr)
		temp_err = GraphTreePrivate(temp_right, dot, depth + 1);

	if (temp_left != nullptr)
		fwprintf(dot, L"	struct%0X:left -> struct%0X:ptr;\n", tnode, temp_left);

	if (temp_right != nullptr)
		fwprintf(dot, L"	struct%0X:right -> struct%0X:ptr;\n", tnode, temp_right);

	if (depth == 0)
	{
		fwprintf(dot, L"}");
		fclose(dot);
	}

	return temp_err;
}

Error TreeLoad(const char* file_name, Node* tnode)
{
	ptr_ver(file_name);
	ptr_ver(tnode);

	FILE* load = fopen(file_name, "r");

	fseek(load, 0, SEEK_END);

	int len = ftell(load);

	fseek(load, 0, SEEK_SET);

	data_t conteiner = (data_t)calloc(len + 1, sizeof(_data_t));

	ptr_ver(conteiner);

	int n_nodes = 0;

	fwscanf(load, L"%d", &n_nodes);

	for (int pos = 0, node = 0; node < n_nodes; node++)
	{
		int prev = pos;
		fwscanf(load, L"%s", conteiner + pos);
		pos += wcslen(conteiner + pos) + 1;
		wprintf(conteiner + prev);
		wprintf(L"\n");
	}

	TreeLoadNode(tnode, conteiner);

	fclose(load);

	return NO_ERROR;
}

int TreeLoadNode(Node* tnode, data_t conteiner)
{
	int temp_len = wcslen(conteiner);

	int pos = 0;

	if ((conteiner[0] == L'\'') && (conteiner[temp_len - 1] == L'\''))
	{
		if (temp_len == 0)
			TreeAddValueToNode(tnode, L"none");
		else
			TreeAddValueToNode(tnode, conteiner + 1, temp_len - 2);

		pos = temp_len + 1;
	}

	else if ((conteiner[0] == L'[') && (conteiner[temp_len - 1] == L']'))
	{
		if (temp_len == 0)
			TreeAddValueToNode(tnode, L"none");
		else
			TreeAddValueToNode(tnode, conteiner + 1, temp_len - 2);;

		pos = temp_len + 1;

		if (wcscmp(conteiner + pos, L"\'\'") != 0)
		{
			tnode->left_child = TreeAddNode(tnode, left);
			pos += TreeLoadNode(tnode->left_child, conteiner + pos);
		}
		else
			pos += 3;

		if (wcscmp(conteiner + pos, L"\'\'") != 0)
		{
			tnode->right_child = TreeAddNode(tnode, right);
			pos += TreeLoadNode(tnode->right_child, conteiner + pos);
		}
		else
			pos += 3;
	}

	return pos;
}

int TreeSave(Node* tnode, const char* file_name, Error* err)
{
	ptr_ver(tnode);
	ptr_ver(file_name);

	FILE* out = fopen(file_name, "w");

	ptr_ver(out);

	int n_nodes = TreeSaveNode(tnode, out);

	fclose(out);

	return n_nodes;
}

int TreeSaveNode(Node* tnode, FILE* out, int depth, Error* err)
{
	if (out == nullptr || tnode == nullptr)
	{
		if (err != nullptr)
			*err = PTR_ERROR;
	}

	int n_nodes = 0;

	if (depth == 0)
		fputwc(L'\n', out);

	for (int tab = 0; tab < depth; tab++)
		fputwc(L'\t', out);

	if (tnode->right_child != nullptr || tnode->left_child != nullptr)
	{
		fputwc(L'[', out);
		fwprintf(out, L"%s", tnode->data);
		fputws(L"]\n", out);

		n_nodes += 1;

		if (tnode->left_child != nullptr)
			n_nodes += TreeSaveNode(tnode->left_child, out, depth + 1, err);

		else
		{
			for (int tab = 0; tab < depth + 1; tab++)
				fputwc(L'\t', out);

			fputws(L"\'\'\n", out);
		}

		if (tnode->right_child != nullptr)
			n_nodes += TreeSaveNode(tnode->right_child, out, depth + 1, err);

		else
		{
			for (int tab = 0; tab < depth + 1; tab++)
				fputwc(L'\t', out);

			fputws(L"\'\'\n", out);
		}
	}
	else
	{
		fputwc(L'\'', out);
		fwprintf(out, L"%s", tnode->data);
		fputws(L"\'\n", out);

		n_nodes += 1;
	}

	if (depth == 0)
	{
		fseek(out, 0, SEEK_SET);

		fwprintf(out, L"%d", n_nodes);
	}

	return n_nodes;
}

int TreeCpy(Node* tnode, Node* tnode_copied, Error* err)
{
	if (tnode == nullptr || tnode_copied == nullptr)
	{
		if (err != nullptr)
			*err = PTR_ERROR;
		
		return 0;
	}

	int n_copied = 0;

	*err = TreeAddValueToNode(tnode, tnode_copied->data);

	if (tnode_copied->left_child != nullptr)
	{
		tnode->left_child = TreeAddNode(tnode, left);

		if (tnode->left_child == nullptr)
		{
			*err = PTR_ERROR;
			return n_copied;
		}

		n_copied += TreeCpy(tnode->left_child, tnode_copied->left_child, err);
	}

	if (tnode_copied->right_child != nullptr)
	{
		tnode->right_child = TreeAddNode(tnode, right);

		if (tnode->right_child == nullptr)
		{
			*err = PTR_ERROR;
			return n_copied;
		}

		n_copied += TreeCpy(tnode->right_child, tnode_copied->right_child, err);
	}

	return n_copied;
}
