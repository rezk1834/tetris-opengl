#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Fallback for std::to_string if not available
template <typename T>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Constants
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const int CELL_SIZE = 30;
const int BASE_SPEED = 550; // Fixed speed in milliseconds

// Game variables
int score = 0;
bool gameOver = false;
bool paused = false;

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

// Grid and tetromino state
std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
Tetromino currentTetromino, nextTetromino;
int tetrominoX = GRID_WIDTH / 2 - 2, tetrominoY = GRID_HEIGHT - 1;

// Function prototypes
bool checkCollision(int dx, int dy);
void rotateTetromino();
void drawCell(int x, int y, float r, float g, float b);
void drawGrid();
void drawGridCells();
void drawTetromino();
void drawNextPiece();
void mergeTetromino();
void clearLines();
void spawnTetromino();
void update(int value);
void renderScene();
void handleInput(unsigned char key, int x, int y);
void handleArrowKeys(int key, int x, int y);
void initialize();
void restartGame();

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

// Draw the grid
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
}

// Draw grid cells
void drawGridCells() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x]) {
                drawCell(x, y, 0.7, 0.7, 0.7); // Fixed cells
            }
        }
    }
}

// Draw the current tetromino
void drawTetromino() {
    for (Block block : currentTetromino.blocks) {
        drawCell(tetrominoX + block.x, tetrominoY - block.y, currentTetromino.color[0], currentTetromino.color[1], currentTetromino.color[2]);
    }
}

