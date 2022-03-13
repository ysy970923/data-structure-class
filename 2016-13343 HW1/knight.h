#pragma once
#include <iostream>
using namespace std;

//set the board size, as given in the textbook, it is set to 8
const int chessBoardLength = 8;

//enum defining the types of TieSolution
//Simple and Complex Solution follows the same named solutions in project pdf
enum class TieSolution
{
	SIMPLE,
	COMPLEX
};

//Defining class Knight
//private: variables: "chessBoard[][]","ktmov1[]", "ktmov2[]", "i", "j", "nexti[8]", "nextj[8]", "npos", "m"
//private: functions: "printChessBoard()", "initializeChessBoard(), "setStartingPosition(int initial_i, int initial_j)", "formSetOfPossibleNextSquares()", "testSpecialCases()",
//						"findNextSquareWithMinimumNumberOfExits_Simple()", "findNextSquareWithMinimumNumberOfExits_Complex()", "moveKnight()"
//public: functions: "knightTour(int initial_i, int initial_j, TieSolution type)"
//							====> function used to solve "knight tour problem" and get the output on the console
class Knight
{
private:
	//set the variables chessBoard and ktmov1,2 as given in the textbook
	int chessBoard[chessBoardLength][chessBoardLength] = {};
	int ktmov1[chessBoardLength] = {
			-2,
			-1,
			1,
			2,
			2,
			1,
			-1,
			-2,
	};
	int ktmov2[chessBoardLength] = {
			1,
			2,
			2,
			1,
			-1,
			-2,
			-2,
			-1,
	};

	//set the variables used for each points while touring as in the textbook(i, j, nexti[8], nextj[8], npos, m)
	int i;
	int j;
	int nexti[8];
	int nextj[8];
	int npos;
	int m;

	//function for printing the chessBoard on the console
	//it will be printed as below
	//    +----+----+----+----+----+----+----+----+
	//    |  2 | 23 |  4 | 19 | 44 | 39 | 14 | 17 |
	//    +----+----+----+----+----+----+----+----+
	//    |  5 | 20 |  1 | 40 | 15 | 18 | 45 | 38 |
	//    +----+----+----+----+----+----+----+----+
	//    | 24 |  3 | 22 | 43 | 52 | 47 | 16 | 13 |
	//    +----+----+----+----+----+----+----+----+
	//    | 21 |  6 | 61 | 48 | 41 | 50 | 37 | 46 |
	//    +----+----+----+----+----+----+----+----+
	//    | 64 | 25 | 42 | 51 | 62 | 53 | 12 | 33 |
	//    +----+----+----+----+----+----+----+----+
	//    |  7 | 28 | 63 | 60 | 49 | 34 | 55 | 36 |
	//    +----+----+----+----+----+----+----+----+
	//    | 26 | 59 | 30 |  9 | 54 | 57 | 32 | 11 |
	//    +----+----+----+----+----+----+----+----+
	//    | 29 |  8 | 27 | 58 | 31 | 10 | 35 | 56 |
	//    +----+----+----+----+----+----+----+----+
	void printChessBoard()
	{
		cout << " +";
		for (int i_index = 0; i_index < chessBoardLength; i_index++)
			cout << "----+";
		cout << endl;
		for (int i_index = 0; i_index < chessBoardLength; i_index++)
		{
			cout << " | ";
			for (int j_index = 0; j_index < chessBoardLength; j_index++)
			{
				cout << chessBoard[i_index][j_index];
				if (chessBoard[i_index][j_index] < 10)
					cout << " ";

				cout << " | ";
			}
			cout << endl;
			cout << " +";
			for (int i_index = 0; i_index < chessBoardLength; i_index++)
				cout << "----+";
			cout << endl;
		}
	}

	//function to initialize all elements in the chessBoard to 0 as given in the textbook
	void initializeChessBoard()
	{
		for (int i = 0; i < chessBoardLength; i++)
			for (int j = 0; j < chessBoardLength; j++)
				chessBoard[i][j] = 0;
	}

