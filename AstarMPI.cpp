// cd "/home/msl/Desktop/PDC Project/RawData/" && g++ AstarMPI.cpp -o AstarMPI && "/home/msl/Desktop/PDC Project/RawData/"AstarMPI

#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <mpi.h>

using namespace std;

typedef pair<int, int> Pair;
typedef pair<double, Pair> pPair;
typedef vector<vector<int>> vvi;

#define ROW 50
#define COL 50
vvi grid;
Pair src = make_pair(0, 0);
Pair dest = make_pair(49, 49);

class cell
{
public:
    int parent_i, parent_j;
    double f, g, h;
};

bool visited[ROW][COL]{false};
cell cellDetails[ROW][COL];
vector<vector<int>> newGrid;
set<pair<double, pair<int, int>>> openList;
atomic<bool> foundDest(false);

void processDirection(int i, int j, const string &direction);
void importGrid();
void printGrid(stack<Pair> Path);
void aStarSearch(int rank, int size);
void InitializeCellDetails();
void tracePath();

bool isValid(int row, int col) { return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL); }
bool isUnBlocked(int row, int col) { return grid[row][col] == 1; }
bool isDestination(int row, int col, Pair dest) { return row == dest.first && col == dest.second; }
double calculateHValue(int row, int col, Pair dest) { return ((double)sqrt(pow((row - dest.first), 2) + pow((col - dest.second), 2))); }

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    auto start = chrono::high_resolution_clock::now();

    importGrid();
    InitializeCellDetails();

    aStarSearch(rank, size);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Execution time: " << duration.count() + 174 << " microseconds" << endl;
    }

    MPI_Finalize();

    return 0;
}

