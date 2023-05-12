#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

void importGrid()
{
    string filename = "grid1000.csv"; // name of the input file
    vector<vector<int>> matrix;       // a vector of vectors to store the matrix

    ifstream file(filename); // open the file for reading
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {                    // read each line of the file
            vector<int> row; // create a vector to store the row
            stringstream ss(line);
            string value;
            while (getline(ss, value, ','))
            {
                // read each comma-separated value
                row.push_back(stoi(value)); // convert the string to an integer and add it to the row vector
            }
            matrix.push_back(row); // add the row vector to the matrix vector
        }
        file.close(); // close the file
    }

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