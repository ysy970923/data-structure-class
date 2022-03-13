#pragma once
#include "exception.h"
using namespace std;

#define HASHTABLE_SIZE 100

/*
* element for each slot on the hash table
* bool empty: check whether the slot is empty
* string symbol: the symbol in the slot
* int link_of_value: will be used in further assignments
*/
class hashTableElement {
private:
	friend class SymbolTable;
	bool empty;
	string symbol;
	int pointer;
	hashTableElement() {
		empty = true;
		symbol = "";
		pointer = -1;
	}
};

/*
* hash table which contains the symbols
* size: HASHTABLE_SIZE
* GetHashValue(string token): stores the token on the table and returns the hash value of the token
* GetSymbol(int index): returns the symbol at the given hashValue
*/
class SymbolTable {
private:
	friend class Procedure;
	hashTableElement* hashTable;

	void wipeTable();

	int hashValToIndex(int hashVal);
	int GetHashValue(string token);

	bool isNumber(string str);
	bool isNumber(int hashVal);

	bool isInt(string str);
	bool isInt(int hashVal);

	bool isFloat(string str);

	string GetSymbol(int hashVal);

	int GetIntVal(int hashVal);
	float GetFloatVal(int hashVal);

	void changePointer(int hashVal, int toWhat);
	int getPointer(int hashVal);

	void Print();
	SymbolTable() {
		hashTable = new hashTableElement[HASHTABLE_SIZE]{ hashTableElement()};
		GetHashValue("+");
		GetHashValue("-");
		GetHashValue("*");
		GetHashValue("eq?");
		GetHashValue("equal?");
		GetHashValue("=");
		GetHashValue("number?");
		GetHashValue("symbol?");
		GetHashValue("null?");
		GetHashValue("CONS");
		GetHashValue("COND");
		GetHashValue("else");
		GetHashValue("Car");
		GetHashValue("Cdr");
		GetHashValue("define");
		GetHashValue("quote");
		GetHashValue("#t");
		GetHashValue("#f");
	}
};

/*
* used when error occured and symbol table needs to be wiped
*/
void SymbolTable::wipeTable() {
	for (int i = 0; i < HASHTABLE_SIZE; i++) {
		hashTable[i].empty = true;
		hashTable[i].symbol = "";
		hashTable[i].pointer = -1;
	}
}

/*
* used when checking if the given string is a number string
*/
bool SymbolTable::isNumber(string str) {
	if (str.empty())
		return false;

	if (str[0] == '-') {
		str = str.erase(0, 1);
	}

	bool hasDotAppeared = false;

	return find_if(str.begin(), str.end(),
		[&hasDotAppeared](unsigned char c) {
			if (c == '.') {
				if (hasDotAppeared)
					return true;
				else {
					hasDotAppeared = true;
					return false;
				}
			}
			return !isdigit(c); }) == str.end();
}

/*
* used when checking if the given hashVal's hashTable symbol is a number string
*/
bool SymbolTable::isNumber(int hashVal)
{
	string str = hashTable[hashValToIndex(hashVal)].symbol;
	return isNumber(str);
}

/*
* check if given string is a integer string
*/
bool SymbolTable::isInt(string str) {
	if (str.empty())
		return false;

	if (str[0] == '-') {
		str = str.erase(0, 1);
	}

	return find_if(str.begin(), str.end(),
		[](unsigned char c) {return !isdigit(c); }) == str.end();
}

/*
* check if given hashVal's hashTable symbol is a integer string
*/
bool SymbolTable::isInt(int hashVal) {
	string str = hashTable[hashValToIndex(hashVal)].symbol;
	return isInt(str);
}

/*
* check if given string is a float string (number string with a point)
*/
bool SymbolTable::isFloat(string str) {
	return isNumber(str) && !isInt(str);
}

//used to convert the hashValue to valid index of the table array
int SymbolTable::hashValToIndex(int hashVal) {
	return -(hashVal + 1);
}

// change pointer to a given value
void SymbolTable::changePointer(int hashVal, int toWhat) {
	hashTable[hashValToIndex(hashVal)].pointer = toWhat;
}

//get pointer of a given value
int SymbolTable::getPointer(int hashVal) {
	return hashTable[hashValToIndex(hashVal)].pointer;
}

//print hash table
void SymbolTable::Print() {
	cout << endl << "Hash Table = " << endl;
	for (int i = 0; i < HASHTABLE_SIZE; i++) {
		if (!hashTable[i].empty)
			cout << "Hash Value " << -(i + 1) << ": " << "symbol: " << hashTable[i].symbol << "  pointer: " << hashTable[i].pointer << endl;
	}
}

//returns the symbol at the given hashValue
string SymbolTable::GetSymbol(int hashVal) {
	return hashTable[hashValToIndex(hashVal)].symbol;
}

//get the given hashVal's symbol in integer
int SymbolTable::GetIntVal(int hashVal) {
	int returnVal = stoi(hashTable[hashValToIndex(hashVal)].symbol);
	return returnVal;
}

//get the given hashVal's symbol in float
float SymbolTable::GetFloatVal(int hashVal) {
	float returnVal = stof(hashTable[hashValToIndex(hashVal)].symbol);
	return returnVal;
}

//used to format float string
// ex) 1.0000 --> 1.0
//     3.1230 --> 3.123
string floatStringFormatter(string str) {
	string token = str;
	bool floatingPoint = false;
	for (int i = 0; i < token.length(); i++) {
		if (token[i] == '.') {
			floatingPoint = true;
			break;
		}
	}
	int eraseStartIndex = token.length();
	if (floatingPoint) {
		for (int i = token.length() - 1; i >= 0; i--) {
			if (token[i] == '0') {
				eraseStartIndex = i;
			}
			else if (token[i] == '.') {
				token += "0";
				eraseStartIndex = i + 2;
				break;
			}
			else
				break;
		}
		token = token.erase(eraseStartIndex, token.length());
	}
	return token;
}

/*
* stores the token on the table and returns the hash value of the token
* hash function: hashVal = - (sum % HASHTABLE_SIZE)
* if hash table entry is not empty, move to next entry
* move until empty entry or match
* if empty entry, fill the entry and return the index
* if entry's symbol is match, return the index
*/
int SymbolTable::GetHashValue(string str) {
	string token = str;
	if (isFloat(token))
		token = floatStringFormatter(token);
	for (int i = 0; i < token.length(); i++) {
		token[i] = tolower(token[i]);
	}
	int sum = 0;
	for (int i = 0; i < token.length(); i++) {
		sum += token[i];
	}
	int hashVal = - (sum % HASHTABLE_SIZE);
	if (hashVal == 0) 
		hashVal = - HASHTABLE_SIZE;
	int startHashVal = hashVal;
	while (!hashTable[hashValToIndex(hashVal)].empty) {
		bool stringMatch = true;
		string hashTableSymbol = hashTable[hashValToIndex(hashVal)].symbol;
		for (int j = 0; j < token.length(); j++) {
			if (hashTableSymbol[j] != token[j]) {
				stringMatch = false;
				break;
			}
		}
		if (stringMatch) {
			return (hashVal);
		}
		hashVal = (hashVal - 1);
		if (hashVal == -(HASHTABLE_SIZE + 1))
			hashVal = -1;

		if (startHashVal == hashVal) {
			throw FULL_SYMBOL_TABLE_EXCEPTION;
		}
	}
	hashTable[hashValToIndex(hashVal)].symbol = token;
	hashTable[hashValToIndex(hashVal)].empty = false;
	return (hashVal);
}