	//set the Starting Position with the given initial (i,j)
	void setStartingPosition(int initial_i, int initial_j)
	{
		m = 1;
		i = initial_i;
		j = initial_j;
		chessBoard[i][j] = m;
		cout << " Starting point: (" << i << ", " << j << ")" << endl;
	}

	//calculate the possible nextsquares and set nexti, nextj, npos
	void formSetOfPossibleNextSquares()
	{
		//variable "l" for how many next possible squares are calculated
		int l = 0;
		//check all next moves by adding ktmov1,2 and save only the valid ones in nexti,j and add 1 to "l"
		for (int k = 0; k < 8; k++)
		{
			int next_i = i + ktmov1[k];
			int next_j = j + ktmov2[k];
			//check if (next_i, next_j) is a valid point
			//is it inside the board?
			if ((0 <= next_i && next_i < chessBoardLength && 0 <= next_j && next_j < chessBoardLength))
				//is the point new for the knight?
				if (chessBoard[next_i][next_j] == 0)
				{
					nexti[l] = next_i;
					nextj[l] = next_j;
					l++;
				}
		}

		//set "l" to npos, total number of possible next squares
		npos = l;
	}

	//if returnVal is 0: unpredicted Error
	//if returnVal is 1: no more possible next position to move
	//if returnVal is 2: only one possible position is left
	//if returnVal is 3: more than one possible positions are left
	//      => have to find Next Square with minimum number of exits
	int testSpecialCases()
	{
		int returnVal = 0;

		if (npos == 0)
			returnVal = 1;

		if (npos == 1)
			returnVal = 2;

		if (1 < npos && npos <= 8)
			returnVal = 3;

		return returnVal;
	}

	//implements simple solution for tie problem
	//return min, min is index for the element next element to move to
	int findNextSquareWithMinimumNumberOfExits_Simple()
	{
		//calculate exits[8]
		//exits[l]: how many exits are possible for 2 more moves
		int exits[8] = {};
		for (int l = 0; l < npos; l++)
		{
			for (int k = 0; k < 8; k++)
			{
				int nextnext_i = nexti[l] + ktmov1[k];
				int nextnext_j = nextj[l] + ktmov2[k];
				//check if nextnext (i,j) is validate then add 1
				if ((0 <= nextnext_i && nextnext_i < chessBoardLength && 0 <= nextnext_j && nextnext_j < chessBoardLength))
					if (chessBoard[nextnext_i][nextnext_j] == 0)
						exits[l]++;
			}
		}

		//find the minimun element in exits[8]
		//store the minimum element's index in min
		//iterate in ktmov order and get the first one if tie happens
		//		==> only the first minimum is checked
		int temp = exits[0];
		int min = 0;
		for (int l = 1; l < npos; l++)
		{
			if (exits[l] < temp)
			{
				temp = exits[l];
				min = l;
			}
		}
		return min;
	}

