void printGrid(stack<Pair> Path)
{
    {
        if (ROW <= 30)
            usleep(1000);
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
            else if (cellDetails[i][j].f != FLT_MAX)
                cout << "\033[1;34m"
                     << "██"
                     << "\033[0m"; // Blue
            else if (grid[i][j] == 1)
                cout << "  ";
            else
                cout << "██";
        }
        cout << endl;
    }
    cout << endl;
}