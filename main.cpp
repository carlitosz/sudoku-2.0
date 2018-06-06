// ============================================================================
// Author: Carlos Zaragoza
//
// Description: This is the main driver for the Sudoku game on the command
//              line. It creates a grid and grid table to support the graphical
//              output of the sudoku game to the command line. Please review
//              GridTable.h and Grid.h to view the implementation.
// ============================================================================

#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <stack>
#include <curl/curl.h>
using namespace std;

// ============================================================================
// User defined classes.
// ============================================================================
#include "GridTable.h"

// ============================================================================
// Globals
// ============================================================================
const int ROWS = 9;
const int COLS = 9;
const int TOTAL = ROWS * COLS;
const string ERROR = "Something went totally wrong, please restart the game.";
const string INCOMPLETE = "Cannot check solution, the puzzle is incomplete!";
const string CHECKING = "Checking your puzzle for correctness, please wait...";

// ============================================================================
// Function prototypes.
// ============================================================================
vector<vector<char> > getPuzzle(char, bool);
string getURL(char, bool);
int fetchValue();
void printInvalid();
stack<char> fetchCoords();
bool validateCoord(string coord);
bool validateValue(string val);
char printMenu();
char printOptions();
void printMsgAndExit();
bool validateOption(string);
bool validateLevel(string);
void printErrorAndExit(string);
void clearStream();
void checkSolution(char difficulty, GridTable<char>& t);
void printWelcome();

// ============================================================================
// Main.
// ============================================================================
int main(void) {

    // ========================================================================
    // Welcome menu.
    // ========================================================================
    printWelcome();

    char c = printMenu();

    if (c == 'q') printMsgAndExit();

    // ========================================================================
    // Create the table and load the puzzle.
    // Load puzzle from file based on difficulty level.
    // ========================================================================
    GridTable<char> table(ROWS, COLS);
    vector<vector<char> > puzzle = getPuzzle(c, false);
    table.populate(puzzle);

    // ========================================================================
    // Print table.
    // ========================================================================
    table.print();

    // ========================================================================
    // Loop the game.
    // ========================================================================
    char input = 'a';
    stack<char> coords;
    bool inserted = false;
    char val;
    int x;
    int y;

    clearStream();
    while (input != 'q') {
        input = printOptions();

        switch (input) {
            case 'q':
                printMsgAndExit();
                break;
            case 'e':
                val = fetchValue();
                coords = fetchCoords();
                table.insert(val, coords);
                table.print();
                break;
            case 'p':
                table.print();
                break;
            case 'c':
                checkSolution(c, table);
                break;
            default:
                printErrorAndExit(ERROR);
        }
    }

    return 0;
}


// ============================================================================
// fetchCoords.
//
// Input -> nothing.
// Output -> coordinates <int>, <int> in a LIFO stack.
// 
// The stack will be such that the X value will be on top.
// ============================================================================
stack<char> fetchCoords() {
    string x = "";
    string y = "";

    while (validateCoord(x) == false) {
        cout << FBLACK_GREEN << "Enter X: " << RST;
        cin >> x;
    }

    while (validateCoord(y) == false) {
        cout << FBLACK_GREEN << "Enter Y: " << RST;
        cin >> y;
    }

    stack<char> temp;
    temp.push(y.at(0));
    temp.push(x.at(0));

    return temp;
}


// ============================================================================
// validateCoord.
//
// Input -> character value of a coordinate to validate.
// Output -> true if valid, else false.
// ============================================================================
bool validateCoord(string coord) {
    if (coord == "") return false;
    if (coord.size() > 1 || coord.at(0) < 48 || coord.at(0) > 56) {
        printInvalid();
        return false;
    }

    return true;
}


// ============================================================================
// fetchValue.
//
// Input -> nothing.
// Output -> the new value the user wants to insert.
// ============================================================================
int fetchValue() {
    string val = "";

    while (validateValue(val) == false) {
        cout << FBLACK_GREEN << "Enter value: " << RST;
        cin >> val;
    }

    return val.at(0);
}


// ============================================================================
// validateValue.
//
// Input -> the value to validate.
// Output -> true if valid, else false
// ============================================================================
bool validateValue(string val) {
    if (val == "") return false;
    if (val.size() > 1 || val.at(0) < 49 || val.at(0) > 57) {
        printInvalid();
        return false;
    }

    return true;
}