	//implements complex solution for tie problem
	//return min, min is index for the element next element to move to
	int findNextSquareWithMinimumNumberOfExits_Complex()
	{
		//calculate exits[8]
		//exits[l]: how many exits are possible for 2 more moves
		int exits[8] = {};
		for (int l = 0; l < npos; l++)
		{
			for (int k = 0; k < 8; k++)
			{
				int nextnext_i = nexti[l] + ktmov1[k];
				int nextnext_j = nextj[l] + ktmov2[k];

				//check if nextnext (i,j) is valid then add 1
				if ((0 <= nextnext_i && nextnext_i < chessBoardLength && 0 <= nextnext_j && nextnext_j < chessBoardLength))
					if (chessBoard[nextnext_i][nextnext_j] == 0)
						exits[l]++;
			}
		}

		//calculate the number, and where the indexes of the ties are in the array
		//tieIndex is the array for where the ties are
		//howManyTies indicates the number of ties
		//		==> if only one minimum : howManyTies == 1
		//		==> if 3 elements are the smallest and same : howManyTies == 3
		int temp = exits[0];
		int tieIndex[8] = {};
		int howManyTies = 1;
		for (int l = 1; l < npos; l++)
		{
			//if tie, add the index in the tieIndex array and add 1 to howManyTies
			if (exits[l] == temp)
			{
				tieIndex[howManyTies] = l;
				howManyTies++;
			}
			if (exits[l] < temp)
			{
				temp = exits[l];
				tieIndex[0] = l;
				howManyTies = 1;
			}
		}

		//if howManyTies == 1, only one minimum ==> return the index as min
		if (howManyTies == 1)
		{
			return tieIndex[0];
		}

		//calculate nextexits[8]
		//nextexits[l]: total how many exits are possible for 3 more moves (one more than simple solution)
		int nextexits[8] = {};
		for (int l = 0; l < howManyTies; l++)
		{
			int next_i = nexti[tieIndex[l]];
			int next_j = nextj[tieIndex[l]];

			//calculate one more move after the next(i,j)
			for (int k = 0; k < 8; k++)
			{
				int nextnext_i = next_i + ktmov1[k];
				int nextnext_j = next_j + ktmov2[k];
				//check if nextnext(i,j) is valid
				if ((0 <= nextnext_i && nextnext_i < chessBoardLength && 0 <= nextnext_j && nextnext_j < chessBoardLength))
					if (chessBoard[nextnext_i][nextnext_j] == 0)
					{
						//calculate one more move after the nextnext(i,j)
						for (int a = 0; a < 8; a++)
						{
							int nextnextnext_i = nextnext_i + ktmov1[a];
							int nextnextnext_j = nextnext_j + ktmov2[a];
							//check if nextnextnext(i,j) is valid
							if (!(nextnextnext_i == next_i && nextnextnext_j == next_j))
								if ((0 <= nextnextnext_i && nextnextnext_i < chessBoardLength && 0 <= nextnextnext_j && nextnextnext_j < chessBoardLength))
									if (chessBoard[nextnextnext_i][nextnextnext_j] == 0)
										//next(i,j), nextnext(i,j), nextnextnext(i,j) are valid meaning this exit is valid ==> add 1
										nextexits[tieIndex[l]]++;
						}
					}
			}
		}

		//get the minimum element throughout the ties nextexits array
		//same as simple solution
		//iterate in tieIndex order which is tie's ktmov order and get the first one if tie again
		int temp2 = nextexits[tieIndex[0]];
		int min = tieIndex[0];
		for (int l = 1; l < howManyTies; l++)
		{
			if (nextexits[tieIndex[l]] < temp2)
			{
				temp2 = nextexits[tieIndex[l]];
				min = tieIndex[l];
			}
		}
		return min;
	}

	//move the Knight to the calculated next index, min
	//check the tour move number "m" on the board
	void moveKnight(int min)
	{
		i = nexti[min];
		j = nextj[min];
		chessBoard[i][j] = m;
	}

