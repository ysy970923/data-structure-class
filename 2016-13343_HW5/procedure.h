#pragma once
#include <string>
#include <iostream>
#include "memory.h"
#include "symbol_table.h"
#include "token.h"
#include "exception.h"
#include "eval_stack.h"
#include "history_stack.h"
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
* void checkStructure(): used when equal? operator
* void clean_data(): used to wipe data when error occured
* string PreProcess(): pre process the given command
* int Eval(int root): evaluate the subtree from given root
*/
class Procedure {
private:
	void exception_handle(int error);
	Memory memory;
	SymbolTable symbolTable;
	TokenList tokenList;
	EvalStack evalStack;
	HistoryStack history;

	void garbage_collection();

	void back();

	bool checkStructure(int root1, int root2);

	string GetCommand();
	string PreProcess();
	int Read();

	int Eval(int root);

	string Print(int root, bool startList);
	
	void PrintResult(int root);

public:
	Procedure() {
		memory = Memory(history);
		tokenList = TokenList();
		symbolTable = SymbolTable(history);
		evalStack = EvalStack();
	}
	void Main();
};

/*
* used when error is occurred
* undo every change made to memory and symbolTable in this operation
* use stack for saving and undoing changes
* after changes are undone, wipe evaluation stack in case it was being used
* set memory free list root to history free list root which is the free list root before
*/
void Procedure::back() {
	while (history.last != 0) {
		HistoryStackNode* temp = history.last;
		if (temp->address > 0) {
			memory.changeChild(temp->address, temp->historyData.memoryData.LorR, temp->historyData.memoryData.child, false);
		}
		if (temp->address < 0) {
			symbolTable.changePointer(temp->address, temp->historyData.tableData.pointer, false);
		}
		history.Pop();
	}
	evalStack.wipeStack();
	memory.free_list_root = history.free_list_root;
}

/*
* called when memory is full
* collect garbage memory then free garbage memory
* garbage memory: memory not pointed by any symbol on the table
*	=> memory which can't be used anymore
*/
void Procedure::garbage_collection() {
	for (int i = 0; i < HASHTABLE_SIZE; i++) {
		if (symbolTable.hashTable[i].pointer > 0) {
			memory.SaveMemory(symbolTable.hashTable[i].pointer);
		}
	}
	memory.garbageCollect();
}

/*
* check if two roots have the same sub tree structure
*/
bool Procedure::checkStructure(int root1, int root2) {
	if (root1 <= 0 || root2 <= 0)
		return (root1 == root2);

	else
		return checkStructure(memory.getChild(root1, 'L'), memory.getChild(root2, 'L')) && checkStructure(memory.getChild(root1, 'R'), memory.getChild(root2, 'R'));
}

