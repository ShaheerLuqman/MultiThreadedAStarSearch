import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

public class LongestPathInMaze {
    public static void main(String[] args) {
        int[][] maze = {
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
                { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
                { 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1 },
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }
        };

        List<Cell> path = findLongestPath(maze);
        if (path == null) {
            System.out.println("No path possible");
            return;
        }

        printGrid(path, maze);
        for (Cell cell : path)
            System.out.print(cell);
        System.out.println();
        System.out.println("\nThe Length of Path is: " + path.size());
    }

    private static List<Cell> findLongestPath(int[][] maze) {
        Cell start = new Cell(0, 0);
        Cell end = new Cell(maze.length - 1, maze[0].length - 1);
        List<Cell> path = findLongestPath(maze, start, end);
        return path;
    }

    private static List<Cell> findLongestPath(int[][] maze, Cell start, Cell end) {
        List<Cell> result = null;
        int rows = maze.length;
        int cols = maze[0].length;
        if (start.row < 0 || start.col < 0)
            return null;
        if (start.row == rows || start.col == cols)
            return null;
        if (maze[start.row][start.col] == 0)
            return null;
        if (start.equals(end)) {
            List<Cell> path = new ArrayList<Cell>();
            path.add(start);
            return path;
        }
        maze[start.row][start.col] = 0;
        Cell[] nextCells = new Cell[4];
        nextCells[0] = new Cell(start.row + 1, start.col);
        nextCells[2] = new Cell(start.row, start.col + 1);
        nextCells[1] = new Cell(start.row - 1, start.col);
        nextCells[3] = new Cell(start.row, start.col - 1);
        int maxLength = -1;
        for (Cell nextCell : nextCells) {
            List<Cell> path = findLongestPath(maze, nextCell, end);
            if (path != null && path.size() > maxLength) {
                maxLength = path.size();
                path.add(0, start);
                result = path;
            }
        }
        maze[start.row][start.col] = 1;
        if (result == null || result.size() == 0)
            return null;
        return result;
    }

    private static class Cell {
        public int row;
        public int col;

        public Cell(int row, int column) {
            this.row = row;
            this.col = column;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj)
                return true;
            if ((obj == null) || (obj.getClass() != this.getClass()))
                return false;
            Cell cell = (Cell) obj;
            if (row == cell.row && col == cell.col)
                return true;
            return false;
        }

        @Override
        public String toString() {
            return "-> (" + row + "," + col + ") ";
        }
    }

    private static void printMaze(int[][] maze) {
        for (int i = 0; i < maze.length; ++i) {
            for (int j = 0; j < maze[i].length; ++j) {
                System.out.print("    " + maze[i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }

    private static void printGrid(List<Cell> path, int[][] maze) {
        System.out.println();
        System.out.print("  ");
        for (int i = 0; i < maze[0].length; i++) {
            System.out.print(String.format("%2d", i));
        }
        System.out.println();

        for (int i = 0; i < maze.length; i++) {
            System.out.print(String.format("%2d", i));
            for (int j = 0; j < maze[i].length; j++) {
                boolean isPath = false;
                for (Cell cell : path) {
                    if (cell.row == i && cell.col == j) {
                        isPath = true;
                        break;
                    }
                }

                if (isPath) {
                    System.out.print("\033[1;32m██\033[0m"); // Green
                } else if (maze[i][j] == 1) {
                    System.out.print("  ");
                } else {
                    System.out.print("██");
                }
            }
            System.out.println();
        }
        System.out.println();
    }

}