// ============================================================================
// checkSolution.
//
// Input -> c - the difficulty level.
// Input -> t - the grid table.
// Output -> nothing.
// ============================================================================
void checkSolution(char difficulty, GridTable<char>& t) {
    int empty = t.getTotalEmpty();
    int populated = t.getTotalPopulated();

    cout << FBLACK_GREEN << CHECKING << RST << endl;

    // Puzzle not completed.
    if (empty != 0) {
        cout << FWHITE_RED << INCOMPLETE << RST << endl;
    }

    // Puzzle complete.
    if (empty == 0) {
        cout << endl << endl;
        sleep(2);

        // Get solution from file and match against current puzzle progress.
        vector<vector<char> > solution = getPuzzle(difficulty, true);
        if (t.match(solution) == false) {
            cout << FWHITE_RED << "Sorry, it looks like the puzzle is "
                 << " incorrect. Please skim for errors and check again."
                 << RST << endl;

            return;
        }

        cout << FGREY_PURPLE << " ********* Congratulations! ********* "
             << RST << endl;
        cout << FGREY_PURPLE << "You successfully completed the puzzle!"
             << RST << endl;
        cout << FGREY_PURPLE << "Thanks for playing!"
             << RST << endl;
        exit(1);
    }
}

// ============================================================================
// printWelcome.
//
// Input -> nothing.
// Output -> nothing.
// ============================================================================
void printWelcome() {
    string len1 = " * * * * * * * * * * * * * * * * * * ";
    string len2 = " * * * * * * * * * * * * * * * * * * * * * *  ";

    cout << "\n\n";
    cout << FGREY_PURPLE;
    cout << len1 << RST;
    cout << " Welcome to Sudoku ";
    cout << FGREY_PURPLE;
    cout << len1 + " " << RST << endl;

    /* Start description */
    cout << FGREY_PURPLE << " *" << RST;
    cout << "Welcome to Sudoku version 2.0. This version of the game contains "
         << "improvements on the back-";
    cout << FGREY_PURPLE << "* " << RST << endl;
    cout << FGREY_PURPLE << " *" << RST;
    cout << "end and the front-end as well. One of the most important "
         << "improvements is now the ability  ";
    cout << FGREY_PURPLE << "* " << RST << endl;
    cout << FGREY_PURPLE << " *" << RST;
    cout << "to download puzzles from the web instead of having them saved on your "
         << "computer. Other     ";
    cout << FGREY_PURPLE << "* " << RST << endl;
    cout << FGREY_PURPLE << " *" << RST;
    cout << "changes include: using sorts, hashing, trees, and graphs. This version "
         << " of Sudoku is just ";
    cout << FGREY_PURPLE << "* " << RST << endl;
    cout << FGREY_PURPLE << " *" << RST;  
    cout << "as fun as the first so test your knowledge and try to solve all"
         << " the puzzles!!!            ";
    cout << FGREY_PURPLE << "* " << RST << endl;
    cout << FGREY_PURPLE << " *" << RST;
    cout << FGREY_PURPLE << len2 << len2 << RST << endl;

    /* End description */

    cout << FBLACK_GREEN;
    cout << "Choose your level of difficulty from the following menu";
    cout << RST;
    cout << endl << endl;
    cout << RST;
}


// ============================================================================
// printMsgAndExit.
//
// Input -> nothing.
// Output -> goodbye message.
// ============================================================================
void printMsgAndExit() {
    cout << endl << endl;
    cout << FGREY_PURPLE << "* Thanks for playing Sudoku! * " << RST << endl;
    exit(1);
}


// ============================================================================
// printInvalid()
//
// Input -> nothing.
// Output -> a message to stdout saying invalid input.
// ============================================================================
void printInvalid() {
    cout << FBLACK_RED << "Invalid input, please try again..." << RST << endl;
}

// ============================================================================
// Writes downloaded data to a buffer.
// ============================================================================
size_t write_data(void *buffer, size_t size, size_t nmemb, void *param) {
  std::string& text = *static_cast<std::string*>(param);
  size_t totalsize = size * nmemb;
  text.append(static_cast<char*>(buffer), totalsize);

  return totalsize;
}

// ============================================================================
// getPuzzleFromFile.
//
// Input -> the difficulty level.
// Input -> whether to retrieve the solution or not.
// Output -> An array of ints containing the puzzle.
// ============================================================================
vector<vector<char> > getPuzzle(char difficulty, bool solution = false) {
    static vector<vector<char> > puzzle(ROWS, vector<char>(COLS));
    string result;
    CURL *curl;
    CURLcode res;
    string difficultyURL = getURL(difficulty, solution);
    char url[difficultyURL.length()];
    strcpy(url, difficultyURL.c_str());
    char outfilename[FILENAME_MAX] = "./json.zip";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        if (!solution) {
            cout << endl << endl 
                 << "Downloading puzzle from the web. . ." << endl;
            usleep(2000000);
        }

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    } else {
        cout << "Something went wrong, please check that you have an "
             << "internet connection and restart the game!" << endl;
        exit(1);
    }

    int start = 0;
    int end = 10;
    int row = 0;
    do {
        string rowData = result.substr(start, end);
        for (int i = 0; i < rowData.size(); ++i) {
            char c = rowData.at(i);
            if (c == 'x') c = ' ';
            puzzle[row][i] = c;
        }
        row++;
        start += 10;
        end += 10;
    } while(start < 100);

    return puzzle;
}


