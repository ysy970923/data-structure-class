How to use scheme interpreter
OS: Windows 10
Compiler: Visual Studio

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
"quote"
"#t"
"#f"

All commands and symbols are translated to "lower" case
 ex) "A" and "a" is same
     "Car" and "car" is same

Has two data types in numbers
  1. integer
  2. float
  ex) 1 == 2
      1.0 != 1
      1.00000 == 1.0

Input/Output
every operation uses console
> (input)

] (output)

ex)
> (define (square x) (* x x))

]
> (define x 3)

]
> (define y 3.0)

]
> (square x)

] 9
> (square y)

] 9.0



Error Handling
Handled error is as below
if (error == FULL_SYMBOL_TABLE_EXCEPTION)
	print: "[ERROR] symbol table is full"
			
else if (error == FULL_MEMORY_EXCEPTION)
	print: "[ERROR] memory is full"

else if (error == NOT_VALID_CHILD)
	print: "[ERROR] not a valid binary child of memory"

else if (error == COND_STATEMENT_ERROR)
	print: "[ERROR] else is required in condition statement"

else if (error == NUMERIC_NOT_NUMBER_PARAM)
	print: "[ERROR] numeric operation(+, -, *, =) parameters should be numbers"

else if (error == SHOULD_START_WITH_LEFT_PARAM)
	print: "[ERROR] command should start with left parem"

else if (error == MISMATCH_LEFT_RIGHT_PAREN)
	print: "[ERROR] left, right paren does not match match"

print: "all data including memory and hash table has been reseted"

after error is occurred wipe all data including memory and hash table

ex)
> (+ 1 a)

[ERROR] numeric operation(+, -, *, =) parameters should be numbers
all data including memory and hash table has been reseted
>