	//follows the basic flow in the textbook
	//	initialize ChessBoard --> set starting position
	//		--> loop(form set of possible next squares --> test special cases --> move knight if not failed of stopped)
	//		--> print output on the console
	void knightTour(int initial_i, int initial_j, TieSolution type)
	{
		//boolean to check failure
		bool failure = false;

		//as textbook flow
		initializeChessBoard();
		setStartingPosition(initial_i, initial_j);

		//print on the console if its simple solution or complex solution
		if (type == TieSolution::SIMPLE)
			cout << " Simple Solution" << endl;
		if (type == TieSolution::COMPLEX)
			cout << " Complex Solution" << endl;

		//now move is the second move, first happend when setting initial (i,j)
		m = 2;

		//continues the loop unless failure happend or move is done(m > 64)
		while (!failure && 2 <= m && m <= 64)
		{
			formSetOfPossibleNextSquares();

			//test which case it is
			//if returnVal is 0: unpredicted Error while calculating npos
			//		=> print error log and quit the function
			//if returnVal is 1: no more possible next position to move
			//		=>failed, check failure to true which will stop the while loop
			//if returnVal is 2: only one possible position is left
			//		=> move to the possible position
			//if returnVal is 3: more than one possible positions are left
			//      => have to find Next Square with minimum number of exits
			//if returnVal is not as above: unPredicted Error while testing special cases
			//		=> print error log and quit the function
			int testVal = testSpecialCases();
			switch (testVal)
			{
			case 0:
				cout << "unpredicted Error while calculating npos" << endl;
				return;
			case 1:
				failure = true;
				break;
			case 2:
			{
				moveKnight(0);
				m++;
				break;
			}
			case 3:
			{
				int min = 0;
				//if TieSolution type is 'SIMPLE', use findNextSquareWithMinimumNumberOfExits_Simple() to find next square
				if (type == TieSolution::SIMPLE)
					min = findNextSquareWithMinimumNumberOfExits_Simple();

				//if TieSolution type is 'COMPLEX', use findNextSquareWithMinimumNumberOfExits_Complex() to find next square
				else if (type == TieSolution::COMPLEX)
					min = findNextSquareWithMinimumNumberOfExits_Complex();

				moveKnight(min);
				//m = m+1, because one more move is made
				m++;
				break;
			}
			default:
				cout << "UnPredicted Error while testing special cases" << endl;
				break;
			}
		}

		//print on the console whether it was failure or success
		if (failure)
		{
			cout << " Failure" << endl;
		}
		else
		{
			cout << " Success" << endl;
		}
		//print the chess board
		printChessBoard();
		cout << endl;

		//knightTour function will have output on the console as below.
		//example console output when initial point is (4,4), TieSolution type is "SIMPLE"
		//Starting point : (4, 4)
		//    Simple Solution
		//    Success
		//    +----+----+----+----+----+----+----+----+
		//    | 63 | 58 | 11 | 46 | 25 | 4  | 9  | 6  |
		//    +----+----+----+----+----+----+----+----+
		//    | 12 | 47 | 64 | 57 | 10 | 7  | 26 |  3 |
		//    +----+----+----+----+----+----+----+----+
		//    | 59 | 62 | 51 | 48 | 45 | 24 | 5  | 8  |
		//    +----+----+----+----+----+----+----+----+
		//    | 50 | 13 | 56 | 61 | 52 | 27 | 2  | 23 |
		//    +----+----+----+----+----+----+----+----+
		//    | 55 | 60 | 49 | 44 | 1  | 34 | 19 | 28 |
		//    +----+----+----+----+----+----+----+----+
		//    | 14 | 43 | 38 | 53 | 40 | 31 | 22 | 33 |
		//    +----+----+----+----+----+----+----+----+
		//    | 37 | 54 | 41 | 16 | 35 | 20 | 29 | 18 |
		//    +----+----+----+----+----+----+----+----+
		//    | 42 | 15 | 36 | 39 | 30 | 17 | 32 | 21 |
		//    +----+----+----+----+----+----+----+----+
	}

public:
	//Constructor to initialize variables for knight Tour(i, j, npos, nexti, nextj, m)
	Knight()
	{
		i = 0;
		j = 0;
		npos = 0;
		m = 0;
		for (int a = 0; a < 8; a++)
		{
			nexti[a] = 0;
			nextj[a] = 0;
		}
	}

	//knightTourSimpleNComplex function
	//1. gets the initial i,j by console input
	//2. simulates the tour with both simple and complex tie break solutions
	//3. console output as below
	//example console output when initial point is (4,4) is as below
	//Starting point : (4, 4)
	//    Simple Solution
	//    Success
	//    +----+----+----+----+----+----+----+----+
	//    | 63 | 58 | 11 | 46 | 25 | 4  | 9  | 6  |
	//    +----+----+----+----+----+----+----+----+
	//    | 12 | 47 | 64 | 57 | 10 | 7  | 26 |  3 |
	//    +----+----+----+----+----+----+----+----+
	//    | 59 | 62 | 51 | 48 | 45 | 24 | 5  | 8  |
	//    +----+----+----+----+----+----+----+----+
	//    | 50 | 13 | 56 | 61 | 52 | 27 | 2  | 23 |
	//    +----+----+----+----+----+----+----+----+
	//    | 55 | 60 | 49 | 44 | 1  | 34 | 19 | 28 |
	//    +----+----+----+----+----+----+----+----+
	//    | 14 | 43 | 38 | 53 | 40 | 31 | 22 | 33 |
	//    +----+----+----+----+----+----+----+----+
	//    | 37 | 54 | 41 | 16 | 35 | 20 | 29 | 18 |
	//    +----+----+----+----+----+----+----+----+
	//    | 42 | 15 | 36 | 39 | 30 | 17 | 32 | 21 |
	//    +----+----+----+----+----+----+----+----+