/*
* gets and returns one line console input as command
* converts all characters in lowercase
*/
string Procedure::GetCommand() {
	string command;
	string str;
	cout << "> ";
	getline(cin, str);
	cout << endl;
	for (int i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	while (str.length() == 0) {
		cout << "> ";
		getline(cin, str);
		cout << endl;
		for (int i = 0; i < str.length(); i++) {
			str[i] = tolower(str[i]);
		}
	}
	command = str;

	return command;
}

/*
* preprocessing the given command token list
* does lambda transform after function define 
* does quote transform when "'" appears
*/
string Procedure::PreProcess() {
	string newcommand = "";
	while (!tokenList.IsNextTokenEmpty()) {
		string token = tokenList.GetNextToken();
		if (token == "define") {
			newcommand += "define ";
			token = tokenList.GetNextToken();
			if (token == "(") {
				token = tokenList.GetNextToken();
				newcommand = newcommand + " " + token + " " + "(lambda(" + PreProcess() + ")";
			}
			else {
				tokenList.PushBack();
			}
		}
		else if (token == "'") {
			newcommand += "(quote";
			int number_of_left_paren = 0;
			do {
				token = tokenList.GetNextToken();
				newcommand += " " + token + " ";
				if (token == "(") {
					number_of_left_paren++;
				}
				else if (token == ")") {
					number_of_left_paren--;
				}
			} while (number_of_left_paren > 0);
			newcommand += ")";
		}
		//blank space between token strings for tokenizer to add as token 
		else
			newcommand += " " + token + " ";
	}
	return newcommand;

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
* evaluate the subtree from given root
* Eval(0) = 0
* Eval(-k) / when minus integer is evaluated
*	= return -k when no pointer attached (symbol or number)
*	= return -k when it is a number
*	= return pointer when pointer is attached
* Eval(k) / when plus integer is evaluated
*	k means memory index
*	token_index := memory[k].lchild
*	compare whether token_index is a embedded function or operator (+, -, *, =, number?, eq?, ...)
*	execute embedded function or operator
*	if token_index is user-defined function use evalStack to control parameters
*	when token_index is not a function or operator, return Eval(left child)
*/
int Procedure::Eval(int root) {
	if (root == 0) {
		return root;
	}
	if (root < 0) {
		//pointer == 0: null
		if (symbolTable.getPointer(root) == 0)
			return 0;
		if (symbolTable.isNumber(root))
			return root;
		//pointer == -(HASHTABLE_SIZE + 1): no pointer is attached (means undefined symbol)
		if (symbolTable.getPointer(root) == -(HASHTABLE_SIZE + 1)) {
			throw UNDEFINED_SYMBOL;
		}
		return symbolTable.getPointer(root);
	}
	int token_index = memory.getChild(root, 'L');
	if (token_index >= 0)
		return Eval(token_index);

	if (token_index == symbolTable.GetHashValue("+")) {
		int param1 = Eval(memory.getChild(memory.getChild(root, 'R'), 'L'));
		int param2 = Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'));
		if (symbolTable.isNumber(param1) && symbolTable.isNumber(param2)) {
			if (symbolTable.isInt(param1) && symbolTable.isInt(param2))
				return symbolTable.GetHashValue(to_string(symbolTable.GetIntVal(param1) + symbolTable.GetIntVal(param2)));
			return symbolTable.GetHashValue(to_string(symbolTable.GetFloatVal(param1) + symbolTable.GetFloatVal(param2)));
		}
		throw NUMERIC_NOT_NUMBER_PARAM;
	}
	else if (token_index == symbolTable.GetHashValue("-")) {
		int param1 = Eval(memory.getChild(memory.getChild(root, 'R'), 'L'));
		int param2 = Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'));
		if (symbolTable.isNumber(param1) && symbolTable.isNumber(param2)) {
			if (symbolTable.isInt(param1) && symbolTable.isInt(param2))
				return symbolTable.GetHashValue(to_string(symbolTable.GetIntVal(param1) - symbolTable.GetIntVal(param2)));
			return symbolTable.GetHashValue(to_string(symbolTable.GetFloatVal(param1) - symbolTable.GetFloatVal(param2)));
		}
		throw NUMERIC_NOT_NUMBER_PARAM;
	}
	else if (token_index == symbolTable.GetHashValue("*")) {
		int param1 = Eval(memory.getChild(memory.getChild(root, 'R'), 'L'));
		int param2 = Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'));
		if (symbolTable.isNumber(param1) && symbolTable.isNumber(param2)) {
			if (symbolTable.isInt(param1) && symbolTable.isInt(param2))
				return symbolTable.GetHashValue(to_string(symbolTable.GetIntVal(param1) * symbolTable.GetIntVal(param2)));
			return symbolTable.GetHashValue(to_string(symbolTable.GetFloatVal(param1) * symbolTable.GetFloatVal(param2)));
		}
		throw NUMERIC_NOT_NUMBER_PARAM;
	}
	else if (token_index == symbolTable.GetHashValue("eq?")) {
		if (Eval(memory.getChild(memory.getChild(root, 'R'), 'L')) == Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'))) {
			return symbolTable.GetHashValue("#t");
		}
		else {
			return symbolTable.GetHashValue("#f");
		}
	}
	else if (token_index == symbolTable.GetHashValue("equal?")) {
		if (checkStructure(Eval(memory.getChild(memory.getChild(root, 'R'), 'L')), Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L')))) {
			return symbolTable.GetHashValue("#t");
		}
		else {
			return symbolTable.GetHashValue("#f");
		}
	}
	else if (token_index == symbolTable.GetHashValue("=")) {
		int param1 = Eval(memory.getChild(memory.getChild(root, 'R'), 'L'));
		int param2 = Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'));
		if (symbolTable.isNumber(param1) && symbolTable.isNumber(param2)) {
			if (param1 == param2) {
				return symbolTable.GetHashValue("#t");
			}
			else {
				return symbolTable.GetHashValue("#f");
			}
		}
		throw NUMERIC_NOT_NUMBER_PARAM;
	}
	else if (token_index == symbolTable.GetHashValue("number?")) {
		if (symbolTable.isNumber(Eval(memory.getChild(memory.getChild(root, 'R'), 'L')))) {
			return symbolTable.GetHashValue("#t");
		}
		else {
			return symbolTable.GetHashValue("#f");
		}
	}
	else if (token_index == symbolTable.GetHashValue("symbol?")) {
		if (!symbolTable.isNumber(Eval(memory.getChild(memory.getChild(root, 'R'), 'L')))) {
			return symbolTable.GetHashValue("#t");
		}
		else {
			return symbolTable.GetHashValue("#f");
		}
	}
	else if (token_index == symbolTable.GetHashValue("null?")) {
		if (memory.getChild(root, 'R') == 0 || Eval(memory.getChild(root, 'R')) == 0) {
			return symbolTable.GetHashValue("#t");
		}
		else {
			return symbolTable.GetHashValue("#f");
		}
	}
	else if (token_index == symbolTable.GetHashValue("Cons")) {
		int newmemory = memory.Alloc();
		memory.changeChild(newmemory, 'L', Eval(memory.getChild(memory.getChild(root, 'R'), 'L')));
		memory.changeChild(newmemory, 'R', Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L')));
		return newmemory;
	}
	else if (token_index == symbolTable.GetHashValue("Cond")) {
		while (memory.getChild(memory.getChild(root, 'R'), 'R') != 0) {
			root = memory.getChild(root, 'R');
			if (Eval(memory.getChild(memory.getChild(root, 'L'), 'L')) == symbolTable.GetHashValue("#t")) {
				return Eval(memory.getChild(memory.getChild(root, 'L'), 'R'));
			}
		}
		if (memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'L'), 'L') != symbolTable.GetHashValue("Else")) {
			throw COND_STATEMENT_ERROR;
		}
		return Eval(memory.getChild(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'L'), 'R'), 'L'));
	}
	else if (token_index == symbolTable.GetHashValue("Car")) {
		int returnVal = memory.getChild(Eval(memory.getChild(memory.getChild(root, 'R'), 'L')), 'L');
		return returnVal;
	}
	else if (token_index == symbolTable.GetHashValue("Cdr")) {
		return memory.getChild(Eval(memory.getChild(memory.getChild(root, 'R'), 'L')), 'R');
	}
	else if (token_index == symbolTable.GetHashValue("Quote")) {
		return memory.getChild(memory.getChild(root, 'R'), 'L');
	}
	else if (token_index == symbolTable.GetHashValue("define")) {
		if (memory.getChild(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'), 'L') == symbolTable.GetHashValue("lambda")) {
			symbolTable.changePointer(memory.getChild(memory.getChild(root, 'R'), 'L'),
				memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L'));
		}
		else {
			symbolTable.changePointer(memory.getChild(memory.getChild(root, 'R'), 'L'),
				Eval(memory.getChild(memory.getChild(memory.getChild(root, 'R'), 'R'), 'L')));
		}
		return root;
	}
	else if (memory.getChild(symbolTable.getPointer(token_index), 'L') == symbolTable.GetHashValue("lambda")) {
		int memIndexOfParam = memory.getChild(memory.getChild(symbolTable.getPointer(token_index), 'R'), 'L');
		int memIndexOfValue = memory.getChild(root, 'R');
		int paramNum = 0;
		while (memIndexOfParam != 0) {
			paramNum++;
			evalStack.Push(memory.getChild(memIndexOfParam, 'L'), symbolTable.getPointer(memory.getChild(memIndexOfParam, 'L')));
			symbolTable.changePointer(memory.getChild(memIndexOfParam, 'L'), Eval(memIndexOfValue));
			memIndexOfParam = memory.getChild(memIndexOfParam, 'R');
			memIndexOfValue = memory.getChild(memIndexOfValue, 'R');
		}
		int result = Eval(memory.getChild(memory.getChild(memory.getChild(symbolTable.getPointer(memory.getChild(root, 'L')), 'R'), 'R'), 'L'));
		for (int i = 0; i < paramNum; i++) {
			symbolTable.changePointer(evalStack.last->hashvalue, evalStack.last->pointer);
			evalStack.Pop();
		}
		return result;
	}
	return Eval(memory.getChild(root, 'L'));
}

/*
* prints the memory tree in linked order
* symbols and params are printed in order
* should be printed without blanks
*/
string Procedure::Print(int root, bool startList) {
	string printString = "";
	if (root == 0)
		printString += "()";
	else if (root < 0) {
		printString += symbolTable.GetSymbol(root);
		printString += " ";
	}
	else if (root > 0) {
		if (memory.getChild(root, 'L') == symbolTable.GetHashValue("define"))
			return "";
		if (startList) {
			printString += "(";
		}
		printString += Print(memory.getChild(root, 'L'), true);
		if (memory.getChild(root, 'R') != 0)
			printString += Print(memory.getChild(root, 'R'), false);
		else {
			if (printString.back() == ' ')
				printString.erase(printString.length() - 1);
			printString += ")";
		}
	}
	return printString;
}

/*
not used in this project
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
	//memory.FreeMemory(root);
}

/*
* repeats whole procedure infinite times
* procedure:
* get command
* initialize tokenizer: make command to token list
* read: read token list and store to symbol table and memory
* print the result of the procedure
* if memory is not enough after garbage collection, stop the process
*/
void Procedure::Main() {
	int garbageCollect = false;
	string command;
	while (true) {
		int result = 0;
		try {
			if (garbageCollect == false) {
				history.wipeStack();
				history.free_list_root = memory.free_list_root;
				command = GetCommand();
			}
			tokenList.InitializeTokenizer(command);
			string newcommand = PreProcess();
			tokenList.InitializeTokenizer(newcommand);
			int root = 0;
			root = Read();
			result = Eval(root);
			string printString = Print(result, true);
			cout << "] " << printString << endl << endl;
			garbageCollect = false;
		}
		catch (int error) {
			exception_handle(error);
			if (error == FULL_MEMORY_EXCEPTION) {
				if (garbageCollect) {
					cout << "[SchemeError] garbage collection not enough for memory allocation, all process stop" << endl;
					break;
				}
				garbageCollect = true;
			}
		}
	}
}

/*
* called if any error is occurred
* for any error cancel the current command evaluating and go back to last stage
* if error is a full memory error, call garbage collect for garbage collection
*/
void Procedure::exception_handle(int error) {
	if (error == FULL_SYMBOL_TABLE_EXCEPTION)
		cout << "[SchemeError] symbol table is full" << endl;

	else if (error == FULL_MEMORY_EXCEPTION)
		cout << "[SchemeError] memory is full" << endl;

	else if (error == NOT_VALID_CHILD)
		cout << "[SchemeError] not a valid binary child of memory" << endl;

	else if (error == COND_STATEMENT_ERROR)
		cout << "[SchemeError] else is required in condition statement" << endl;

	else if (error == NUMERIC_NOT_NUMBER_PARAM)
		cout << "[SchemeError] numeric operation(+, -, *, =) parameters should be numbers" << endl;

	else if (error == MISMATCH_LEFT_RIGHT_PAREN)
		cout << "[SchemeError] left, right paren does not match match" << endl;

	else if (error == INVALID_SYMBOL_VALUE)
		cout << "[SchemeError] not on symbol table, symbol table address should not be over 0" << endl;

	else if (error == NUMBER_POINTER_ERROR)
		cout << "[SchemeError] number should not be allocated to a new value" << endl;

	else if (error == UNDEFINED_SYMBOL)
		cout << "[SchemeError] undefined symbol" << endl;
	
	cout << "==========================================================" << endl;

	cout << "all data including memory and hash table will set back" << endl;
	back();
	cout << "all data including memory and hash table has been set back" << endl;

	cout << "==========================================================" << endl;

	if (error == FULL_MEMORY_EXCEPTION) {
		cout << "garbage collection start" << endl;
		garbage_collection();
		cout << "garbage collection end" << endl;
	}
}
