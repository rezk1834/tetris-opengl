```markdown
# Tetris Game in OpenGL

A simple and interactive Tetris game built using **C++** and **OpenGL**. This project demonstrates fundamental computer graphics concepts such as shape rendering, animations, and user interaction.

## Features

- **Interactive Gameplay**:
  - Move blocks left, right, or down.
  - Rotate tetrominoes for better placement.
- **Dynamic UI**:
  - Visual grid with borders for clarity.
  - Real-time score display.
- **Game Mechanics**:
  - Automatically detects and clears completed lines.
  - Ends the game when the grid is full.

## Controls

- **A**: Move tetromino left.
- **D**: Move tetromino right.
- **S**: Move tetromino down faster.
- **W**: Rotate tetromino clockwise.

## Requirements

- C++ Compiler (e.g., GCC, MinGW)
- OpenGL and GLUT libraries installed on your system.

## Setup and Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/rezk1834/tetris-opengl.git
   cd tetris-opengl
   ```

2. **Compile the code**:
   Use any C++ compiler with OpenGL and GLUT support. For example, with `g++`:
   ```bash
   g++ tetris.cpp -o tetris -lGL -lGLU -lglut
   ```

3. **Run the game**:
   ```bash
   ./tetris
   ```

## How It Works

1. **Game Grid**:
   - The game uses a 10x20 grid to display tetrominoes.
   - Each tetromino consists of 4 blocks, which can move or rotate.

2. **Collision Detection**:
   - Ensures that tetrominoes do not overlap or move out of bounds.

3. **Line Clearing**:
   - A row is cleared when it is completely filled with blocks.
   - The score increases by 100 points per cleared row.

4. **Game Over**:
   - The game ends when no space is available for a new tetromino.




## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- **OpenGL** for graphics rendering.
- Inspiration from the classic **Tetris** game.
- Built as part of a Computer Graphics course project.

---

**Contributions** are welcome! Feel free to fork this repository, make improvements, and submit pull requests.
```

### Key Points of the README:
1. **Comprehensive Feature List**: Explains gameplay mechanics and features.
2. **Setup Instructions**: Covers compiling and running the game.
3. **Controls**: Lists key bindings for player interaction.
4. **Future Enhancements**: Outlines possible improvements for contributors.
5. **Screenshots**: Placeholder for adding images of the game.
6. **License**: Encourages clarity regarding project usage rights.
```