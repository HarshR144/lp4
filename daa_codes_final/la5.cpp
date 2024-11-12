#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

class NQueens {
private:
    int n;
    vector<vector<int>> board;
    
    // Function to check if a queen can be placed on board[row][col]
    bool isSafe(int row, int col) {
        // Check this row on left side
        for (int j = 0; j < col; j++)
            if (board[row][j])
                return false;
        
        // Check this row on right side
        for (int j = col + 1; j < n; j++)
            if (board[row][j])
                return false;
            
        // Check this column on upper side
        for (int i = 0; i < row; i++)
            if (board[i][col])
                return false;
            
        // Check this column on lower side
        for (int i = row + 1; i < n; i++)
            if (board[i][col])
                return false;
        
        // Check upper diagonal on left side
        for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
            if (board[i][j])
                return false;
        
        // Check lower diagonal on left side
        for (int i = row, j = col; i < n && j >= 0; i++, j--)
            if (board[i][j])
                return false;
            
        // Check upper diagonal on right side
        for (int i = row, j = col; i >= 0 && j < n; i--, j++)
            if (board[i][j])
                return false;
            
        // Check lower diagonal on right side
        for (int i = row, j = col; i < n && j < n; i++, j++)
            if (board[i][j])
                return false;
        
        return true;
    }
    
    // Recursive function to solve N-Queens problem
    bool solveNQueensUtil(int queensPlaced, int firstRow, int firstCol) {
        // Base case: If all queens are placed, return true
        if (queensPlaced == n)
            return true;
        
        // Try placing queen in each available position
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Skip the position where first queen is already placed
                // if (i == firstRow && j == firstCol) continue;
                // i place 1 queen 

                // Skip positions where queens are already placed
                if (board[i][j] == 1) continue;
                
                if (isSafe(i, j)) {
                    // Place queen
                    board[i][j] = 1;
                    
                    // Recur to place rest of the queens
                    if (solveNQueensUtil(queensPlaced + 1, firstRow, firstCol))
                        return true;
                    
                    // If placing queen doesn't lead to a solution,
                    // remove queen from board[i][j]
                    board[i][j] = 0;
                }
            }
        }
        
        // If queen can't be placed in any position, return false
        return false;
    }

public:
    NQueens(int size) : n(size) {
        board = vector<vector<int>>(n, vector<int>(n, 0));
    }
    
    bool solve(int firstRow, int firstCol) {
        // Place the first queen
        if (!isSafe(firstRow, firstCol)) {
            cout << "First queen position is not safe!" << endl;
            return false;
        }
        
        board[firstRow][firstCol] = 1;
        
        // Start solving with one queen already placed
        return solveNQueensUtil(1, firstRow, firstCol);
    }
    
    void printSolution() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << (board[i][j] ? "Q " : ". ");
            }
            cout << endl;
        }
    }
};

int main() {
    int n, firstRow, firstCol;
    
    cout << "Enter the size of the board (n): ";
    cin >> n;
    
    cout << "Enter the position for first queen (row and column from 0 to " << n-1 << "): ";
    cin >> firstRow >> firstCol;
    
    if (firstRow < 0 || firstRow >= n || firstCol < 0 || firstCol >= n) {
        cout << "Invalid position for first queen!" << endl;
        return 1;
    }
    
    auto startTime = chrono::high_resolution_clock::now();
    
        NQueens nQueens(n);
        
        if (nQueens.solve(firstRow, firstCol)) {
            cout << "\nSolution exists! Here's the board:\n\n";
            nQueens.printSolution();
        } else {
            cout << "\nNo solution exists for the given first queen position.\n";
        }
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    




    cout << "\nTime taken: " << duration.count() << " microseconds" << endl;
    
    return 0;
}