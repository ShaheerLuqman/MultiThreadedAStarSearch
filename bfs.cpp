// https://www.tutorialspoint.com/shortest-path-in-binary-matrix-in-cplusplus

#include <bits/stdc++.h>
using namespace std;
int d[8][2] = {{1, 1}, {1, -1}, {-1, 1}, {1, 0}, {0, 1}, {-1, -1}, {0, -1}, {-1, 0}};
struct point
{
    int x, y, c;
    point(int a, int b, int z)
    {
        x = a;
        y = b;
        c = z;
    }
};
class Solution
{
public:
    int shortestPathBinaryMatrix(vector<vector<int>> &grid)
    {
        queue<point> q;
        int n = grid.size();
        if (!grid[0][0])
        {
            q.push(point(0, 0, 1));
            grid[0][0] = 1;
        }
        while (!q.empty())
        {
            point curr = q.front();
            q.pop();
            int x = curr.x;
            int y = curr.y;
            int c = curr.c;
            if (x == n - 1 && y == n - 1)
                return c;
            c++;
            for (int i = 0; i < 8; i++)
            {
                int X = x + d[i][0];
                int Y = y + d[i][1];
                if (X >= 0 && X < n && Y >= 0 && Y < n &&
                    !grid[X][Y])
                {
                    grid[X][Y] = 1;
                    q.push(point(X, Y, c));
                }
            }
        }
        return -1;
    }
};

int main()
{
    auto start = chrono::high_resolution_clock::now();
    vector<vector<int>> grid = {{0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0},
                                {0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
                                {0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1},
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

    Solution ob;
    cout << (ob.shortestPathBinaryMatrix(grid)) << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Execution time: " << duration.count() << " microseconds" << endl;
}