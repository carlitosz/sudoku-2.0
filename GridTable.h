// ============================================================================
// Author: Carlos Zaragoza
//
// Description: This is the GridTable that extends from the Grid.h class to 
//              create a nice interface for better UX. It adds colors and other
//              modifications to give the user a better experience. This class
//              can be replaced by extending the Grid.h class and implementing
//              your own. This way you can create your own table graphics.
// ============================================================================

#ifndef GRID_TABLE_H
#define GRID_TABLE_H

#include "Colors.h"
#include "Grid.h"

const int EMPTY = -1;
const int POPULATED = -2;

template<class T>
class GridTable : public Grid<T> {
    public:
        GridTable() { this->numRows = 0; this->numCols = 0; }
        GridTable(const int r, const int c) : Grid<T>(r, c) { };
        void print();
        int getTotalEmpty();
        int getTotalPopulated();
        void insert(const T&, stack<char>&);
        bool match(vector<vector<T> >&);

    private:
        void printInvalid();
        bool validateUserInput(const string);
        int getTotalOfType(int);
        void printHorizontalMargin();
        void printVerticalMargin();
};


// ============================================================================
// match.
//
// Input -> a vector containing x by y data.
// Output -> true if the vector is the same as the grid columns, else false.
// ============================================================================
template<class T>
bool GridTable<T>::match(vector<vector<T> >& someVector) {
    bool isValid = true;

    for (int i = 0; i < this->numRows; ++i) {
        for (int j = 0; j < this->numCols; ++j) {
            if (someVector[i][j] != this->columns[i][j]) {
                isValid = false;
                break;
            }
        }
    }

    return isValid;
}

// ============================================================================
// validateUserInput.
//
// Input -> user's input
// Output -> true if valid, else false.
// ============================================================================
template<class T>
bool GridTable<T>::validateUserInput(const string input) {
    if (input == "") return false;
    if (input.size() > 1) {
        printInvalid();
        return false;
    }

    // if (input.at(0) < 49 || input.at(0) > 57) return false;
    if (input.at(0) == 'y' || input.at(0) == 'n') return true;

    return false;
}

// ============================================================================
// insert.
//
// Input -> value - the new value to insert.
// Input -> coordinates - the spot where to insert.
// Output -> nothing.
// ============================================================================
template<class T>
void GridTable<T>::insert(const T& value, stack<char>& coordinates) {
    int x = coordinates.top() - 48;
    coordinates.pop();
    int y = coordinates.top() - 48;
    coordinates.pop();

    string userInput = "";
    if (this->columns[x][y] != ' ') {
        while (validateUserInput(userInput) == false) {
            cout << FBLACK_RED << "Coordinates ["
                 << x << ", " << y << "] contain a value."
                 << "Overwrite? (y/n): " << RST;
            cin >> userInput;
        }

        if (userInput.at(0) == 'n') {
            cout << FBLACK_RED << "Value not inserted" << RST << endl;
            return;
        } else if (userInput.at(0) == 'y') {
            cout << FBLACK_GREEN << "Value inserted!" << RST << endl;
            this->columns[x][y] = value;
            return;
        }
    }

    this->columns[x][y] = value;
}

// ============================================================================
// print.
//
// Input -> nothing.
// Output -> prints the table to stdout.
//
// Need to refactor this :|
// ============================================================================
template<class T>
void GridTable<T>::print() {
    if (this->columns == NULL) {
        cout << "No data!" << endl;
        return;
    }

    cout << endl << endl;
    cout << FBLACK_GREEN << "                               ";
    cout << "Y";
    cout << "                               " << RST << endl;
    cout << FBLACK_GREEN << "  " << RST;
    cout << FRED_CYAN << "      0     1     2";
    cout << "      3     4     5";
    cout << "      6     7     8    " << RST << endl;
    for (int i = 0; i < this->numCols; ++i) {
        if (i % 3 == 0 && i != 0) printHorizontalMargin();
        if (i == 4) {
            cout << FBLACK_GREEN << "X " << RST;
        } else {
            cout << FBLACK_GREEN << "  " << RST;
        }
        cout << FRED_CYAN << " " << i << " " << RST;
        for (int j = 0; j < this->numRows; j++) {
            if (j % 3 == 0) printVerticalMargin();
            cout << FYELLOW_BLACK << "  " << this->columns[i][j] << "  |" << RST;
        }
        printVerticalMargin();
        cout << endl;
    }

    printHorizontalMargin();
}

// ============================================================================
// printHorizontalMargin.
//
// prints a new horizontal margin.
// ============================================================================
template<class T>
void GridTable<T>::printHorizontalMargin() {
    string margin = "-------------------------------------------------------- ";
    cout << FBLACK_GREEN << "  " << RST;
    cout << FRED_CYAN << "    " << margin << RST << endl;
}

// ============================================================================
// printHorizontalMargin.
//
// prints a new horizontal margin.
// ============================================================================
template<class T>
void GridTable<T>::printVerticalMargin() {
    string margin = "|";
    cout << FRED_CYAN << margin << RST;
}

// ============================================================================
// getTotalEmpty.
//
// Input -> nothing.
// Output -> the number of empty cells in the table.
// ============================================================================
template<class T>
int GridTable<T>::getTotalEmpty() {
    return getTotalOfType(EMPTY);
}

// ============================================================================
// getTotalPopulated.
//
// Input -> nothing.
// Output -> the number of populated cells in the table.
// ============================================================================
template<class T>
int GridTable<T>::getTotalPopulated() {
    return getTotalOfType(POPULATED);
}

// ============================================================================
// getTotalOfType.
//
// Input -> the type to search for.
// Output -> the total of the type.
// ============================================================================
template<class T>
int GridTable<T>::getTotalOfType(const int type) {
    int counter = 0;

    // Count all empty cells.
    for (int i = 0; i < this->numRows; ++i) {
        for (int j = 0; j < this->numCols; ++j) {
            if (this->columns[i][j] == 32) counter++;
        }
    }

    // Check which type and return that number.
    switch (type) {
        case EMPTY:
            return counter;
        case POPULATED:
            return (this->numRows * this->numCols) - counter;
        default:
            break;
    }

    return counter;
}

// ============================================================================
// printInvalid()
//
// Input -> nothing.
// Output -> a message to stdout saying invalid input.
// ============================================================================
template<class T>
void GridTable<T>::printInvalid() {
    cout << FBLACK_RED << "Invalid input, please try again..." << RST << endl;
}

#endif /* GRID_TABLE_H */
