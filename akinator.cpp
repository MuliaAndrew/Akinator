#include "akinator.h"
#include "massages.h"

#define W wprintf
#define user_answer_yes !wcscmp(user, L"Да") || !wcscmp(user, L"да") || !wcscmp(user, L"д")
#define user_answer_no !wcscmp(user, L"Нет") || !wcscmp(user, L"нет") || !wcscmp(user, L"н")

Error AkiMaster(const char* file_name)
{
	Node* tree = (Node*)calloc(1, sizeof(Node));

	if (!TreeLoad("conteiner.txt", tree))
	{
		W(L"Возникли ошибки при запуске игры, игра будет закрыта\n");
		return LOAD_ERROR;
	}

	FILE* conteiner = fopen("conteiner.txt", "r");
	ptr_ver(conteiner);

	int seq_size = 0;
	fscanf(conteiner, "%d", &seq_size);

	char* answ_seq = (char*)calloc(seq_size, sizeof(char));
	ptr_ver(answ_seq);

	fclose(conteiner);

	while(true)
	{
		int mode = NO_MODE;

		wchar_t user[USER_MSG_LEN] = {0};
		int n_answs = 0;

		while(true)
		{
			W(select_modes, def_mode, view_mode, guess_mode,  exit_mode);

			W(L"[и/о/п/в]> ");
			wscanf(L"%s", user);

			for (int n = 0; user[n] != 0; n++)
				user[n] = 0;

			if (*user == L'и')
			{
				W(mode_msg, guess_mode);
				mode = GUESSING_MODE;
				break;
			}

			else if (*user == L'о')
			{
				W(mode_msg, add_mode);
				mode = DEFINITION_MODE;
				break;
			}

			else if (*user == L'п')
			{
				W(mode_msg, view_mode);
				mode = VIEWING_MODE;
				break;
			}

			else if (*user == L'в')
				return NO_ERROR;

			else
				W(unreg_answ);
		}

		for (int n = 0; user[n] != 0; n++)
			user[n] = 0;

		switch (mode)
		{
			case DEFINITION_MODE:
			{

				break;
			}

			case VIEWING_MODE:
			{
				GraphTree(tree, "AkinatorTree.dot");
				system("AkinatorTree.dot.pdf");
				break;
			}

			case GUESSING_MODE:
			{
				Node* master = tree;

				while (true)
				{
					if (master == nullptr)
						break;

					if (master->left_child == nullptr || master->right_child == nullptr)
					{
						W(answ_qstn, master->data);

						W(yes_no);
						wscanf(L"%s", user);

						if (user_answer_yes)
						{
							W(right_answ);

							break;
						}

						else if (user_answer_no)
						{
							W(false_answ);

							W(yes_no);
							wscanf(L"%s", user);

							while (true)
							{
								if (user_answer_yes)
								{
									AddCaseToAkinatorTree(tree, );
									break;
								}

								else if (user_answer_no)
									break;

								else
									W(unreg_answ);
							}
						}

						else
							W(unreg_answ);

						break;
					}

					while (true)
					{
						W(qstn, master->data);

						W(yes_no);
						wscanf(L"%s", user);

						if (user_answer_yes)
						{
							master = master->left_child;

							answ_seq[n_answs++] = left;

							break;
						}
						else if (user_answer_no)
						{
							master = master->right_child;

							answ_seq[n_answs++] = right;

							break;
						}
						else
							W(unreg_answ);
					}

					for (int n = 0; user[n] != 0; n++)
						user[n] = 0;
				}
				mode = NO_MODE;

				break;
			}
		}	
	}

	free(answ_seq);
	answ_seq = nullptr;

	TreeRemoveNode(tree);
}

Error AddCaseToAkinatorTree(Node* tree, char* answ_seq, Node* last_qstn)
{
	ptr_ver(tree);

	Node* new_qstn_node = (Node*)calloc(1, sizeof(Node));

	ptr_ver(new_qstn_node);

	int last_answ = 0;

	while (answ_seq[last_answ++] != 0);

	last_answ--;

	Node* wrong_answ = nullptr;

	if (answ_seq[last_answ] == left)
		wrong_answ = last_qstn->left_child;

	else if (answ_seq[last_answ] == right)
		wrong_answ = last_qstn->right_child;

	else
		return SEQ_ERROR;

	W(def_qstn, );
}

Error WordDefinition(Node* tree, char* answ_seq, Node* gueesed_word)
{
	W(decrptn, gueesed_word->data);

	Node* temp = tree;

	int n_answ = 0;

	while (temp->left_child != nullptr && temp->right_child != nullptr)
	{
		W(L" %s", temp->data);

		if (answ_seq[n_answ] == left)
		{
			temp = temp->left_child;
		}

		else if (answ_seq[n_answ] == right)
		{
			temp = temp->right_child;
		}

		else
			return SEQ_ERROR;

		n_answ++;

		if (temp->left_child == gueesed_word || temp-> right_child == gueesed_word)
			W(L".");

		else
			W(L",");
	}
}

#undef user_answer_yes
#undef user_answer_no
#undef W
