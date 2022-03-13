#pragma once
#include "exception.h"
using namespace std;

#define HASHTABLE_SIZE 30

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
	int link_of_value;
	hashTableElement() {
		empty = true;
		symbol = "";
		link_of_value = 0;
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
	int hashValToIndex(int hashVal);
	int GetHashValue(string token);
	string GetSymbol(int hashVal);
	void Print();
	SymbolTable() {
		hashTable = new hashTableElement[HASHTABLE_SIZE]{ hashTableElement()};
	}
};

//used to convert the hashValue to valid index of the table array
int SymbolTable::hashValToIndex(int hashVal) {
	return -(hashVal + 1);
}

//print hash table
void SymbolTable::Print() {
	cout << endl << "Hash Table = " << endl;
	for (int i = 0; i < HASHTABLE_SIZE; i++) {
		if (!hashTable[i].empty)
			cout << "Hash Value " << -(i + 1) << ": " << hashTable[i].symbol << endl;
	}
}

//returns the symbol at the given hashValue
string SymbolTable::GetSymbol(int hashVal) {
	return hashTable[hashValToIndex(hashVal)].symbol;
}

/*
* stores the token on the table and returns the hash value of the token
* hash function: hashVal = - (sum % HASHTABLE_SIZE)
* if hash table entry is not empty, move to next entry
* move until empty entry or match
* if empty entry, fill the entry and return the index
* if entry's symbol is match, return the index
*/
int SymbolTable::GetHashValue(string token) {
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