#include <iostream>
#include "knight.h"

//the Knight Class is in the header file "knight.h"
//the functions are declared in the Knight class
int main()
{
    //simulating the knight tour from initial positions multiple times until the user wants to stop
    while (true) {
        //Set a knight variable of Knight class to simulate the knight tour;
        Knight knight = Knight();

        //knightTourSimpleNComplex function
        //1. gets the initial i,j by console input
        //2. simulates the tour with both simple and complex tie break solutions
        //3. console output example is described in the header file
        knight.knightTourSimpleNComplex();

        //Quit mechanism
        //when the input in the console is 'q', while loop breaks, simulation stops, 
        //when the input in the console is not 'q', simulation proceeds and the user can tour the knight from a different initial position
        char quitChar;
        std::cout << "Quit? type in 'q' and press Enter..."<<endl;
        std::cout << "if not, type in any single character other than 'q' and press Enter..." << endl;
        std::cin >> quitChar;
        if (quitChar == 'q') {
            std::cin.clear();
            break;
        }
    }
    std::cout << "simulation ended";

    return 0;
}