	//    Starting point : (4, 4)
	//    Complex Solution
	//    Success
	//    +----+----+----+----+----+----+----+----+
	//    | 51 | 16 | 39 | 34 | 49 | 18 | 21 | 24 |
	//    +----+----+----+----+----+----+----+----+
	//    | 38 | 35 | 50 | 17 | 40 | 23 | 42 | 19 |
	//    +----+----+----+----+----+----+----+----+
	//    | 15 | 52 | 37 | 48 | 33 | 20 | 25 | 22 |
	//    +----+----+----+----+----+----+----+----+
	//    | 36 | 47 | 14 | 59 | 64 | 41 | 32 | 43 |
	//    +----+----+----+----+----+----+----+----+
	//    | 53 | 58 | 63 | 46 | 1  | 44 | 5  | 26 |
	//    +----+----+----+----+----+----+----+----+
	//    | 10 | 13 | 54 | 57 | 60 | 29 | 2  | 31 |
	//    +----+----+----+----+----+----+----+----+
	//    | 55 | 62 | 11 | 8  | 45 | 4  | 27 | 6  |
	//    +----+----+----+----+----+----+----+----+
	//    | 12 | 9  | 56 | 61 | 28 | 7  | 30 | 3  |
	//    +----+----+----+----+----+----+----+----+
	void knightTourSimpleNComplex()
	{
		//take the initial input as a console input
		int initial_i;
		int initial_j;
		//loop to check if the initial console input for i is an integer & between 0~7
		while (true)
		{
			cout << " set initial value for i(i: 0~7 integer): ";
			cin >> initial_i;
			//check whether or not input is not a integer
			//when not a integer, console error message and get the input again
			if (!cin)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " invalid i input: i has to be an integer" << endl;
			}
			//check if input is in the valid range
			else if (0 <= initial_i && initial_i < 8)
			{
				break;
			}
			//if the input is not in the range, console error message and get the input again
			else
			{
				cin.clear();
				cout << " invalid i input: i has to be between 0~7" << endl;
			}
		}

		//loop to check if the initial console input for j is an integer & between 0~7
		while (true)
		{
			cout << " set initial value for j(j: 0~7 integer): ";
			cin >> initial_j;
			//check whether or not input is not a integer
			//when not a integer, console error message and get the input again
			if (!cin)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "invalid j input: j has to be an integer" << endl;
			}
			//check if input is in the valid range
			else if (0 <= initial_j && initial_j < 8)
			{
				break;
			}
			//if the input is not in the range, console error message and get the input again
			else
			{
				cin.clear();
				cout << "invalid j input: j has to be between 0~7" << endl;
			}
		}
		cout << endl;
		//declare Knight class in header file "knight.h"
		Knight knight_simple = Knight();
		Knight knight_complex = Knight();

		//knightTour function in Knight class
		//knightTour(int initial_i, int initial_j, TieSolution type)
		//enum class TieSolution = {'SIMPLE', 'COMPLEX'}
		//TieSolution declares how to solve the tie problem

		//Simple tie Solution
		knight_simple.knightTour(initial_i, initial_j, TieSolution::SIMPLE);

		//Complex tie Solution
		knight_complex.knightTour(initial_i, initial_j, TieSolution::COMPLEX);
	}
};
