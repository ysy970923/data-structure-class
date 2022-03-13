#pragma once
#include <string>
#include <iostream>
#include "memory.h"
#include "symbol_table.h"
#include "token.h"
#include "exception.h"
using namespace std;

/*
* Procedure for Scheme Interpreter
* currently input commands only in one line
* currently does not evaluate the scheme command, but only stores in memory and symbol table
* Memory memory: main memory used to store the memory tree
* SymbolTable sybolTable: hash table used to store symbols
* TokenList tokenList: token list generated from input command
* string GetCommand(): gets one line console input as command
* int Read(): reads the token list and store in memory and symbol table
* void Print(int root, bool startList): prints the memory tree in linked order
* void PrintResult(int root): prints the whole process of the Procedure
* void Main(): repeats whole procedure infinite times
*/
class Procedure {
private:
	Memory memory;
	SymbolTable symbolTable;
	TokenList tokenList;
	string GetCommand();
	int Read();
	void Print(int root, bool startList);
	void PrintResult(int root);

public:
	Procedure() {
		memory = Memory();
		tokenList = TokenList();
		symbolTable = SymbolTable();
	}
	void Main();
};

/*
* gets and returns one line console input as command
* converts all characters in lowercase
*/
string Procedure::GetCommand() {
	string command;
	cout << endl<< "> ";
	string str;
	getline(cin, str);
	cout << endl;
	for (int i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	command = str;
	return command;
}

/*
* reads the token list and store in memory and symbol table
* store tokens in tree structure
* returns the root of tree
*/
int Procedure::Read() {
	int root = 0;
	bool first = true;
	int temp = 0;
	int tokenHashValue = symbolTable.GetHashValue(tokenList.GetNextToken());

	/*
	* when left param is read
	* start reading until right param is read
	*/
	if (tokenHashValue == symbolTable.GetHashValue("(")) {
		while ((tokenHashValue = symbolTable.GetHashValue(tokenList.GetNextToken())) != symbolTable.GetHashValue(")")) {
			//where first token after left param is stored is returned as root 
			if (first) {
				temp = memory.Alloc();
				root = temp;
				first = false;
			}
			//allocate new memory to right child
			else {
				memory.changeChild(temp, 'R', memory.Alloc());
				temp = memory.getChild(temp, 'R');
			}
			/*
			* when left param is read
			* push back to read left param again
			* add to left child the new subtree made by recursively reading from left param
			*/
			if (tokenHashValue == symbolTable.GetHashValue("(")) {
				tokenList.PushBack();
				memory.changeChild(temp, 'L', Read());
				memory.changeChild(temp, 'R', 0);
			}
			//if not left param store the hash value of the token in the left child
			else {
				memory.changeChild(temp, 'L', tokenHashValue);
				memory.changeChild(temp, 'R', 0);
			}
		}
		return root;
	}
	//if anything other than left param is read first, just return the token value
	else {
		return tokenHashValue;
	}
}

/*
* prints the memory tree in linked order
* symbols and params are printed in order
* should be printed without blanks
*/
void Procedure::Print(int root, bool startList) {
	if (root == 0)
		cout << "()";
	else if (root < 0) {
		cout << symbolTable.GetSymbol(root);
	}
	else if (root > 0) {
		if (startList) {
			cout << "(";
		}
		Print(memory.getChild(root, 'L'), true);
		if (memory.getChild(root, 'R') != 0)
			Print(memory.getChild(root, 'R'), false);
		else cout << ")";
	}
}

/*
* prints the whole process of the Procedure
* starts with ']'
* Free list's root = (root of free list)
* List's root = (root of the memory tree made with given command)
* print all elements memory list
* print not empty symbol table entries
* print memory tree in linked order
* 
* Frees the memory allocated after print
*/
void Procedure::PrintResult(int root) {
	cout << "] ";
	cout << "Free list's root = " << memory.free_list_root << endl;
	cout << "List's root = " << root << endl;
	memory.Print();
	symbolTable.Print();
	cout << endl;
	Print(root, true);
	cout << endl;
	memory.FreeMemory(root);
}

/*
* repeats whole procedure infinite times
* procedure:
* get command
* initialize tokenizer: make command to token list
* read: read token list and store to symbol table and memory
* print the result of the procedure
*/
void Procedure::Main() {
	while (true) {
		string command = GetCommand();
		tokenList.InitializeTokenizer(command);
		int root = 0;
		try {
			root = Read();
		}
		catch (int error) {
			if (error == FULL_SYMBOL_TABLE_EXCEPTION) 
				cout << "[ERROR] symbol table is full";
			
			if (error == FULL_MEMORY_EXCEPTION)
				cout << "[ERROR] memory is full";

			if (error == NOT_VALID_CHILD)
				cout << "[ERROR] not a valid binary child of memory";

			break;
		}
		PrintResult(root);
	}
}
