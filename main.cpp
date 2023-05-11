#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <pthread.h>
#include <chrono>
using namespace std;

typedef pair<int, int> Pair;
typedef pair<double, Pair> pPair;
typedef vector<vector<int>> vvi;

#define ROW 20
#define COL 20

vvi grid = {{1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
            {1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1},
            {1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
            {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0},
            {0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
            {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
            {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
            {0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
            {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
            {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1},
            {0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
            {1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
            {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1},
            {0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0}};
Pair src = make_pair(11, 1);
Pair dest = make_pair(5, 0);
class cell
{
public:
    int parent_i, parent_j;
    double f, g, h;
};

bool visited[ROW][COL]{false};
cell cellDetails[ROW][COL];
vector<vector<int>> newGrid;
set<pair<double, pair<int, int>>> openList; // Create an open list having information as <f, <i, j>>
atomic<bool> foundDest(false);              // Atomic boolean flag to signal when the destination is found, initially cleared

// prototypes start
void North(int i, int j);
void NorthEast(int i, int j);
void East(int i, int j);
void SouthEast(int i, int j);
void South(int i, int j);
void SouthWest(int i, int j);
void West(int i, int j);
void NorthWest(int i, int j);
void importGrid();
void printGrid();
void aStarSearch();
void InitializeCellDetails();
void tracePath();
// prototypes end

bool isValid(int row, int col) { return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL); }
bool isUnBlocked(int row, int col) { return grid[row][col] == 1; }
bool isDestination(int row, int col, Pair dest) { return row == dest.first && col == dest.second; }
double calculateHValue(int row, int col, Pair dest) { return ((double)sqrt(pow((row - dest.first), 2) + pow((col - dest.second), 2))); } // Euclidean Distance for hueristic Function

// pthread functions start
void *pThreadInitializeCellDetails(void *arg)
{
    int id = *((int *)arg);
    int start = id * ROW / 4;
    int end = (id + 1) * ROW / 4;

    for (int i = start; i < end; i++)
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

    pthread_exit(NULL);
}
void *pThreadImportGrid(void *arg)
{
    string filename = "grid.csv";

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            vector<int> row;
            stringstream ss(line);
            string value;
            while (getline(ss, value, ','))
                row.push_back(stoi(value));

            // Lock the access to newGrid before updating it
            pthread_mutex_lock((pthread_mutex_t *)arg);
            newGrid.push_back(row);
            pthread_mutex_unlock((pthread_mutex_t *)arg);
        }
        file.close();
    }

    pthread_exit(NULL);
}
struct ThreadArgsNeighbours
{
    int i;
    int j;
};
// pthread functions end
/*
void *NorthThread(void *arg)
{
    ThreadArgsNeighbours *args = static_cast<ThreadArgsNeighbours *>(arg);
    int i = args->i, j = args->j;
    if (isValid(i - 1, j) == true)
    {
        if (isDestination(i - 1, j, dest) == true)
        {
            cellDetails[i - 1][j].parent_i = i;
            cellDetails[i - 1][j].parent_j = j;
            printf("The destination cell is found\n");
            tracePath();
            foundDest.store(true);
            return NULL;
        }
        else if (visited[i - 1][j] == false && isUnBlocked(i - 1, j) == true)
        {
            int gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(i - 1, j, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i - 1, j)));

                cellDetails[i - 1][j].f = fNew;
                cellDetails[i - 1][j].g = gNew;
                cellDetails[i - 1][j].h = hNew;
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
            }
        }
    }
    return NULL;
}
*/
int main()
{
    auto start = chrono::high_resolution_clock::now();

    // importGrid();
    InitializeCellDetails();
    // printGrid();

    aStarSearch();

    { // Calculating End Time
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        cout << "Execution time: " << duration.count() << " microseconds" << endl;
    }

    return 0;
}

// Functions Implementation
void aStarSearch()
{
    { // conditions to check before starting
        if (isValid(src.first, src.second) == false)
        {
            cout << "Source is invalid\n";
            return;
        }
        if (isValid(dest.first, dest.second) == false)
        {
            cout << "Destination is invalid\n";
            return;
        }
        if (isUnBlocked(dest.first, dest.second) == false)
        {
            cout << "The destination is blocked\n";
            return;
        }
        if (isUnBlocked(src.first, src.second) == false)
        {
            cout << "The Source is blocked\n";
            return;
        }
        if (isDestination(src.first, src.second, dest) == true)
        {
            cout << "We are already at the destination\n";
            return;
        }
    }

    // Initialising the parameters of the starting node on the grid
    int i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
    openList.insert(make_pair(0.0, make_pair(i, j))); // Put the starting cell on the open list and set its 'f' as 0

    vector<pthread_t> threads(8);
    while (!openList.empty())
    {
        pPair p = *openList.begin();

        openList.erase(openList.begin()); // Remove this vertex from the open list

        // Add this vertex to the visited list
        i = p.second.first;
        j = p.second.second;
        visited[i][j] = true;

        double gNew, hNew, fNew;

        // 1st Successor (North)
        North(i, j);
        if (foundDest)
            return;

        // 2nd Successor (South)
        South(i, j);
        if (foundDest)
            return;

        // 3rd Successor (East)
        East(i, j);
        if (foundDest)
            return;

        // 4th Successor (West)
        West(i, j);
        if (foundDest)
            return;

        // 5th Successor (North-East)
        NorthEast(i, j);
        if (foundDest)
            return;

        // 6th Successor (North-West)
        NorthWest(i, j);
        if (foundDest)
            return;

        // 7th Successor (South-East)
        SouthEast(i, j);
        if (foundDest)
            return;

        // 8th Successor (South-West)
        SouthWest(i, j);
        if (foundDest)
            return;
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to blockages)

    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return;
}
void InitializeCellDetails()
{
    pthread_t threads[4];
    int thread_ids[4];
    for (int i = 0; i < 4; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, pThreadInitializeCellDetails, (void *)&thread_ids[i]);
    }
    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);
}
void tracePath(Pair dest)
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
    while (!Path.empty())
    {
        Pair p = Path.top();
        Path.pop();
        cout << "-> (" << p.first << "," << p.second << ") ";
        count++;
    }
    cout << endl
         << "The Length of Path is: " << count - 1 << endl;

    return;
}
void importGrid()
{
    // Create a mutex for accessing newGrid
    pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

    // Create a thread for reading the CSV file
    pthread_t thread;
    pthread_create(&thread, NULL, pThreadImportGrid, &myMutex);

    // Wait for the thread to finish and join it
    pthread_join(thread, NULL);

    // Access newGrid after the thread has finished
    for (int i = 0; i < newGrid.size(); i++)
    {
        for (int j = 0; j < newGrid[i].size(); j++)
            cout << newGrid[i][j] << " ";
        cout << endl;
    }

    // Destroy the mutex
    pthread_mutex_destroy(&myMutex);

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
void North(int i, int j)
{
    if (isValid(i - 1, j) == true)
    {
        if (isDestination(i - 1, j, dest) == true)
        {
            cellDetails[i - 1][j].parent_i = i;
            cellDetails[i - 1][j].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }
        else if (visited[i - 1][j] == false && isUnBlocked(i - 1, j) == true)
        {
            int gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(i - 1, j, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i - 1, j)));

                cellDetails[i - 1][j].f = fNew;
                cellDetails[i - 1][j].g = gNew;
                cellDetails[i - 1][j].h = hNew;
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
            }
        }
    }
}
void NorthEast(int i, int j)
{
    if (isValid(i - 1, j + 1) == true)
    {

        if (isDestination(i - 1, j + 1, dest) == true)
        {

            cellDetails[i - 1][j + 1].parent_i = i;
            cellDetails[i - 1][j + 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i - 1][j + 1] == false && isUnBlocked(i - 1, j + 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.414;
            int hNew = calculateHValue(i - 1, j + 1, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i - 1][j + 1].f == FLT_MAX || cellDetails[i - 1][j + 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i - 1, j + 1)));

                cellDetails[i - 1][j + 1].f = fNew;
                cellDetails[i - 1][j + 1].g = gNew;
                cellDetails[i - 1][j + 1].h = hNew;
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
            }
        }
    }
}
void East(int i, int j)
{
    if (isValid(i, j + 1) == true)
    {

        if (isDestination(i, j + 1, dest) == true)
        {

            cellDetails[i][j + 1].parent_i = i;
            cellDetails[i][j + 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i][j + 1] == false && isUnBlocked(i, j + 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(i, j + 1, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i, j + 1)));

                cellDetails[i][j + 1].f = fNew;
                cellDetails[i][j + 1].g = gNew;
                cellDetails[i][j + 1].h = hNew;
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
            }
        }
    }
}
void SouthEast(int i, int j)
{
    if (isValid(i + 1, j + 1) == true)
    {

        if (isDestination(i + 1, j + 1, dest) == true)
        {

            cellDetails[i + 1][j + 1].parent_i = i;
            cellDetails[i + 1][j + 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i + 1][j + 1] == false && isUnBlocked(i + 1, j + 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.414;
            int hNew = calculateHValue(i + 1, j + 1, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i + 1][j + 1].f == FLT_MAX || cellDetails[i + 1][j + 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i + 1, j + 1)));

                cellDetails[i + 1][j + 1].f = fNew;
                cellDetails[i + 1][j + 1].g = gNew;
                cellDetails[i + 1][j + 1].h = hNew;
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
            }
        }
    }
}
void South(int i, int j)
{
    if (isValid(i + 1, j) == true)
    {

        if (isDestination(i + 1, j, dest) == true)
        {

            cellDetails[i + 1][j].parent_i = i;
            cellDetails[i + 1][j].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i + 1][j] == false && isUnBlocked(i + 1, j) == true)
        {
            int gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(i + 1, j, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i + 1, j)));

                cellDetails[i + 1][j].f = fNew;
                cellDetails[i + 1][j].g = gNew;
                cellDetails[i + 1][j].h = hNew;
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
            }
        }
    }
}
void SouthWest(int i, int j)
{
    if (isValid(i + 1, j - 1) == true)
    {

        if (isDestination(i + 1, j - 1, dest) == true)
        {

            cellDetails[i + 1][j - 1].parent_i = i;
            cellDetails[i + 1][j - 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i + 1][j - 1] == false && isUnBlocked(i + 1, j - 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.414;
            int hNew = calculateHValue(i + 1, j - 1, dest);
            int
                fNew = gNew + hNew;

            if (cellDetails[i + 1][j - 1].f == FLT_MAX || cellDetails[i + 1][j - 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i + 1, j - 1)));

                cellDetails[i + 1][j - 1].f = fNew;
                cellDetails[i + 1][j - 1].g = gNew;
                cellDetails[i + 1][j - 1].h = hNew;
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
            }
        }
    }
}
void West(int i, int j)
{
    if (isValid(i, j - 1) == true)
    {

        if (isDestination(i, j - 1, dest) == true)
        {

            cellDetails[i][j - 1].parent_i = i;
            cellDetails[i][j - 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i][j - 1] == false && isUnBlocked(i, j - 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.0;
            int hNew = calculateHValue(i, j - 1, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i, j - 1)));

                cellDetails[i][j - 1].f = fNew;
                cellDetails[i][j - 1].g = gNew;
                cellDetails[i][j - 1].h = hNew;
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
            }
        }
    }
}
void NorthWest(int i, int j)
{
    if (isValid(i - 1, j - 1) == true)
    {

        if (isDestination(i - 1, j - 1, dest) == true)
        {

            cellDetails[i - 1][j - 1].parent_i = i;
            cellDetails[i - 1][j - 1].parent_j = j;
            printf("The destination cell is found\n");
            tracePath(dest);
            foundDest.store(true);
            return;
        }

        else if (visited[i - 1][j - 1] == false && isUnBlocked(i - 1, j - 1) == true)
        {
            int gNew = cellDetails[i][j].g + 1.414;
            int hNew = calculateHValue(i - 1, j - 1, dest);
            int fNew = gNew + hNew;

            if (cellDetails[i - 1][j - 1].f == FLT_MAX || cellDetails[i - 1][j - 1].f > fNew)
            {
                openList.insert(make_pair(
                    fNew, make_pair(i - 1, j - 1)));

                cellDetails[i - 1][j - 1].f = fNew;
                cellDetails[i - 1][j - 1].g = gNew;
                cellDetails[i - 1][j - 1].h = hNew;
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
            }
        }
    }
}