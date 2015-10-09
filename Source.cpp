#include <iostream>           // for cout and cin 
#include <string>             // for strings and string arrays
#include <time.h>             // for random number generation 
#include <vector>             // vector library
#include <stdlib.h>           // for Sleep
#include "ConsoleColors.h"    // for console color output 
using namespace std;
using namespace eku;


// Global Variable Declarations
float pauseTime = 0.75;
// Game Colors: 
concol boardLabelColor = dark_red;
concol unsunkShip      = blue; 
concol hitOrSunkShip   = red; 
concol missGuess       = purple; 


// Function Prototypes: 

	// for printing the board to the console during player 1 ship input
		void printBoardInput(string gameBoard[11][11], string p1Display[11][11]);
	// for printing the game boards to the console during game play
		void printBoard(string p1Guesses[11][11], string p1Board[11][11],string p1Display[11][11], int p2Guesses[11][11]);
	// for printing the game boards to the console at the end of the game:  The Reveal
		void endPrintBoard(string p1Board[11][11], string p1Display[11][11], string p2Board[11][11], string p2Display[11][11], string p1Guesses[11][11]);
	// for taking in a LetterNumber combination, checking its validity, and setting the row and column accordingly: returns true is input is invalid
		bool stringInterpreter(vector<char> shipLocation, int &row1, int &col1);
	// for taking in a string input and making sure it's valid -- and then placing the ship in the correct location while updating the right game boards
		void setShipLocation(string p1Board[11][11], int ships,int shipLengths[5], int p1ShipLocations[5][4], string p1Display[11][11]);
	// for randomly assigning the CPU ship locations
		void setP2ShipLocs(string p2Board[11][11], int p2ShipLocations[5][4], int shipLengths[5]);
	// for checking if player one just sunk a ship: returns true if the last guess sunk a ship
		bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn);
	// for checking if player two sunk a ship: returns true if the last guess sunk a ship
		bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn, int p2Guesses[11][11]);
	// the entirety of player 1's turn: returns true if the game is over. 
		bool chooseTile(string p1Board[11][11], string p2Board[11][11], string p1Guesses[11][11], int p2ShipLocations[5][4], bool p2ShipStatus[5], string p1Display[11][11], int p2Guesses[11][11]);
	// for looping through the 5 ships to be placed.  This is the intro function.
		void getP1ShipLocs(string p1Board[11][11], int shipLengths[], int p1ShipLocations[5][4], string p1Display[11][11]);
	// for finding the most probable ship orientation once a ship has been found.
		void huntingMostProbable(int p2Guesses[11][11], int row, int col, int directions[4]);
	// for calculating the probability fields and randomly choosing one of the best options.
		void smart_CPU_Fire(int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int &row, int &col);
	// for taking in the CPU guess and turning it into a LetterNumber
		void displayCPUGuess(int row, int col);
	// for performing the entirety of the CPU's turn: the algorithm
		bool BSAI(string p1Board[11][11], int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int p1ShipLocations[5][4]);
	// for taking the ship locations of player 2 and setting the player 2 display board.
		void setDisplayBoard(string board[11][11], int locs[5][4]);

