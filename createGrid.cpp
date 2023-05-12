#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

void importGrid()
{
    string filename = "grid.csv"; // name of the input file
    vector<vector<int>> matrix;   // a vector of vectors to store the matrix

    srand(time(NULL));
    ofstream file(filename); // open the file for reading
    if (file.is_open())
    {
        string line;

        int count = 1000;
        for (int i = 0; i < count; i++)
        {
            for (int j = 0; j < count; j++)
            {
                file << rand() % 2 << ",";
            }
            file << endl;
        }
    }
    file.close(); // close the file

    // Print the matrix for verification
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    return;
}

int main()
{
    importGrid();
    return 0;
}