#include <bits/stdc++.h>
using namespace std;

int d[8][2] = {{1, 1}, {1, -1}, {-1, 1}, {1, 0}, {0, 1}, {-1, -1}, {0, -1}, {-1, 0}};

int shortestPathBinaryMatrix(vector<vector<int>> &grid, pair<int, int> src, pair<int, int> dest)
{
    int n = grid.size();
    vector<vector<int>> parent(n, vector<int>(n, -1));
    vector<vector<int>> distance(n, vector<int>(n, INT_MAX));
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;
    if (!grid[src.first][src.second])
    {
        pq.push({1, {src.first, src.second}});
        distance[src.first][src.second] = 1;
    }
    while (!pq.empty())
    {
        pair<int, pair<int, int>> curr = pq.top();
        pq.pop();
        int x = curr.second.first;
        int y = curr.second.second;
        int c = curr.first;
        if (x == dest.first && y == dest.second)
        {
            break;
        }
        if (c > distance[x][y])
        {
            continue;
        }
        for (int i = 0; i < 8; i++)
        {
            int X = x + d[i][0];
            int Y = y + d[i][1];
            if (X >= 0 && X < n && Y >= 0 && Y < n && !grid[X][Y])
            {
                int new_distance = c + 1;
                if (new_distance < distance[X][Y])
                {
                    distance[X][Y] = new_distance;
                    parent[X][Y] = x * n + y;
                    pq.push({new_distance, {X, Y}});
                }
            }
        }
    }
    if (parent[dest.first][dest.second] == -1)
    {
        return -1;
    }
    vector<int> path;
    int curr = dest.first * n + dest.second;
    while (curr != -1)
    {
        int x = curr / n;
        int y = curr % n;
        path.push_back(curr);
        curr = parent[x][y];
    }
    reverse(path.begin(), path.end());
    for (int p : path)
    {
        int x = p / n;
        int y = p % n;
        cout << "(" << x << "," << y << ") ";
    }
    cout << endl;
    return distance[dest.first][dest.second];
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    vector<vector<int>> grid =
        {
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
            {1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0},
            {0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0},
            {0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
            {0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1},
            {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
            {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
            {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0},
            {1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0},
            {1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1},
            {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0},
            {0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0},
            {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0},
            {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1}};

    pair<int, int> src = make_pair(11, 1);
    pair<int, int> dest = make_pair(5, 0);
    cout << shortestPathBinaryMatrix(grid, src, dest) - 1
         << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
}