int main(){
	concolinit();      // activate colors
	srand(time(NULL)); // seed the random number generator

	// define empty game boards:

	// p1Board is going to store ship locations for player 1
	//		Will also store when a ship gets hit with an "X"

	string p1Board[11][11] = {
		{ " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" },
		{ "A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " }
	};

	// p1Guesses is for remembering the previous guesses of the user.
	//		will display a "O" for guesses that didn't hit a ship.
	//		will display a "X" for guesses that did.
	string p1Guesses[11][11] = {
		{ " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" },
		{ "A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " }
	};

	// p1Display will store the ship locations and the letters that 
	// correspond to the specific ship for the user.
	//		A  -  Aircraft Carrier
	//		B  -  Battleship
	//		C  -  Cruiser
	//		S  -  Submarine
	//		D  -  Destroyer
	string p1Display[11][11] = {
		{ " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" },
		{ "A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " }
	};

	// p2Board will store the locations of the CPU ships
	//		Will also store when a CPU ship gets hit with an "X"
	string p2Board[11][11] = {
		{ " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" },
		{ "A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " }
	};

	// p2Display will store the ship locations and the letters that 
	// correspond to the specific ship for the CPU.
	//		A  -  Aircraft Carrier
	//		B  -  Battleship
	//		C  -  Cruiser
	//		S  -  Submarine
	//		D  -  Destroyer
	string p2Display[11][11] = {
		{ " ", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" },
		{ "A", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "B", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "C", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "D", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "E", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "F", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "G", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "H", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "I", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " },
		{ "J", " ", " ", " ", " ", " ", " ", " ", " ", " ", " " }
	};

	// p2Guesses will store the guesses of the computer
	//		value             meaning
	//      -----             -------
	//        0			  unvisited space
	//		  1			       miss
	//		  2            Damaged Ship
	//		  3				Sunk  Ship
	int p2Guesses[11][11] = { 0 };


	int  p1ShipLocations[5][4];				 // array with player 1 ship locations [ r1 c1 r2 c2 ]
	int  p2ShipLocations[5][4];	             // array with player 2 ship locations [ r1 c1 r2 c2 ]
	int  shipLengths[5] = { 5, 4, 3, 3, 2 }; // remember the length of each ship. 

	// if true, the ship is still alive.
	bool p1ShipStatus[5] = { true, true, true, true, true };
	bool p2ShipStatus[5] = { true, true, true, true, true };
	//                         A     B     C     S     D

	// get the user's ship locations
	getP1ShipLocs(p1Board, shipLengths, p1ShipLocations,p1Display);
	//setP2ShipLocs(p1Board, p1ShipLocations, shipLengths);
	//setDisplayBoard(p1Board, p1ShipLocations);
	
	// set the CPU's ship locations
	setP2ShipLocs(p2Board, p2ShipLocations, shipLengths);

	// set the CPU display board based on the ship locations
	setDisplayBoard(p2Display, p2ShipLocations);

	// begin gameplay
	int turn = 1;  // keep track of whose turn it is
	while (true){
		if (turn == 1){
			if (chooseTile(p1Board, p2Board, p1Guesses, p2ShipLocations, p2ShipStatus,p1Display,p2Guesses)){
				cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
				cout << "|                                                     You Win!                                                        |";
				endPrintBoard(p1Board, p1Display, p2Board, p2Display,p1Guesses);
				return 0;
			} // end if (chooseTile(p1Guesses, p2Board, userGuess))
			turn = 2; // CPU's turn 

			

		} // end if (turn == 1)
		else{
			// CPU will guess 
			//		CPU_Fire will return true if its guess
			//		results in the user losing their last ship
			if (BSAI(p1Board, p2Guesses, p1ShipStatus, shipLengths, p1ShipLocations)){
				cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
				cout << "|                                                     You Lose!                                                         |";
				endPrintBoard(p1Board, p1Display, p2Board, p2Display,p1Guesses);
				return 0;
			}
			turn = 1; // user's turn 
		} // end else: if( turn == 1 )
	} // end while(true)
} // end int main()

// Function Definitions:
	void printBoardInput(string gameBoard[11][11], string p1Display[11][11]){
	// loop through each row of the grids
	// and each column and display the current
	// game board with the updated ship locations.

	for (int r = 0; r < 11; r++){
		cout << "\t\t\t\t        ";
		for (int c = 0; c < 11; c++){
			if (r == 0){
				cout << boardLabelColor << gameBoard[r][c] << black << "   ";
			} // end if (r == 0)
			else{
				if (gameBoard[r][c] == "S"){
					cout << unsunkShip << p1Display[r][c] << black << " | ";
				} // end if (gameBoard[r][c] == "S")
				else if (gameBoard[r][c] == " "){
					cout << gameBoard[r][c] << " | ";
				} // end else if (gameBoard[r][c] == " ")
				else{
					cout << boardLabelColor << gameBoard[r][c] << black << " | ";
				}// end else
			} // end else: if (r==0)
		} // end for (int c = 0; c < 11; c++)
		cout << "\n \t\t\t\t          -----------------------------------------\n";
	} // end for (int r = 0; r < 11; r++)
} // end void printBoardInput(string gameBoard[11][11], string p1Display[11][11])
	void printBoard(string p1Guesses[11][11], string p1Board[11][11], string p1Display[11][11], int p2Guesses[11][11]){
	// a whole lot of annoying nested for loops that display the game boards in a pretty way.
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "|" << boardLabelColor << "                        YOUR GUESSES                                      YOUR SHIPS/CPU GUESSES                     " << black << "|\n";
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "|                                                                                                                     |\n";
	for (int r = 0; r < 11; r++){
		cout << "|\t";
		for (int c = 0; c < 11; c++){
			if (r == 0){
				cout << boardLabelColor << p1Guesses[r][c] << black << "   ";
			} // end if (r == 0)
			else{
				if ( (p1Guesses[r][c] == "X" || p1Guesses[r][c] == "A" || p1Guesses[r][c] == "B" || p1Guesses[r][c] == "C" || p1Guesses[r][c] == "S" || p1Guesses[r][c] == "D") && c > 0){
					cout << hitOrSunkShip << p1Guesses[r][c] << black << " | ";
				} // end if ( (p1Guesses[r][c] == "X" || p1Guesses[r][c] == "A" || p1Guesses[r][c] == "B" || p1Guesses[r][c] == "C" || p1Guesses[r][c] == "S" || p1Guesses[r][c] == "D") && c > 0)
				else if (p1Guesses[r][c] == "O"){
					cout << missGuess << p1Guesses[r][c] << black << " | ";
				} // end else if (p1Guesses[r][c] == "O")
				else if (p1Guesses[r][c] == " "){
					cout << p1Guesses[r][c] << " | ";
				} // end else if (p1Guesses[r][c] == " ")
				else{
					cout << boardLabelColor << p1Guesses[r][c] << black << " | ";
				} // end else
			} // end else: if (r == 0)
		} // end for (int c = 0; c < 11; c++)
		cout << "\t\t";
		for (int c = 0; c < 11; c++){
			if (r == 0){
				cout << boardLabelColor << p1Board[r][c] << black << "   ";
			} // end if (r == 0)
			else{
				if (p1Board[r][c] == "S"){
					cout << unsunkShip << p1Display[r][c] << black << " | ";
				} // end if (p1Board[r][c] == "S")
				else if (p1Board[r][c] == "X"){
					if (p2Guesses[r][c] == 2 || p2Guesses[r][c] == 3){
						cout << hitOrSunkShip << p1Display[r][c] << black << " | ";
					} // end if (p2Guesses[r][c] == 2 || p2Guesses[r][c] == 3)
					else{
						cout << blue << p1Display[r][c] << black << " | ";
					} // end else: if (p2Guesses[r][c] == 2 || p2Guesses[r][c] == 3)
				} // end else if (p1Board[r][c] == "X")
				else if (p1Board[r][c] == "O"){
					cout << missGuess << p1Board[r][c] << black << " | ";
				} // end else if (p1Board[r][c] == "O")
				else if (p1Board[r][c] == " "){
					cout << p1Board[r][c] << black << " | ";
				} // end else if (p1Board[r][c] == " ")
				else{
					cout << boardLabelColor << p1Board[r][c] << black << " | ";
				} // end else
			} // end else: if (r == 0)
		} // end for (int c = 0; c < 11; c++)
		if (r == 0){
			cout << "         |";
		} // end if (r == 0)
		else{
			cout << "          |";
		} // end else: if (r == 0)
		cout << "\n| \t  -----------------------------------------  \t\t  -----------------------------------------           |\n";
	} // end for (int r = 0; r < 11; r++)
	cout << "|                                                                                                                     |";
	cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
} // end void printBoard(string p1Guesses[11][11], string p1Board[11][11], string p1Display[11][11], int p2Guesses[11][11])
	bool stringInterpreter(vector<char> shipLocation, int &row1, int &col1){

	if (shipLocation.size() < 2){
		cout << boardLabelColor <<  "   Mistakes were made. Try Again.\n   Inputs must be a Letter(A - J) followed by a Number(1 - 10).\n\n" << black;
		return true;
	} // end if (shipLocation.size() < 2)

	// interpret first letter
	if (shipLocation[0] == 'A' || shipLocation[0] == 'a'){
		row1 = 1;
	} // end if (shipLocation[0] == 'A' || shipLocation[0] == 'a')
	else if (shipLocation[0] == 'B' || shipLocation[0] == 'b'){
		row1 = 2;
	} // end else if (shipLocation[0] == 'B' || shipLocation[0] == 'b')
	else if (shipLocation[0] == 'C' || shipLocation[0] == 'c'){
		row1 = 3;
	}  // end else if (shipLocation[0] == 'C' || shipLocation[0] == 'c')
	else if (shipLocation[0] == 'D' || shipLocation[0] == 'd'){
		row1 = 4;
	} // end else if (shipLocation[0] == 'D' || shipLocation[0] == 'd')
	else if (shipLocation[0] == 'E' || shipLocation[0] == 'e'){
		row1 = 5;
	} // end else if (shipLocation[0] == 'E' || shipLocation[0] == 'e')
	else if (shipLocation[0] == 'F' || shipLocation[0] == 'f'){
		row1 = 6;
	} // end else if (shipLocation[0] == 'F' || shipLocation[0] == 'f')
	else if (shipLocation[0] == 'G' || shipLocation[0] == 'g'){
		row1 = 7;
	} // end else if (shipLocation[0] == 'G' || shipLocation[0] == 'g')
	else if (shipLocation[0] == 'H' || shipLocation[0] == 'h'){
		row1 = 8;
	} // end else if (shipLocation[0] == 'H' || shipLocation[0] == 'h')
	else if (shipLocation[0] == 'I' || shipLocation[0] == 'i'){
		row1 = 9;
	} // end else if (shipLocation[0] == 'I' || shipLocation[0] == 'i')
	else if (shipLocation[0] == 'J' || shipLocation[0] == 'j'){
		row1 = 10;
	} // end else if (shipLocation[0] == 'J' || shipLocation[0] == 'j')
	else{
		cout << hitOrSunkShip << "   Mistakes were made. Try Again.\n   Inputs must be a Letter(A-J) followed by a Number(1-10).\n\n" << black;
		return true;
	} // end else

	// interpret the first number
	if (shipLocation.size() < 3){
		if (shipLocation[1] == '1'){
			col1 = 1;
		} // end if (shipLocation[1] == '1')
		else if (shipLocation[1] == '2'){
			col1 = 2;
		} // end else if (shipLocation[1] == '2')
		else if (shipLocation[1] == '3'){
			col1 = 3;
		} // end else if (shipLocation[1] == '3')
		else if (shipLocation[1] == '4'){
			col1 = 4;
		} // end else if (shipLocation[1] == '4')
		else if (shipLocation[1] == '5'){
			col1 = 5;
		} // end else if (shipLocation[1] == '5')
		else if (shipLocation[1] == '6'){
			col1 = 6;
		} // end else if (shipLocation[1] == '6')
		else if (shipLocation[1] == '7'){
			col1 = 7;
		} // end else if (shipLocation[1] == '7')
		else if (shipLocation[1] == '8'){
			col1 = 8;
		} // end else if (shipLocation[1] == '8')
		else if (shipLocation[1] == '9'){
			col1 = 9;
		} // end else if (shipLocation[1] == '9')
		else{
			cout << "   Mistakes were made.Try Again.\n   Inputs must be a Letter(A - J) followed by a Number(1 - 10).\n\n";
			return true;
		} // end else
	} // end if (shipLocation.size() < 3)
	else if (shipLocation.size() == 3){
		if (shipLocation[1] == '1' && shipLocation[2] == '0'){
			col1 = 10;
		} // end if (shipLocation[1] == '1' && shipLocation[2] == '0')
		else{
			cout << "   Mistakes were made.Try Again.\n   Inputs must be a Letter(A - J) followed by a Number(1 - 10).\n\n";
			return true;
		} // end else: if (shipLocation[1] == '1' && shipLocation[2] == '0')
	} // end else if (shipLocation.size() == 3)
	else{
		cout << "   Mistakes were made.Try Again.\n   Inputs must be a Letter(A - J) followed by a Number(1 - 10).\n\n";
		return true;
	} // end else
	return false;
} // end bool stringInterpreter(vector<char> shipLocation, int &row1, int &col1)
	void setShipLocation(string p1Board[11][11], int ships, int shipLengths[5], int p1ShipLocations[5][4], string p1Display[11][11]){

	string shipLocation;   // string to store user input
	int row1, col1;        // stores first endpoint of ship
	int row2, col2;		   // stores second endpoint of ship
	int temp;			   // for swapping
	vector<char> loc1;     // first half of the user input string
	vector<char> loc2;     // second half of the user input string
	bool secondString;     // flag that flips when the "-" is found
	bool shipsIntersected; // flag that flips when ships intersect 
	static int p1Ships = 0;// keep track of how many ships have been placed.
	// you will be sent to the top of this loop via a continue
	// if your input is invalid.  The end of the loop has a break 
	// to get you out, if you get through all the error checks
	while (true) {
		shipLocation = "       ";
		
		cout << "Please place your " << shipLengths[ships] << "-tile ship: " << boardLabelColor;
		cin >> shipLocation;
		cout << black;
		
		secondString = false;

		// this for loop changes shipLocation and creates
		// two strings loc1 and loc2 that correspond to the
		// beginning and endpoints of the ship to be placed. 
		for (int idx = 0; idx < shipLocation.size(); idx++){
			if (!secondString){
				if (shipLocation[idx] == '-'){
					secondString = true;
				} // end if (shipLocation[idx] == '-')
				else{
					loc1.push_back(shipLocation[idx]);
				} // end else: if (shipLocation[idx] == '-')
			} // end if (!secondString)
			else{
				loc2.push_back(shipLocation[idx]);
			} // end else: if (!secondString)
		} // end for (int idx = 0; idx < shipLocation.size(); idx++)

		// make sure the input had a dash
		if (!secondString){
			cout << hitOrSunkShip << "   Input must be LetterNumber-LetterNumber.\n" << black;
			loc1.clear(); // if it isn't, clear the vectors
			loc2.clear();
			continue;
		} // end if (!secondString)
		// make sure that the first combo is a valid input
		if (stringInterpreter(loc1, row1, col1)){
			loc1.clear();  // if it isn't, clear the vectors
			loc2.clear();
			continue;
		} // end if (stringInterpreter(loc1, row1, col1))
		// make sure that the second combo is a valid input 
		if (stringInterpreter(loc2, row2, col2)){
			loc1.clear(); // if it isn't, clear the vectors
			loc2.clear();
			continue;
		} // end if (stringInterpreter(loc2, row2, col2))

		// if we've gotten this far, it means that both
		// input locations are correct.  now make sure that
		// the size of the input is correct

		// one dimensional ship, so either the row numbers
		// or the column numbers must be equivalent.

		if (row1 != row2 && col1 != col2){
			cout << hitOrSunkShip << "   The ships are one dimensional. Try again.\n" << black;
			loc1.clear(); // if it isn't, clear the vectors
			loc2.clear();
			continue;
		} // end if (row1 != row2 && col1 != col2)


		// check the size of the ship. 

		// the ship is being placed horizontally
		if (row1 == row2){
			// put the column inputs in order from least to greatest
			if (col2 < col1){
				temp = col1;
				col1 = col2;
				col2 = temp;
			} // end if (col2 < col1)
			if ((col2 - col1 + 1) != shipLengths[ships]){
				cout << hitOrSunkShip << "   Your input was not the correct size. Try again.\n" << black;
				loc1.clear(); // if it isn't, clear the vectors
				loc2.clear();
				continue;
			} // end if ((col2 - col1 + 1) != shipLengths[ships])
		} // end if (row1 == row2)
		// the ship is being placed vertically
		else{
			// put the row inputs in order from least to greatest
			if (row2 < row1){
				temp = row1;
				row1 = row2;
				row2 = temp;
			} // end if (row2 < row1)
			if ((row2 - row1 + 1) != shipLengths[ships]){
				cout << hitOrSunkShip << "   Your input was not the correct size. Try again.\n" << black;
				loc1.clear(); // if it isn't, clear the vectors
				loc2.clear();
				continue;
			} // end if ((row2 - row1 + 1) != shipLengths[ships])
		} // end else: if (row1 == row2)

		// cool, you finally input locations 
		// that are one dimensional and the
		// correct size. 
		//---------------------------------
		// check to see if there is already
		// a ship where you're trying to go.
		shipsIntersected = false;
		for (int r = row1; r <= row2; r++){
			for (int c = col1; c <= col2; c++){
				if (p1Board[r][c] == "S"){
					shipsIntersected = true;
					cout << hitOrSunkShip << "   Ships may not intersect. Try Again.\n" << black;
					loc1.clear(); // if it isn't, clear the vectors
					loc2.clear();
					break;
				} // end if (p1Board[r][c] == "S")
			} // end for (int c = col1; c <= col2; c++)
			if (shipsIntersected){
				break;
			} // end if (shipsIntersected)
		} // end for (int r = row1; r <= row2; r++)
		if (shipsIntersected){
			continue;
		} // end if (shipsIntersected)
		break;
	} // end while(true)

	// finally place the F%^&*^G ships.
	for (int r = row1; r <= row2; r++){
		for (int c = col1; c <= col2; c++){
			p1Board[r][c] = "S";
		} // end for (int c = col1; c <= col2; c++)
	} // end for (int r = row1; r <= row2; r++)

	// update player 1's display board 
	string shipNames[5] = { "A", "B", "C", "S","D" };
	for (int r = row1; r <= row2; r++){
		for (int c = col1; c <= col2; c++){
			p1Display[r][c] = shipNames[p1Ships];
		} // end for (int c = col1; c <= col2; c++)
	} // end for (int r = row1; r <= row2; r++)

	// update p1 ship locations matrix
	int shipLocs[4] = { row1, col1, row2, col2 };
	if (p1Ships < 5){
		for (int i = 0; i < 4; i++){
			p1ShipLocations[p1Ships][i] = shipLocs[i];
		} // for (int i = 0; i < 4; i++)
		p1Ships++;
	} // if (p1Ships < 5)

}
	void setP2ShipLocs(string p2Board[11][11], int p2ShipLocations[5][4], int shipLengths[5]){

	// randomly place the cpu's ships

	int  max[] = { 6, 5, 4, 4, 3 }; // determines the board buffer requihitOrSunkShip for each ship
	bool isPlaced = false;          // flag to determine if a ship can be placed there 
	int  row = 0;                   // the row guess location to begin placing the ships
	int  col = 0;                   // the col guess location to begin placing the ships. 

	// whichWay will be the orientation of the ship being placed.
	// randomly horizontal or vertical
	enum direction{
		horizontal,
		vertical
	};
	direction whichWay;

	for (int ships = 0; ships < 5; ships++){
		while (!isPlaced){
			isPlaced = true;
			whichWay = (rand() % 2 == 0) ? horizontal : vertical;
			if (whichWay == horizontal){
				col = 1 + rand() % max[ships];
				row = 1 + rand() % 10;
				for (int c = col; c < col + (11 - max[ships]); c++){
					if (p2Board[row][c] == "S"){
						isPlaced = false; break;
					} // end if (p2Board[row][c] == "S")
				} // end for (int c = col; c < col + (11 - maxGuess); c++)
			} // end if (whichWay == horizontal)
			else{
				row = 1 + rand() % max[ships];
				col = 1 + rand() % 10;
				for (int r = row; r < row + (11 - max[ships]); r++){
					if (p2Board[r][col] == "S"){
						isPlaced = false; break;
					} // end if (gameBoard[r][col] == "S")
				} // end for (int r = row; r < row + (11 - maxGuess); r++)
			} // end else: if (whichWay == horizontal)
		} // end which(!isPlaced)
		isPlaced = false;
		if (whichWay == horizontal){
			for (int c = 0; c < max[ships] - 1; c++){
				p2Board[row][col + c] = 'S';
			} // end for (int c = 0; c < maxGuess - 1; c++)
			p2ShipLocations[ships][0] = row;
			p2ShipLocations[ships][1] = col;
			p2ShipLocations[ships][2] = row;
			p2ShipLocations[ships][3] = col + shipLengths[ships] - 1;
		} // end if (whichWay == horizontal)
		else{
			for (int r = 0; r < max[ships] - 1; r++){
				p2Board[row + r][col] = 'S';
			} // end for (int r = 0; r < maxGuess - 1; r++)
			p2ShipLocations[ships][0] = row;
			p2ShipLocations[ships][1] = col;
			p2ShipLocations[ships][2] = row + shipLengths[ships] - 1;
			p2ShipLocations[ships][3] = col;
		} // end else: if (whichWay == horizontal)
	} // end for (int ships = 0; ships < 5; ships++)
} // end 
	bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn, string p1Guesses[11][11]){
	// determine if the ship is sunk during player 1's turn

	bool shipSunk = true;
	// for each ship
	//		- check if it's still alive
	//		- if it is, check each location it's in for an S
	//		- if there is an S, the ship isn't sunk. move on.
	for (int ship = 0; ship < 5; ship++){
		if (shipStatus[ship]){
			shipSunk = true;
			for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++){
				for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++){
					if (board[r][c] == "S"){
						shipSunk = false; break;
					} // end if (board[r][c] == "S")
				} // end for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++)
				if (!shipSunk){
					break;
				} // end if (!shipSunk)
			} // end for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++)
			if (shipSunk){
				// if shipSunk, then the current ship has just been sunk 
				// update its status and change p1Guesses to represent
				// the ship that has just been sunk.
				shipStatus[ship] = false;
				string shipNames[5] = { "A", "B", "C", "S", "D" };
				for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++){
					for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++){
						p1Guesses[r][c] = shipNames[ship];
					} // end for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++)
				} // end for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++)
				if (turn == 1){
					cout << "   You sunk the computer's ";
				} // end if (turn == 1)
				else{
					cout << "   The CPU sank your ";
				} // end else: if (turn == 1)
				switch (ship){
				case 0:
					cout << "Aircraft Carrier!\n";
					break;
				case 1:
					cout << "Battleship!\n";
					break;
				case 2:
					cout << "Cruiser!\n";
					break;
				case 3:
					cout << "Submarine!\n";
					break;
				case 4:
					cout << "Destroyer!\n";
					break;
				} // end switch (ship)
				Sleep(pauseTime * 1000);
				return true;
			} // end if (shipSunk)
		} // end if (shipStatus[ship])
	} // end for (int ship = 0; ship < 5; ship++)
	return false;
} // end bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn, string p1Guesses[11][11])
	bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn, int p2Guesses[11][11]){
	// determine if CPU's last guess sunk a ship. 
	bool shipSunk = true;
	for (int ship = 0; ship < 5; ship++){
		if (shipStatus[ship]){
			shipSunk = true;
			for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++){
				for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++){
					if (board[r][c] == "S"){
						shipSunk = false; break;
					} // end if board 
				} // end for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++)
				if (!shipSunk){
					break;
				} // end if (!shipSunk)
			} // end for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++)
			if (shipSunk){
				// if shipsunk is true, the previous guess sunk the ship
				// update p2Guesses to indicate the sunk ship
				for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++){
					for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++){
						p2Guesses[r][c] = 3;
					} // end for (int c = shipLocs[ship][1]; c <= shipLocs[ship][3]; c++)
				} // end for (int r = shipLocs[ship][0]; r <= shipLocs[ship][2]; r++)
				// update the ship's status
				shipStatus[ship] = false;
			
				if (turn == 1){
					cout << "   You sunk the computer's ";
				} // end if (turn == 1)
				else{
					cout << "   The CPU sank your ";
				} // end else: if (turn == 1)
				switch (ship){
				case 0:
					cout << "Aircraft Carrier!\n";
					break;
				case 1:
					cout << "Battleship!\n";
					break;
				case 2:
					cout << "Cruiser!\n";
					break;
				case 3:
					cout << "Submarine!\n";
					break;
				case 4:
					cout << "Destroyer!\n";
					break;
				}
				Sleep(pauseTime * 1000);
				return true;
			} // end switch (ship)
		} // end if (shipStatus[ship]){
	} // end for (int ship = 0; ship < 5; ship++)
	return false;
} // end bool isShipSunk(int shipLocs[5][4], bool shipStatus[5], string board[11][11], int turn, int p2Guesses[11][11])
	bool isGameOver(bool shipStatus[5]){
	// check each ships status.
	// if a ship is alive, return false -- else return true; 
	for (int i = 0; i < 5; i++){
		if (shipStatus[i])
			return false;
	}
	return true;
} // end bool isGameOver(bool shipStatus[5])
	bool chooseTile(string p1Board[11][11], string p2Board[11][11], string p1Guesses[11][11], int p2ShipLocations[5][4], bool p2ShipStatus[5], string p1Display[11][11], int p2Guesses[11][11]){
	
	// handle user's turn.
	// when the user sinks a ship, check to see if the ship was the last ship.
	// if the last ship was sunk, return true to alert main that the game is over.
	
	vector<char> userGuess; // vector to hold user input
	string userInput;   // string for user guess
	int row;            // user's row guess
	int col;            // user's col guess
	bool tryAgain;      // if their input is invalid -- true

	// print the game board
	printBoard(p1Guesses, p1Board, p1Display, p2Guesses); 
	do{
		cout << "   Where are we aiming our cannons: " << boardLabelColor;
		cin >> userInput;
		cout << black;
		// this wouldn't have to be a char vector,
		// but the input for placing the user's ships does
		// so string interpreter was written to handle char vectors
		// ergo, char vector. 
		for (int i = 0; i < userInput.size(); i++){
			userGuess.push_back(userInput[i]);
		} // end for (int i = 0; i < userInput.size(); i++)
		tryAgain = stringInterpreter(userGuess, row, col);
		if (tryAgain){
			userGuess.clear();
		} // end if (tryAgain)
	} while (tryAgain);

	if (p2Board[row][col] == "S"){
		cout << "   You hit the CPU's ship!\n";
		Sleep(pauseTime * 1000);
		p1Guesses[row][col] = "X";
		p2Board[row][col] = "X";
		if (isShipSunk(p2ShipLocations, p2ShipStatus, p2Board, 1, p1Guesses)){
			if (isGameOver(p2ShipStatus)){
				return true;
			} // end if (isGameOver(p2ShipStatus))
		} // end if (isShipSunk(p2ShipLocations, p2ShipStatus, p2Board, 1, p1Guesses))
	} // end if (p2Board[row][col] == "S")
	else if (p1Guesses[row][col] == "X" || p1Guesses[row][col] == "O" || p1Guesses[row][col] == "A" || p1Guesses[row][col] == "B" || p1Guesses[row][col] == "C" || p1Guesses[row][col] == "S" || p1Guesses[row][col] == "D"){
		cout << "   You've already guessed this location.  Try Again.\n";
		// if they've already guessed -- recursion -- guess again
		if (chooseTile(p1Board, p2Board, p1Guesses, p2ShipLocations, p2ShipStatus, p1Display, p2Guesses)){
			return true;
		} // end if (chooseTile(p1Board, p2Board, p1Guesses, p2ShipLocations, p2ShipStatus, p1Display, p2Guesses))
	} // end else if (p1Guesses[row][col] == "X" || p1Guesses[row][col] == "O" || p1Guesses[row][col] == "A" || p1Guesses[row][col] == "B" || p1Guesses[row][col] == "C" || p1Guesses[row][col] == "S" || p1Guesses[row][col] == "D")
	else{
		cout << "   you missed :( \n";
		Sleep(pauseTime * 1000);
		p1Guesses[row][col] = "O";
	} // end else 
	return false;
} // end bool chooseTile(string p1Board[11][11], string p2Board[11][11], string p1Guesses[11][11], int p2ShipLocations[5][4], bool p2ShipStatus[5], string p1Display[11][11], int p2Guesses[11][11])
	void getP1ShipLocs(string p1Board[11][11], int shipLengths[], int p1ShipLocations[5][4], string p1Display[11][11]){
	// getP1ShipLocs loops 5 times for each ship to be placed.
	// takes in input from the user and translates it to a ship location
	for (int ships = 0; ships < 5; ships++){
		cout << black << "\n-----------------------------------------------------------------------------------------------------------------------\n";
		cout << boardLabelColor << "                                                     WELCOME TO BATTLESHIP \n" << black;
		cout << black << "-----------------------------------------------------------------------------------------------------------------------\n\n" << black;
		printBoardInput(p1Board,p1Display);
		setShipLocation(p1Board, ships, shipLengths, p1ShipLocations,p1Display);
		system("cls");
	} // end for (int ships = 0; ships < 5; ships++)
} // end void getP1ShipLocs(string p1Board[11][11], int shipLengths[], int p1ShipLocations[5][4], string p1Display[11][11])
	void huntingMostProbable(int p2Guesses[11][11], int row, int col, int directions[4]){

	// determine the most probable cardinal direction to guess once
	// a ship has been found.

	// it's assumed that the most probable direction will be the direction
	// that has the most open space before an obstruction. 

	int dir[4] = { 1, 1, 1, 1 };

	// check North
	while ((p2Guesses[row - dir[0]][col] == 0 || p2Guesses[row - dir[0]][col] == 2) && row - dir[0] >= 1){
		dir[0]++;
	}// end while ((p2Guesses[row - dir[0]][col] == 0 || p2Guesses[row - dir[0]][col] == 2) && row - dir[0] >= 1)

	// check South
	while ((p2Guesses[row + dir[1]][col] == 0 || p2Guesses[row + dir[1]][col] == 2) && row + dir[1] <= 10){
		dir[1]++;
	}// end while ((p2Guesses[row - dir[1]][col] == 0 || p2Guesses[row - dir[1]][col] == 2) && row - dir[1] >= 1)

	// check East
	while ((p2Guesses[row][col + dir[2]] == 0 || p2Guesses[row][col + dir[2]] == 2) && col + dir[2] <= 10){
		dir[2]++;
	}// end while ((p2Guesses[row][col + dir[2]] == 0 || p2Guesses[row][col + dir[2]] == 2) && col + dir[2] <= 10)

	// check West
	while ((p2Guesses[row][col + dir[3]] == 0 || p2Guesses[row][col + dir[3]] == 2) && col + dir[3] >= 1){
		dir[3]++;
	}// end while ((p2Guesses[row][col + dir[3]] == 0 || p2Guesses[row][col + dir[3]] == 2) && col + dir[3] <= 10)

	// now rearrange the directions array into the order
	// of most probable to least probable
	int whichWay = 0;
	int maxValue = 0;
	int temp = 0;
	for (int j = 0; j < 4; j++){
		maxValue = 0;
		for (int i = j; i < 4; i++){
			if (dir[i] > maxValue){
				maxValue = dir[i];
				whichWay = i;
			} // end if (dir[i] > maxValue)
		} // end for (int i = j; i < 4; i++)
		// swap directions matrix
		temp = directions[j];
		directions[j] = directions[whichWay];
		directions[whichWay] = temp;
		// swap idx's 
		temp = dir[j];
		dir[j] = dir[whichWay];
		dir[whichWay] = temp;
	} // end for (int j = 0; j < 4; j++)
} // end void huntingMostProbable(int p2Guesses[11][11], int row, int col, int directions[4])
	void smart_CPU_Fire(int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int &row, int &col){

	// we're going to calculate the probability fields for the most likely ship locations
	// this is done by calculating a superposition of all the possible ship orientations
	// of the remaining ships.
	// to do this, start with ship one and increase the probability in every spot where that
	// ship can be placed horizontally.  Giving preference to the center of the ship. 
	// do this again for each ship.
	// now do the same thing vertically.
	// find all the positions with the highest probability and randomly choose one to guess.
	int probabilityField[11][11] = { 0 };
	bool possible;


	// loop through each ship. 
	for (int ship = 0; ship < 5; ship++){
		if (p1ShipStatus[ship]){
			// loop through each row.
			for (int i = 1; i <= 10; i++){
				for (int j = 1; j <= 10 - (shipLengths[ship] - 1); j++){
					possible = true;
					for (int idx = 0; idx < shipLengths[ship]; idx++){
						if (p2Guesses[i][j + idx] == 1 || p2Guesses[i][j + idx] == 3){
							possible = false; break;
						}
					}
					if (possible){
						for (int idx = 0; idx < shipLengths[ship] / 2; idx++){
							probabilityField[i][j + idx] += idx + 1;
							probabilityField[i][j + shipLengths[ship] - 1 - idx] += idx + 1;
						} // end for (int idx = 0; idx < shipLengths[ship]; idx++)
						if (shipLengths[ship] % 2 == 1){
							probabilityField[i][j + shipLengths[ship] / 2] += shipLengths[ship] / 2 + 1;
						}
					} // end if(possible)
				} // end for (int c = 1; c <= 10 - (shipLengths[ship] - 1); c++)
			} // end for (int r = 1; r <= 10; r++)
			// break; // uncomment to  only worry about the largest possible ship
		} // end if (p1ShipStatus[ship])
	} // end for (int ship = 0; ship < 5; ship++)


	// loop through each ship. 
	for (int ship = 0; ship < 5; ship++){
		if (p1ShipStatus[ship]){
			// loop through each row.
			for (int i = 1; i <= 10 - (shipLengths[ship] - 1); i++){
				for (int j = 1; j <= 10; j++){
					possible = true;
					for (int idx = 0; idx < shipLengths[ship]; idx++){
						if (p2Guesses[i + idx][j] == 1 || p2Guesses[i + idx][j] == 3){
							possible = false; break;
						} // end if (p2Guesses[i + idx][j] == 1 || p2Guesses[i + idx][j] == 3)
					} // end for (int idx = 0; idx < shipLengths[ship]; idx++)
					if (possible){
						// give the center of the possible ship location priority
						for (int idx = 0; idx < shipLengths[ship] / 2; idx++){
							probabilityField[i + idx][j] += idx + 1;
							probabilityField[i + shipLengths[ship] - 1 - idx][j] += idx + 1;
						} // end for (int idx = 0; idx < shipLengths[ship]; idx++)
						if (shipLengths[ship] % 2 == 1){
							probabilityField[i + shipLengths[ship] / 2][j] += shipLengths[ship] / 2 + 1;
						} // end if (shipLengths[ship] % 2 == 1)
					} // end if(possible)				
				} // end for (int c = 1; c <= 10 - (shipLengths[ship] - 1); c++)
			} // end for (int r = 1; r <= 10; r++)
			// break; // uncomment to only worry about the largest possible ship
		} // end if (p1ShipStatus[ship])
	} // end for (int ship = 0; ship < 5; ship++)


	// find the max value and randomly select one if there are more than one.
	vector<int> i_locs;
	vector<int> j_locs;
	// find the highest value 
	int maxValue = 0;
	for (int i = 1; i < 11; i++){
		for (int j = 1; j < 11; j++){
			if (probabilityField[i][j] > maxValue && p2Guesses[i][j] != 1 && p2Guesses[i][j] != 2 && p2Guesses[i][j] != 3){
				maxValue = probabilityField[i][j];
			} // end if (probabilityField[i][j] > maxValue && p2Guesses[i][j] != 1 && p2Guesses[i][j] != 2 && p2Guesses[i][j] != 3)
		} // end for (int j = 1; j < 11; j++)
	} // end for (int i = 1; i < 11; i++)

	// find all the spots that have the highest value
	for (int i = 1; i < 11; i++){
		for (int j = 1; j < 11; j++){
			if (probabilityField[i][j] == maxValue && p2Guesses[i][j] == 0){
				i_locs.push_back(i);
				j_locs.push_back(j);
			} // end if (probabilityField[i][j] == maxValue && p2Guesses[i][j] == 0)
		} // end for (int j = 1; j < 11; j++)
	}// end for (int i = 1; i < 11; i++)
	// randomly select one 
	int randNum = rand() % i_locs.size();
	row = i_locs.at(randNum);
	col = j_locs.at(randNum);

} // end void smart_CPU_Fire(int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int &row, int &col)
	void displayCPUGuess(int row, int col){
	// interpret the guess into something the user will understand
	char letters[] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "   The CPU guessed: " << boardLabelColor << letters[row] << col << black << "\n";
} // end void displayCPUGuess(int row, int col)
	bool BSAI(string p1Board[11][11], int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int p1ShipLocations[5][4]){
		
	// this is the algorithm for the CPU
	//
	// static vars are used so that the CPU can remember it's state
	// between turns.  
	//
	// Algorithm:
	//	1. intelligently guess the most probable location of an emey ship
	//     based on the probability fields calculated by smart_CPU_Fire
	//  2. Guess that Location
	//		a. if miss, repeat steps 1 and 2
	//		b. if hit, continue on to step 3
	//	3. Intelligently guess at the orientation of the ship based on
	//	   the number of empty board slots in each direction.
	//		a. if miss, guess the next location until hit
	//		b. if hit, move on to step 4
	//	4. With two hits in a row, continue guessing that direction
	//	   until the edge of the board, encountering a sunk ship,
	//     finding a previously guessed miss, or missing a ship.
	//	5.   directions and go until you sink a ship or encounter
	//	   one of the cases mention in (4).
	//		a. if sink -- check if game is over -- if not go back to 1.
	//		b. if not -- you encountehitOrSunkShip multiple touching ships.
	//			i. go back to 3 and continue until sunk ship
	//			ii. with first ship sunk, move on to the other ships you just found.

	static bool Hunting = true;				   // state flag for remembering if the cpu is hunting for a ship.
	static bool directionFound = false;		   // state flag for remembering if the cpu has found the orientation of the ship
	static bool bothDirections = false;		   // state flag for remembering if the cpu has found the orientation of the ship and checked both sides.
	static int directions[4] = { 1, 2, 3, 4 }; // for guessing the direction once a ship has been found.
	static int row = 0, col = 0;			   // for remembering the last cpu guess
	static int idx = 0;                        // for remembering how far away from the origin the cpu is now guessing.
	static bool findMostProbable = true;       // for determining if you should calculate the most probable neighborhood shot
	static int whichWay = 0;			       // for keeping track of which neighborhoods have been shot at it. 
	static int dRow = 0, dCol = 0;             // for guessing at ship locations while no longer in hunt mode. *conserves original hit. 

	static int pursuingDirection = 0;          // for keeping track of the direction being pursued
	static int checkingDirection = 0;          // for keeping track of the direction being checked. 
	/* when the orientation is found, pursuing will equal checking */
	
	if (Hunting){
		// update row and col to reflect the most probable location
		// of the player's ships

		//#################################################################################################################//
		// if you're hunting and there's a partially damaged ship, 
		// it means that you hit an adjacent ship while sinking the previous
		// one, so now sink that adjacent ship before continuing to hunt. 
		for (int r = 0; r < 11; r++){
			for (int c = 0; c < 11; c++){
				if (p2Guesses[r][c] == 2){
					Hunting = false;
					directionFound = false;
					bothDirections = false;
					findMostProbable = true;
					for (int i = 0; i < 4; i++){ 
						directions[i] = i + 1; 
					} // end for (int i = 0; i < 4; i++)
					row = r;
					col = c;
					idx = 1;
					whichWay = 0;
					cout << "   The CPU is moving on to the next found ship.\n";
					if (BSAI(p1Board, p2Guesses, p1ShipStatus, shipLengths, p1ShipLocations)){
						return true;
					} // end if (BSAI(p1Board, p2Guesses, p1ShipStatus, shipLengths, p1ShipLocations))
					else{
						return false;
					} // end else: if (BSAI(p1Board, p2Guesses, p1ShipStatus, shipLengths, p1ShipLocations))
				} // end if (p2Guesses[r][c] == 2)
			} // end for (int c = 0; c < 11; c++)
		} // end for (int r = 0; r < 11; r++)
		//#################################################################################################################//

		// calculate the probability fields and choose a location to guess -- updating row and col by 
		// pass by reference 
		smart_CPU_Fire(p2Guesses, p1ShipStatus, shipLengths, row, col);
		cout << "-----------------------------------------------------------------------------------------------------------------------\n";
		cout << "   The CPU is hunting.\n";
		Sleep(pauseTime * 1000);
		displayCPUGuess(row, col); // display the cpu's guess

		if (p1Board[row][col] == "S"){
			// the cpu hit a player's ship.
			cout << "   The CPU hit your ship!\n";
			p1Board[row][col] = "X";  // update player 1's board
			p2Guesses[row][col] = 2;    // update the cpu's guessing matrix
			Hunting = false;            // the cpu is no longer hunting!
			idx = 1;					// begin looking 1 away from the origin

			// the cpu hit a ship, so check if that ship got sunk.
			if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2,p2Guesses)){
				if (isGameOver(p1ShipStatus)){
					return true;   // the game is over.
				} // end if (isGameOver(p1ShipStatus))
				// a ship was sunk, so reset the flags and begin hunting
				Hunting = true;
				directionFound = false;
				bothDirections = false;
				findMostProbable = true;
				for (int i = 0; i < 4; i++){
					directions[i] = i + 1;
				} // end for (int i = 0; i < 4; i++)
			} // end if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2))
		} // end if (p1Board[row][col] == "S")
		else{
			// the cpu missed a player's ship
			cout << "   The CPU missed your ship!\n";
			p1Board[row][col] = "O";    // update player 1's board
			p2Guesses[row][col] = 1;    // update the cpu's guessing matrix
		} // end else: if (p1Board[row][col] == "S")
	} // end if (Hunting)
	else{
		if (directionFound){
			
			// keep guessing until you find a new
			// spot to guess.
			do{	
				// North
				if (pursuingDirection == 1){
					dRow = row - idx;
					dCol = col;
				} // end if (pursuingDirection == 1)
				// South
				else if (pursuingDirection == 2){
					dRow = row + idx;
					dCol = col;
				} // end else if (pursuingDirection == 2)
				// East
				else if (pursuingDirection == 3){
					dRow = row;
					dCol = col + idx;
				} // end else if (pursuingDirection == 3)
				// West
				else if (pursuingDirection == 4){
					dRow = row;
					dCol = col - idx;
				} // end // end else if (pursuingDirection == 4)
				
				// skip over previous guesses that
				// hit a ship and the ship never sunk
				if (p2Guesses[dRow][dCol] == 2){
					idx++;
				} // end if (p2Guesses[dRow][dCol] == 2)

				// if the direction is a dead end swap directions 
				if (p2Guesses[dRow][dCol] == 1 || p2Guesses[dRow][dCol] == 3){
					if (bothDirections){
						cout << "-----------------------------------------------------------------------------------------------------------------------\n";
						cout << "   The CPU found multiple touching ships.\n";
						directionFound = false;
						bothDirections = false;  
						whichWay++;
						idx = 1; 
						for (int i = 0; i < 4; i++){ 
							directions[i] = i + 1; 
						} // end for (int i = 0; i < 4; i++)
						if (BSAI(p1Board,p2Guesses,p1ShipStatus,shipLengths,p1ShipLocations)){
							return true;
						} // end if (BSAI(p1Board,p2Guesses,p1ShipStatus,shipLengths,p1ShipLocations))
						else{
							return false;
						} // end else: if (BSAI(p1Board,p2Guesses,p1ShipStatus,shipLengths,p1ShipLocations))
					} // end if (bothDirections)
					else{
						bothDirections = true;
						if (pursuingDirection % 2 == 1){
							pursuingDirection++;
							idx = 1; 
						} // end if (pursuingDirection % 2 == 1)
						else{
							pursuingDirection--;
							idx = 1; 
						} // end else: if (pursuingDirection % 2 == 1)
					} // end else: if (bothDirections)
				} // end if (p2Guesses[dRow][dCol] == 1 || p2Guesses[dRow][dCol] == 3)
			} while (p2Guesses[dRow][dCol] != 0);

			cout << "-----------------------------------------------------------------------------------------------------------------------\n";
			if (pursuingDirection == 1){
				cout << "   The CPU is pursuing North.\n";
			} // end if (pursuingDirection == 1)
			else if (pursuingDirection == 2){
				cout << "   The CPU is pursuing South.\n";
			} // end else if (pursuingDirection == 2)
			else if (pursuingDirection == 3){
				cout << "   The CPU is pursuing East.\n";
			} // end else if (pursuingDirection == 3)
			else if (pursuingDirection == 4){
				cout << "   The CPU is pursuing West.\n";
			} // end else if (pursuingDirection == 4)
			Sleep(pauseTime * 1000);
			displayCPUGuess(dRow, dCol); // display the cpu's guess

			if (p1Board[dRow][dCol] == "S"){
				// the cpu hit a player's ship.
				cout << "   The CPU hit your ship!\n";
				p1Board[dRow][dCol] = "X";    // update player 1's board
				p2Guesses[dRow][dCol] = 2;    // update the cpu's guessing matrix
				idx++;	                      // begin looking 1 away from the origin
				directionFound = true;        // the cpu has discovered the orientation

				// the cpu hit a ship, so check if that ship got sunk.
				if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2,p2Guesses)){
					if (isGameOver(p1ShipStatus)){
						return true;   // the game is over.
					} // end if (isGameOver(p1ShipStatus))
					// a ship was sunk, so reset the flags and begin hunting
					Hunting = true;
					directionFound = false;
					bothDirections = false;
					findMostProbable = true;
					for (int i = 0; i < 4; i++){ 
						directions[i] = i + 1;
					} // end for (int i = 0; i < 4; i++)
				} // end if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2))
			} // end if (p1Board[dRow][dCol] == "S")
			else if (p1Board[dRow][dCol] == " "){
				// the cpu missed a player's ship
				cout << "   The CPU missed your ship!\n";
				p1Board[dRow][dCol] = "O";    // update player 1's board
				p2Guesses[dRow][dCol] = 1;    // update the cpu's guessing matrix
			} // end else: if (p1Board[dRow][dCol] == "S")
		} // end if (directionFound)
		else{

			// rearrange the directions array to reflect
			// the order of the most probable orientations
			// of the ship.
			if (findMostProbable){
				huntingMostProbable(p2Guesses, row, col, directions);
				findMostProbable = false;
				whichWay = 0;
			} // end if (findMostProbable)

			do{
				checkingDirection = directions[whichWay];
				// North
				if (checkingDirection == 1){
					dRow = row - idx;
					dCol = col;
				} // end if (checkingDirection == 1)
				// South
				else if (checkingDirection == 2){
					dRow = row + idx;
					dCol = col;
				} // end else if (checkingDirection == 2)
				// East
				else if (checkingDirection == 3){
					dRow = row;
					dCol = col + idx;
				} // end else if (checkingDirection == 3)
				// West
				else if (checkingDirection == 4){
					dRow = row;
					dCol = col - idx;
				} // end else if (checkingDirection == 3)
				if (dRow >= 1 && dRow <= 10 && dCol >= 1 && dCol <= 10){
					if (p2Guesses[dRow][dCol] != 0){
						whichWay++;
					} // end if (p2Guesses[dRow][dCol] != 0)
				} // end if (dRow >= 1 && dRow <= 10 && dCol >= 1 && dCol <= 10)
				else{
					whichWay++;
				} // end else: if (dRow >= 1 && dRow <= 10 && dCol >= 1 && dCol <= 10)
			} while (p2Guesses[dRow][dCol] != 0);

			cout << "-----------------------------------------------------------------------------------------------------------------------\n";
			if (checkingDirection == 1){
				cout << "   The CPU is checking North.\n";
			} // end if (checkingDirection == 1)
			else if (checkingDirection == 2){
				cout << "   The CPU is checking South.\n";
			} // end else if (checkingDirection == 2)
			else if (checkingDirection == 3){
				cout << "   The CPU is checking East.\n";
			} // end else if (checkingDirection == 3)
			else if (checkingDirection == 4){
				cout << "   The CPU is checking West.\n";
			} // end else if (checkingDirection == 4)
			Sleep(pauseTime * 1000);
			displayCPUGuess(dRow, dCol); // display the cpu's guess

			if (p1Board[dRow][dCol] == "S"){
				// the cpu hit a player's ship.
				cout << "   The CPU hit your ship!\n";
				cout << "   The CPU has entered Destroyer Mode.\n";
				p1Board[dRow][dCol] = "X";    // update player 1's board
				p2Guesses[dRow][dCol] = 2;    // update the cpu's guessing matrix
				idx++;	                      // begin looking 1 away from the origin
				directionFound = true;        // the cpu has discovered the orientation
				pursuingDirection = checkingDirection;

				// the cpu hit a ship, so check if that ship got sunk.
				if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2,p2Guesses)){
					if (isGameOver(p1ShipStatus)){
						return true;   // the game is over.
					} // end if (isGameOver(p1ShipStatus))
					// a ship was sunk, so reset the flags and begin hunting
					Hunting = true;
					directionFound = false;
					bothDirections = false;
					findMostProbable = true;
					for (int i = 0; i < 4; i++){ directions[i] = i + 1; }
				} // end if (isShipSunk(p1ShipLocations, p1ShipStatus, p1Board, 2))
			} // end if (p1Board[dRow][dCol] == "S")
			else if (p1Board[dRow][dCol] == " "){
				// the cpu missed a player's ship
				cout << "   The CPU missed your ship!\n";
				p1Board[dRow][dCol] = "O";    // update player 1's board
				p2Guesses[dRow][dCol] = 1;    // update the cpu's guessing matrix
			} // end else: if (p1Board[dRow][dCol] == "S")
		} // end else: if (directionFound)
	} // end else: if (Hunting)
	return false;
} // end bool BSAI(string p1Board[11][11], int p2Guesses[11][11], bool p1ShipStatus[5], int shipLengths[5], int p1ShipLocations[5][4])
	void endPrintBoard(string p1Board[11][11], string p1Display[11][11], string p2Board[11][11], string p2Display[11][11], string p1Guesses[11][11]){

	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "|" << boardLabelColor << "                       CPU Game Board                                          Your Game Board                       " << black << "|\n";
	cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	cout << "|                                                                                                                     |\n";
	for (int r = 0; r < 11; r++){
		cout << "|\t";
		for (int c = 0; c < 11; c++){
			if (r == 0){
				cout << boardLabelColor << p2Board[r][c] << black << "   ";
			} // end if (r == 0)
			else{
				if (p2Board[r][c] == "S"){
					cout << blue << p2Display[r][c] << black << " | ";
				} // end if (p2Board[r][c] == "S"
				else if (p2Board[r][c] == "X"){
					cout << red << p2Display[r][c] << black << " | ";
				} // end else if (p2Board[r][c] == "X")
				else if (p1Guesses[r][c] == "O"){
					cout << missGuess << p1Guesses[r][c] << black << " | ";
				} // end else if (p1Guesses[r][c] == "O")
				else if (p2Board[r][c] == " "){
					cout << p2Board[r][c] << black << " | ";
				} // end else if (p2Board[r][c] == " "
				else{
					cout << boardLabelColor << p2Board[r][c] << black << " | ";
				} // end else 
			} // end else: if (r == 0)
		} // end for (int c = 0; c < 11; c++
		cout << "\t\t";
		for (int c = 0; c < 11; c++){
			if (r == 0){
				cout << boardLabelColor << p1Board[r][c] << black << "   ";
			} // end if (r == 0)
			else{
				if (p1Board[r][c] == "S"){
					cout << blue << p1Display[r][c] << black << " | ";
				} // end if (p1Board[r][c] == "S")
				else if (p1Board[r][c] == "X"){
					cout << red << p1Display[r][c] << black << " | ";
				} // end else if (p1Board[r][c] == "X")
				else if (p1Board[r][c] == "O"){
					cout << missGuess << p1Board[r][c] << black << " | ";
				} // end else if (p1Board[r][c] == "O")
				else if (p1Board[r][c] == " "){
					cout << p1Board[r][c] << " | ";
				} // end else if (p1Board[r][c] == " ")
				else{
					cout << boardLabelColor << p1Board[r][c] << black << " | ";
				} // end else
			} // end else: if (r == 0)
		} // end for (int c = 0; c < 11; c++)
		if (r == 0){
			cout << "         |";
		} // end if (r == 0)
		else{
			cout << "          |";
		} // end else: if (r == 0)
		cout << "\n| \t  -----------------------------------------  \t\t  -----------------------------------------           |\n";
	} // end for (int r = 0; r < 11; r++)
	cout << "|                                                                                                                     |";
	cout << "\n-----------------------------------------------------------------------------------------------------------------------\n";
} // end endPrintBoard(string p1Board[11][11], string p1Display[11][11], string p2Board[11][11], string p2Display[11][11], string p1Guesses[11][11])
	void setDisplayBoard(string board[11][11], int locs[5][4]){
	string shipNames[5] = { "A", "B", "C", "S", "D" };
	for (int s = 0; s < 5; s++){
		for (int r = locs[s][0]; r <= locs[s][2]; r++){
			for (int c = locs[s][1]; c <= locs[s][3]; c++){
				board[r][c] = shipNames[s];
			} // end for (int c = locs[s][1]; c <= locs[s][3]; c++
		} // end for (int r = locs[s][0]; r <= locs[s][2]; r++)
	} // end for (int s = 0; s < 5; s++)
} // end void setDisplayBoard(string board[11][11], int locs[5][4])
