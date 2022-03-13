#pragma once
#include "exception.h"

/*
* used for HistoryData
* when History is about memory child change, MemoryData is a structure to save information
* int child: what child is was before
* char LorR: whether left or right child has changed
*/
struct MemoryData {
	int child;
	char LorR;
	MemoryData(int child, char LorR) { this->child = child; this->LorR = LorR; }
};

/*
* used for HistoryData
* when History is about table pointer change, TableData is a structure to save information
* int pointer: what the pointer was before
*/
struct TableData { int pointer; TableData(int pointer) { this->pointer = pointer; } };

/*
* used for HistoryStackNode
* union to save either it is about memory child change or table pointer change
*/
union HistoryData {
	MemoryData memoryData;
	TableData tableData;
	HistoryData() {};
	HistoryData(MemoryData memoryData) {
		this->memoryData = memoryData;
	}
	HistoryData(TableData tableData) {
		this->tableData = tableData;
	}
};

/*
* used for HistoryStack
* int address: memory or hash table address that changed
* HistoryData historyData: data about change
* HistoryStackNode(int address, int pointer): when it is about table pointer change
* HistoryStackNode(int address, int child, char LorR): when it is about memory child change
*/
class HistoryStackNode {
	friend class HistoryStack;
	friend class Procedure;

	int address;
	HistoryData historyData;
	HistoryStackNode* before;

	// HistoryStackNode(int address, int pointer): when it is about table pointer change
	HistoryStackNode(int address, int pointer) {
		if (address >= 0)
			throw INVALID_SYMBOL_VALUE;
		this->address = address;
		this->historyData = HistoryData(TableData(pointer));
		this->before = 0;
	}

	//HistoryStackNode(int address, int child, char LorR) : when it is about memory child change
	HistoryStackNode(int address, int child, char LorR) {
		if (address <= 0)
			throw INVALID_SYMBOL_VALUE;
		this->address = address;
		this->historyData = HistoryData(MemoryData(child, LorR));
		this->before = 0;
	}
};

/*
* friend class Procedure, Memory, SymbolTable
* saves all changes in current execution
* has basic stack operations: isEmpty, Pop, Push
* hash wipeStack for every new execution
*/
class HistoryStack {
	friend class Procedure;
	friend class Memory;
	friend class SymbolTable;

	HistoryStackNode* last;
	int free_list_root;
	int size;

	HistoryStack() {
		last = 0;
		free_list_root = 0;
		size = 0;
	}

	bool isEmpty() {
		return last == 0;
	}
	void Pop() {
		if (!isEmpty()) {
			last = last->before;
		}
	}
	void Push(int address, int pointer) {
		HistoryStackNode* temp = new HistoryStackNode(address, pointer);
		temp->before = last;
		last = temp;
		size++;
	}

	void Push(int address, int child, char LorR) {
		HistoryStackNode* temp = new HistoryStackNode(address, child, LorR);
		temp->before = last;
		last = temp;
		size++;
	}
	void wipeStack() {
		while (last != 0) {
			HistoryStackNode* temp = last;
			last = last->before;
			delete temp;
		}
	}
};

