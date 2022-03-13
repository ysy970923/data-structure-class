#pragma once

/*
* friend class With stack & Procedure
* each node element of a stack
* stores hashVal & pointer
* has pointer to before element
*/
class StackNode {
	int symbolHashVal;
	int pointer;
	StackNode* before;
	StackNode(int symbolHashVal, int pointer) {
		this->symbolHashVal = symbolHashVal;
		this->pointer = pointer;
		before = 0;
	}
	friend class Stack;
	friend class Procedure;
};

/*
* friend class Procedure
* represents the evaluation stack
* has basic stack operations: isEmpty, Pop, Push
* hash wipeStack for error occuring and needs to wipe the stack clean
*/
class Stack {
	StackNode* last;
	Stack() {
		last = 0;
	}

	bool isEmpty() {
		return last == 0;
	}
	void Pop() {
		if (!isEmpty()) {
			last = last->before;
		}
	}
	void Push(int symbolHashVal, int pointer) {
		StackNode* temp = new StackNode(symbolHashVal, pointer);
		temp->before = last;
		last = temp;
	}
	friend class Procedure;
	void wipeStack() {
		while (last != 0) {
			StackNode* temp = last;
			last = last->before;
			delete temp;
		}
	}
};
