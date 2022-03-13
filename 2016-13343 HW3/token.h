#pragma once
using namespace std;

/*
* Token for tokenizing the command to token list
* linked list, linked in both ways(before, next)
*/
class Token {
public:
	Token* before;
	string symbol;
	Token* next;
	Token(string symbol) {
		this->symbol = symbol;
		before = 0;
		next = 0;
	}
};

/*
* Token list used for tokenizing the command
* linked list, linked in both ways(before, next)
* always has dummy first node
* last: pointer pointing at the last token node
* now: pointer pointing at the token node which is being read now
* void addSymbol(string symbol): make new symbol into token and add at the end of the list
* void resetList(): free all nodes except dummy first node
* void InitializeTokenizer(string command): converts the string command to a token list
* void PushBack(): move to the node before now
* string GetNextToken(): read the next node
*/
class TokenList {
private:
	Token* first;
	TokenList() {
		first = new Token("dummy");
		last = first;
		now = first;
	}
	Token* last;
	Token* now;
	void addSymbol(string symbol);
	void resetList();
	void InitializeTokenizer(string command);
	void PushBack();
	string GetNextToken();
public:
	friend class Procedure;
};

//make new symbol into token and add at the end of the list
void TokenList::addSymbol(string symbol) {
	last->next = new Token(symbol);
	last->next->before = last;
	last = last->next;
}

//free all nodes except dummy first node
void TokenList::resetList() {
	now = first;

	while (last != first) {
		Token* temp = last->before;
		delete last;
		last = temp;
	}
	first->next = 0;
}

/*
* converts the string command to a token list
* tokens seperated by blanks or '(', ')' are stored to seperate tokens
*/
void TokenList::InitializeTokenizer(string command) {
	resetList();
	string token = "";
	int tokenLen = 0;
	for (int i = 0; i < command.length(); i++) {
		if ((command[i] == ' ')) {
			if (tokenLen != 0) {
				addSymbol(token);
				token = "";
				tokenLen = 0;
			}
		}
		else if (command[i] == '(') {
			addSymbol("(");
		}
		else if (command[i] == ')') {
			if (tokenLen != 0) {
				addSymbol(token);
				token = "";
				tokenLen = 0;
			}
			addSymbol(")");
		}
		else {
			token += command[i];
			tokenLen++;
		}
	}
}

//move to the node before now
void TokenList::PushBack() {
	now = now->before;
}

//read the next node
string TokenList::GetNextToken() {
	now = now->next;
	return now->symbol;
}