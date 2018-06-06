// ============================================================================
// Author: Carlos Zaragoza
//
// Description: The Grid class creates 2 dimensional array for T data, and then
//              populates the 2 dimensional array with that data. Any user that
//              extends this class will have to implement a few functions to
//              build a table for graphic representation of the grid.
//              I provided an example called GridTable.h (may need refactoring)
//              to give an idea about how to implement a table.
// ============================================================================

#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

template<class T>
class Grid {
    public:
        Grid() { columns = NULL; }
        Grid(const int rows, const int cols);
        void populate(const vector<vector<T> > data);
        int getNumRows();
        int getNumCols();
        void setNumRows(int r);
        void setNumCols(int c);
        int getTotalElements() { return numRows * numCols; }
        ~Grid();

        virtual void print() = 0;
        virtual void printHorizontalMargin() = 0;
        virtual void printVerticalMargin() = 0;
        virtual void insert(const T&, stack<char>&) = 0;

    protected:
        T **columns;
        int numRows;
        int numCols;
};

// ============================================================================
// destructor.
// ============================================================================
template<class T>
Grid<T>::~Grid(){
    for (int i = 0; i < numRows; ++i) {
        delete [] columns[i];
    }

    delete [] columns;
}


// ============================================================================
// setNumCols.
//
// Input -> integer value.
// Output -> nothing.
// ============================================================================
template<class T>
void Grid<T>::setNumCols(int c) {
    numCols = c;
}

// ============================================================================
// setNumRows.
//
// Input -> integer value.
// Output -> nothing.
// ============================================================================
template<class T>
void Grid<T>::setNumRows(int r) {
    numRows = r;
}

// ============================================================================
// getNumCols.
//
// Input -> nothing.
// Output -> the number of columns.
// ============================================================================
template<class T>
int Grid<T>::getNumCols() {
    return numCols;
}

// ============================================================================
// getNumRows.
//
// Input -> nothing.
// Output -> the number of rows.
// ============================================================================
template<class T>
int Grid<T>::getNumRows() {
    return numRows;
}

// ============================================================================
// Grid(int).
//
// Overloaded constructor.
// ============================================================================
template<class T>
Grid<T>::Grid(const int rows, const int cols) {
    // Set rows/cols.
    numRows = rows;
    numCols = cols;

    // Create array of pointers.
    columns = new T * [rows];

    // Create rows.
    for (int i = 0; i < cols; ++i) {
        columns[i] = new T[cols];
    }
}

// ============================================================================
// Populate.
// 
// Populates the 2D grid with T data.
// ============================================================================
template<class T>
void Grid<T>::populate(const vector<vector<T> > data) {
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            columns[i][j] = data[i][j];
        }
    }
}

#endif /* GRID_H */
