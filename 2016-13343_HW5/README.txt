How to use scheme interpreter
====================================================================
OS: Windows 10
Compiler: Visual Studio

====================================================================
Basic Scheme interpreter

embedded function / operators:
"+"
"-"
"*"
"eq?"
"equal?"
"="
"number?"
"symbol?"
"null?"
"CONS"
"COND"
"else"
"Car"
"Cdr"
"define"
"'" as "quote"
"#t"
"#f"

====================================================================
Resources
Memory Size: 100
Symbol Table Size: 100

====================================================================
About Data types

All commands and symbols are translated to "lower" case
 ex) "A" and "a" is same
     "Car" and "car" is same

Has two data types in numbers
  1. integer
  2. float
  ex) 1 == 2
      1.0 != 1

Floats are formatted
      1.0000 --> 1.0
      1.2310 --> 1.231

====================================================================
Input/Output

every operation uses console
> (input)

] (output)

ex)
> (define x 3)

]

> (number? x)

] #t

> (number? 'x)

] #f

====================================================================
Error Handling

error handling function
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

Full memory exception will be handled by canceling the current command then garbage collecting. After garbage collecting, if memory is enough, execute the command.
Exceptions above will be handled by canceling the current command and setting back all resources(memory, hash table pointer) as before the command was executed.

Unless memory is not enough after garbage collection, the interpreter can execute next commands.

ex)
Simple error
> (+ 'a 1)

[SchemeError] numeric operation(+, -, *, =) parameters should be numbers
==========================================================
all data including memory and hash table will set back
all data including memory and hash table has been set back
==========================================================

> 

Memory full error
> (define (prefix List n) (cond ((= n 0) '()) (else (cons (car List) (prefix (cdr List) (- n 1))))))

[SchemeError] memory is full
==========================================================
all data including memory and hash table will set back
all data including memory and hash table has been set back
==========================================================
garbage collection start
garbage collection end

>

When memory is not enough after garbage collection
> (cond ((null? '(3)) 4) ((null? '(3)) 4))

[SchemeError] memory is full
==========================================================
all data including memory and hash table will set back
all data including memory and hash table has been set back
==========================================================
garbage collection start
garbage collection end
[SchemeError] else is required in condition statement
==========================================================
all data including memory and hash table will set back
all data including memory and hash table has been set back
==========================================================
[SchemeError] memory is full
==========================================================
all data including memory and hash table will set back
all data including memory and hash table has been set back
==========================================================
garbage collection start
garbage collection end
[SchemeError] garbage collection not enough for memory allocation, all process stop