// ============================================================================
// printOptions.
//
// Input -> nothing.
// Output -> the options the user has during the game.
// ============================================================================
char printOptions() {
    string userInput = "";

    cout << endl << endl;
    while(validateOption(userInput) == false) {
        cout << FBLACK_GREEN << "p)rint the table" << RST << endl;
        cout << FBLACK_GREEN << "e)nter value" << RST << endl;
        cout << FBLACK_GREEN << "c)heck solution" << RST << endl;
        cout << FBLACK_GREEN << "q)uit game" << RST << endl << endl;
        cout << FBLACK_GREEN << "Enter choice (p, e, c, q): " << RST;
        cin >> userInput;
    }

    return userInput.at(0);
}


// ============================================================================
// validateOption.
//
// Input -> Option chosen as char.
// Output -> true if valid, else false.
// ============================================================================
bool validateOption(string input) {
    if (input == "") return false;

    char c = input.at(0);
    if (c == 'p' || c == 'e' || c == 'q' || c == 'c') return true;

    cout << endl << endl << "Invalid input, please try again..." << endl << endl;
    return false;
}


// ============================================================================
// printMenu.
//
// Input -> nothing
// Output -> returns the user input as a character i.e. 'e', 'i' etc...
// ============================================================================
char printMenu() {
    string userInput = "";

    while (validateLevel(userInput) == false) {
        cout << FBLACK_GREEN << "e)asy" << RST << endl;
        cout << FBLACK_GREEN << "i)ntermediate" << RST << endl;
        cout << FBLACK_GREEN << "d)ifficult" << RST << endl;
        cout << FBLACK_GREEN << "r)eally hard" << RST << endl;
        cout << FBLACK_GREEN << "q)uit" << RST << endl << endl;
        cout << FBLACK_GREEN << "Please enter your level (e, i, d, r, q): "
             << RST;
        cin >> userInput;
    }

    return userInput.at(0);
}


// ============================================================================
// validateLevel.
//
// Input -> The user selection.
// Output -> true if the selection is valid, otherwise false.
// ============================================================================
bool validateLevel(string input) {
    // Beginning of program, don't need to validate.
    if (input == "") return false;

    // Valid input.
    char c = input.at(0);
    if (c == 'e' || c == 'i' || c == 'd' || c == 'r' || c == 'q') return true;

    // Invalid input.
    cout << "\n\nInvalid input, please try again...\n\n" << endl;
    return false;
}


// ============================================================================
// printErrorAndExit.
// 
// Input -> the message to be printed.
// Output -> nothing.
// ============================================================================
void printErrorAndExit(string message = "") {
    cout << message << endl;
    exit(0);
}


// ============================================================================
// clearStream.
//
// Input -> nothing.
// Output -> nothing.
// ============================================================================
void clearStream() {
    fflush(stdin);
}


// ============================================================================
// getURL.
//
// Input -> the difficulty level.
// Output -> the URL of the difficulty level.
// ============================================================================
string getURL(char difficulty, bool solution = false) {
    string easy = "http://m.uploadedit.com/bbtc/1528267058303.txt";
    string easySol = "http://m.uploadedit.com/bbtc/1528253085392.txt";
    string inter = "http://m.uploadedit.com/bbtc/1528267078101.txt";
    string interSol = "http://m.uploadedit.com/bbtc/1528267101186.txt";
    string diff = "http://m.uploadedit.com/bbtc/1528267137564.txt";
    string diffSol = "http://m.uploadedit.com/bbtc/1528267148727.txt";
    string expert = "http://m.uploadedit.com/bbtc/1528267163452.txt";
    string expertSol = "http://m.uploadedit.com/bbtc/1528267187305.txt";

    switch (difficulty) {
        case 'e':
            if (solution) return easy;
            return easySol;
        case 'i':
            if (solution) return inter;
            return interSol;
        case 'd':
            if (solution) return diff;
            return diffSol;            
        case 'r':
            if (solution) return expert;
            return expertSol;
        default:
            printErrorAndExit(ERROR);
    }

    return "";
}