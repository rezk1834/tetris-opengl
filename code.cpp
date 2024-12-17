#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const int CELL_SIZE = 30;

int score = 0;
bool gameOver = false;

struct Block {
    int x, y;
};

struct Tetromino {
    std::vector<Block> blocks;
    float color[3];
};

// Tetromino definitions
Tetromino tetrominoes[] = {
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {1.0, 0.0, 0.0}}, // Line
    {{{0, 0}, {0, 1}, {1, 1}, {1, 0}}, {0.0, 1.0, 0.0}}, // Square
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {0.0, 0.0, 1.0}}, // T-shape
    {{{0, 1}, {1, 1}, {1, 0}, {2, 0}}, {1.0, 1.0, 0.0}}, // Z-shape
    {{{1, 1}, {2, 1}, {0, 0}, {1, 0}}, {1.0, 0.0, 1.0}}, // S-shape
};

std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
Tetromino currentTetromino;
int tetrominoX = GRID_WIDTH / 2 - 2, tetrominoY = GRID_HEIGHT - 1;

// Function prototypes
bool checkCollision(int dx, int dy);
void rotateTetromino();

// Draw a single cell
void drawCell(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x * CELL_SIZE, y * CELL_SIZE);
    glVertex2f((x + 1) * CELL_SIZE, y * CELL_SIZE);
    glVertex2f((x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
    glVertex2f(x * CELL_SIZE, (y + 1) * CELL_SIZE);
    glEnd();
}

// Draw the game grid
void drawGrid() {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    for (int i = 0; i <= GRID_WIDTH; i++) {
        glVertex2f(i * CELL_SIZE, 0);
        glVertex2f(i * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        glVertex2f(0, i * CELL_SIZE);
        glVertex2f(GRID_WIDTH * CELL_SIZE, i * CELL_SIZE);
    }
    glEnd();

    // Draw border
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    glVertex2f(GRID_WIDTH * CELL_SIZE, 0);
    glVertex2f(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE);
    glVertex2f(0, GRID_HEIGHT * CELL_SIZE);
    glEnd();
}

// Draw the current tetromino
void drawTetromino() {
    for (Block block : currentTetromino.blocks) {
        drawCell(tetrominoX + block.x, tetrominoY - block.y, currentTetromino.color[0], currentTetromino.color[1], currentTetromino.color[2]);
    }
}

// Draw the grid cells
void drawGridCells() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                drawCell(x, y, 0.7, 0.7, 0.7); // Fixed cells
            }
        }
    }
}

// Rotate the tetromino
void rotateTetromino() {
    for (Block& block : currentTetromino.blocks) {
        int temp = block.x;
        block.x = -block.y;
        block.y = temp;
    }
    // Check for collision after rotation; revert if necessary
    if (checkCollision(0, 0)) {
        for (Block& block : currentTetromino.blocks) {
            int temp = block.x;
            block.x = block.y;
            block.y = -temp;
        }
    }
}

// Check for collision
bool checkCollision(int dx, int dy) {
    for (Block block : currentTetromino.blocks) {
        int nx = tetrominoX + block.x + dx;
        int ny = tetrominoY - block.y + dy;
        if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || grid[ny][nx]) {
            return true;
        }
    }
    return false;
}

// Merge tetromino into the grid
void mergeTetromino() {
    for (Block block : currentTetromino.blocks) {
        int x = tetrominoX + block.x;
        int y = tetrominoY - block.y;
        grid[y][x] = 1;
    }
}

// Clear completed lines
void clearLines() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        bool fullLine = true;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (!grid[y][x]) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            score += 100;
            for (int ny = y; ny < GRID_HEIGHT - 1; ny++) {
                grid[ny] = grid[ny + 1];
            }
            grid[GRID_HEIGHT - 1] = std::vector<int>(GRID_WIDTH, 0);
        }
    }
}

// Spawn a new tetromino
void spawnTetromino() {
    currentTetromino = tetrominoes[rand() % 5];
    tetrominoX = GRID_WIDTH / 2 - 2;
    tetrominoY = GRID_HEIGHT - 1;
    if (checkCollision(0, 0)) {
        gameOver = true;
    }
}

// Game logic
void update(int value) {
    if (!gameOver) {
        if (!checkCollision(0, -1)) {
            tetrominoY--;
        } else {
            mergeTetromino();
            clearLines();
            spawnTetromino();
        }
        glutTimerFunc(500, update, 0);
        glutPostRedisplay();
    }
}

// Render the scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawGridCells();
    drawTetromino();

    // Display score
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(5, GRID_HEIGHT + 1);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    if (gameOver) {
        glRasterPos2i(3, GRID_HEIGHT / 2);
        std::string gameOverText = "Game Over!";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}

// Handle keyboard input
void handleInput(unsigned char key, int x, int y) {
    if (!gameOver) {
        if (key == 'a' && !checkCollision(-1, 0)) {
            tetrominoX--;
        } else if (key == 'd' && !checkCollision(1, 0)) {
            tetrominoX++;
        } else if (key == 's' && !checkCollision(0, -1)) {
            tetrominoY--;
        } else if (key == 'w') {
            rotateTetromino();
        }
        glutPostRedisplay();
    }
}

// Initialize OpenGL settings
void initialize() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GRID_WIDTH * CELL_SIZE, 0, (GRID_HEIGHT + 1) * CELL_SIZE);
    srand(time(0));
    spawnTetromino();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(GRID_WIDTH * CELL_SIZE, (GRID_HEIGHT + 1) * CELL_SIZE);
    glutCreateWindow("Tetris");
    initialize();
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleInput);
    glutTimerFunc(500, update, 0);
    glutMainLoop();
    return 0;
}