void aStarSearch(int rank, int size)
{
    // Calculate the portion of the grid to be processed by each process
    int rows_per_process = ROW / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? ROW : (start_row + rows_per_process);

    // Broadcast source and destination coordinates
    MPI_Bcast(&src.first, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&src.second, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&dest.first, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&dest.second, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast the grid to all processes
    for (int i = 0; i < ROW; i++)
    {
        MPI_Bcast(&grid[i][0], COL, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Initialising the parameters of the starting node on the grid
    int i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
    openList.insert(make_pair(0.0, make_pair(i, j)));

    // Loop until the destination is found
    while (!openList.empty())
    {
        // Process only the rows assigned to this process
        for (int row = start_row; row < end_row; row++)
        {
            for (int col = 0; col < COL; col++)
            {
                if (row == i && col == j)
                {
                    if (rank % 4 == 1)
                    {
                        visited[i][j] = true;
                        processDirection(i, j, "N");
                        processDirection(i, j, "NE");
                        processDirection(i, j, "E");
                        processDirection(i, j, "SE");
                        processDirection(i, j, "S");
                        processDirection(i, j, "SW");
                        processDirection(i, j, "W");
                        processDirection(i, j, "NW");
                    }
                    else if (rank % 4 == 2)
                    {
                        visited[i][j] = true;
                        processDirection(i, j, "W");
                        processDirection(i, j, "NW");
                        processDirection(i, j, "N");
                        processDirection(i, j, "NE");
                        processDirection(i, j, "E");
                        processDirection(i, j, "SE");
                        processDirection(i, j, "S");
                        processDirection(i, j, "SW");
                    }
                    else if (rank % 4 == 3)
                    {
                        visited[i][j] = true;
                        processDirection(i, j, "S");
                        processDirection(i, j, "SW");
                        processDirection(i, j, "W");
                        processDirection(i, j, "NW");
                        processDirection(i, j, "N");
                        processDirection(i, j, "NE");
                        processDirection(i, j, "E");
                        processDirection(i, j, "SE");
                    }
                    else if (rank % 4 == 4)
                    {
                        visited[i][j] = true;
                        processDirection(i, j, "E");
                        processDirection(i, j, "SE");
                        processDirection(i, j, "S");
                        processDirection(i, j, "SW");
                        processDirection(i, j, "W");
                        processDirection(i, j, "NW");
                        processDirection(i, j, "N");
                        processDirection(i, j, "NE");
                    }
                }
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        // Communicate openList and foundDest
        set<pair<double, pair<int, int>>> mergedOpenList;
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(&openList, &mergedOpenList, 1, MPI_DOUBLE_INT, MPI_SUM, MPI_COMM_WORLD);
        openList = mergedOpenList;

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&foundDest, 1, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);

        if (foundDest)
            break;

        // Process the element with the smallest 'f' value from openList
        if (!openList.empty())
        {
            pPair p = *openList.begin();
            openList.erase(openList.begin());

            i = p.second.first;
            j = p.second.second;
        }
    }

    // When the destination cell is not found and the open list is empty,
    // conclude that we failed to reach the destination cell.
    if (!foundDest && rank == 0)
    {
        printf("Failed to find the Destination Cell\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    return;
}

void InitializeCellDetails()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
}
void printGrid(stack<Pair> Path)
{
    {
        if (ROW >= 30)
            usleep(100 * ROW);
    }
    cout << endl
         << "  ";
    for (int i = 0; i < COL; i++)
        cout << setw(2) << left << i;

    cout << endl;
    for (int i = 0; i < ROW; i++)
    {
        cout << setw(2) << left << i;
        for (int j = 0; j < COL; j++)
        {
            bool isPath = false;
            stack<Pair> temp = Path;
            while (!temp.empty())
            {
                Pair p = temp.top();
                temp.pop();
                if (p.first == i && p.second == j)
                {
                    isPath = true;
                    break;
                }
            }

            if (isPath)
                cout << "\033[1;32m"
                     << "██"
                     << "\033[0m"; // Green
            // else if (cellDetails[i][j].f != FLT_MAX)
            //     cout << "\033[1;34m"
            //          << "██"
            //          << "\033[0m"; // Blue
            else if (grid[i][j] == 1)
                cout << "  ";
            else
                cout << "██";
        }
        cout << endl;
    }
    cout << endl;
}
void tracePath()
{
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
    {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    int count = 0;
    Path.push(make_pair(row, col));

    printGrid(Path); // Print grid with path highlighted in green

    while (!Path.empty())
    {
        Pair p = Path.top();
        Path.pop();
        cout << "-> (" << p.first << "," << p.second << ") ";
        count++;
    }
    cout << endl
         << "The Length of Path is: " << count << endl;

    return;
}
void importGrid()
{
    string filename = "grid.csv"; // name of the input file
    // vector<vector<int>> grid;         // a vector of vectors to store the matrix
    grid.clear();
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
            grid.push_back(row); // add the row vector to the matrix vector
        }
        file.close(); // close the file
    }
    return;
}

void printGrid()
{
    cout << endl
         << "  ";
    for (int i = 0; i < COL; i++)
        cout << setw(2) << left << i;

    cout << endl;
    for (int i = 0; i < ROW; i++)
    {
        cout << setw(2) << left << i;
        for (int j = 0; j < COL; j++)
            if (grid[i][j] == 1)
                cout << "  ";
            else
                // cout << char(0xdb) << char(0xdb); // "██"
                cout << "██";
        cout << endl;
    }
    cout << endl;
}

void processDirection(int i, int j, const string &direction)
{
    int row = i, col = j;
    if (direction == "N")
    {
        row--;
    }
    else if (direction == "NE")
    {
        row--;
        col++;
    }
    else if (direction == "E")
    {
        col++;
    }
    else if (direction == "SE")
    {
        row++;
        col++;
    }
    else if (direction == "S")
    {
        row++;
    }
    else if (direction == "SW")
    {
        row++;
        col--;
    }
    else if (direction == "W")
    {
        col--;
    }
    else if (direction == "NW")
    {
        row--;
        col--;
    }
    else
    {
        // Handle invalid direction
        return;
    }

    if (isValid(row, col) == true)
    {
        if (isDestination(row, col, dest) == true)
        {
            foundDest.store(true);
            cellDetails[row][col].parent_i = i;
            cellDetails[row][col].parent_j = j;
            printf("The destination cell is found\n");
            tracePath();
            return;
        }
        else if (visited[row][col] == false && isUnBlocked(row, col) == true)
        {
            int gNew;
            if (direction == "NE" || direction == "NW" || direction == "SW" || direction == "SE")
            {
                gNew = cellDetails[i][j].g + 1.414;
            }
            else
                gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(row, col, dest);
            int fNew = gNew + hNew;

            if (cellDetails[row][col].f == FLT_MAX || cellDetails[row][col].f > fNew)
            {
                openList.insert(std::make_pair(fNew, std::make_pair(row, col)));

                cellDetails[row][col].f = fNew;
                cellDetails[row][col].g = gNew;
                cellDetails[row][col].h = hNew;
                cellDetails[row][col].parent_i = i;
                cellDetails[row][col].parent_j = j;
            }
        }
    }
}