// Draw the next tetromino
void drawNextPiece() {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(GRID_WIDTH * CELL_SIZE + 20, GRID_HEIGHT * CELL_SIZE - 20);
    std::string nextText = "Next:";
    for (char c : nextText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    for (Block block : nextTetromino.blocks) {
        drawCell(GRID_WIDTH + 2 + block.x, GRID_HEIGHT - 2 - block.y, nextTetromino.color[0], nextTetromino.color[1], nextTetromino.color[2]);
    }
}

// Rotate the tetromino
void rotateTetromino() {
    for (Block& block : currentTetromino.blocks) {
        int temp = block.x;
        block.x = -block.y;
        block.y = temp;
    }
    if (checkCollision(0, 0)) {
        for (Block& block : currentTetromino.blocks) {
            int temp = block.x;
            block.x = block.y;
            block.y = -temp;
        }
    }
}

// Spawn a new tetromino
void spawnTetromino() {
    if (!nextTetromino.blocks.empty()) {
        currentTetromino = nextTetromino;
    }
    else {
        currentTetromino = tetrominoes[rand() % 5];
    }
    nextTetromino = tetrominoes[rand() % 5];
    tetrominoX = GRID_WIDTH / 2 - 2;
    tetrominoY = GRID_HEIGHT - 1;

    if (checkCollision(0, 0)) {
        gameOver = true;
    }
}

// Check for collision
bool checkCollision(int dx, int dy) {
    for (Block block : currentTetromino.blocks) {
        int nx = tetrominoX + block.x + dx;
        int ny = tetrominoY - block.y + dy;
        if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || (ny < GRID_HEIGHT && grid[ny][nx])) {
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
//clearlines function
void clearLines() {
    int linesCleared = 0; // Counter for cleared lines

    // Iterate through all rows in the grid
    for (int y = 0; y < GRID_HEIGHT; ) {
        bool fullLine = true;

        // Check if the current row is full
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (!grid[y][x]) {
                fullLine = false;
                break;
            }
        }

        // If the line is full, remove it
        if (fullLine) {
            linesCleared++; // Increment cleared lines counter
            for (int ny = y; ny < GRID_HEIGHT - 1; ny++) {
                grid[ny] = grid[ny + 1]; // Shift rows above down
            }
            grid[GRID_HEIGHT - 1] = std::vector<int>(GRID_WIDTH, 0); // Clear the topmost row
        }
        else {
            y++; // Only move to the next row if the current row is not cleared
        }
    }

    // Update the score based on the number of lines cleared
    score += linesCleared * 100; // Add 100 points per line
}


// Restart the game
void restartGame() {
    grid = std::vector<std::vector<int>>(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0)); // Reset grid
    score = 0;       // Reset score
    gameOver = false; // Reset game over state
    paused = false;   // Ensure the game is not paused
    spawnTetromino(); // Spawn a new tetromino
    glutTimerFunc(BASE_SPEED, update, 0); // Restart the timer for automatic updates
    glutPostRedisplay(); // Ensure the screen is redrawn
}


// Update game logic
void update(int value) {
    if (!gameOver && !paused) { // Ensure the game is not over or paused
        if (!checkCollision(0, -1)) {
            tetrominoY--; // Move the tetromino down
        }
        else {
            mergeTetromino(); // Fix tetromino to grid
            clearLines();     // Clear completed lines
            spawnTetromino(); // Spawn a new tetromino
        }
        glutPostRedisplay(); // Redraw the scene
    }
    // Always re-register the timer
    if (!gameOver) {
        glutTimerFunc(BASE_SPEED, update, 0);
    }
}


// Render the scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    drawGridCells();
    drawTetromino();
    drawNextPiece();

    // Display "Score" label and the score value under the "Next" sign
    glColor3f(1.0, 1.0, 1.0);
    // "Score" label
    glRasterPos2i(GRID_WIDTH * CELL_SIZE + 10, GRID_HEIGHT * CELL_SIZE - 100); // Adjust position for "Score" label
    std::string scoreLabel = "Score:";
    for (char c : scoreLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Score value
    glRasterPos2i(GRID_WIDTH * CELL_SIZE + 10, GRID_HEIGHT * CELL_SIZE - 130); // Adjust position for the score value
    std::string scoreText = to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Display game-over message if the game is over
    if (gameOver) {
        glRasterPos2i(GRID_WIDTH * CELL_SIZE + 10, GRID_HEIGHT / 2 * CELL_SIZE);
        std::string gameOverText = "Game Over!";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2i(GRID_WIDTH * CELL_SIZE + 10, GRID_HEIGHT / 2 * CELL_SIZE - 20);
        std::string restartText = "Press R to Restart.";
        for (char c : restartText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    // Display pause message if the game is paused
    if (paused) {
        glRasterPos2i(GRID_WIDTH * CELL_SIZE + 10, GRID_HEIGHT / 2 * CELL_SIZE + 20);
        std::string pauseText = "Paused. Press P to Resume.";
        for (char c : pauseText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    glutSwapBuffers();
}


// Handle keyboard input
void handleInput(unsigned char key, int x, int y) {
    if (key == 'p') {
        paused = !paused;
    }
    else if (key == 'r') {
        restartGame();
    }
    glutPostRedisplay();
}

// Handle arrow key input
void handleArrowKeys(int key, int x, int y) {
    if (!gameOver && !paused) {
        if (key == GLUT_KEY_LEFT && !checkCollision(-1, 0)) {
            tetrominoX--;
        }
        else if (key == GLUT_KEY_RIGHT && !checkCollision(1, 0)) {
            tetrominoX++;
        }
        else if (key == GLUT_KEY_DOWN && !checkCollision(0, -1)) {
            tetrominoY--;
        }
        else if (key == GLUT_KEY_UP) {
            rotateTetromino();
        }
        glutPostRedisplay();
    }
}

// Initialize OpenGL settings
void initialize() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GRID_WIDTH * CELL_SIZE + 100, 0, (GRID_HEIGHT + 1) * CELL_SIZE);
    srand(time(0));
    spawnTetromino();
    nextTetromino = tetrominoes[rand() % 5];
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(GRID_WIDTH * CELL_SIZE + 200, (GRID_HEIGHT + 1) * CELL_SIZE);
    glutCreateWindow("Tetris");
    initialize();
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(handleInput);
    glutSpecialFunc(handleArrowKeys);
    glutTimerFunc(BASE_SPEED, update, 0);
    glutMainLoop();
    return 0;
}
