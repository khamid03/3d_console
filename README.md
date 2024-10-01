# 3D Console Raycasting Engine

A simple **3D raycasting engine** implemented in C++ that renders a first-person perspective within the Windows console. The engine simulates a 3D-like experience using 2D maps and runs directly in the Windows terminal.

## Features

- **3D Raycasting**: Simulates a 3D environment using 2D calculations.
- **First-person Movement**: Allows the player to move forward, backward, and rotate left or right.
- **Collision Detection**: Players cannot walk through walls.
- **Real-time Feedback**: Displays player’s position, view direction, and frames per second (FPS).
- **Top-down Mini-Map**: Renders a simple ASCII map in the console, showing the player’s current position and orientation.

## Requirements

### Operating System
- **Windows**: The program relies on the Windows API (`windows.h`) for console handling and user input.

### Software
- **C++ Compiler**:
  - **MinGW (g++)** for compiling on Windows.
  - **Microsoft Visual Studio** for compiling and running in the IDE.
  - Ensure that the compiler supports **C++11** or newer.

### Console Window Settings
- **Console Window Size**: The program requires the console window to be set to **200 characters wide** and **50 characters high** for proper rendering.
  - You can adjust the size of the Command Prompt or PowerShell window manually, or use this command in the terminal:
    ```bash
    mode con: cols=200 lines=50
    ```

- **Font**: For the best experience, use a font that supports block characters, such as **Consolas** or **Lucida Console**.

### Memory & CPU
- The project is lightweight, and it should run smoothly on most modern hardware.

## Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/your-repo/3dConsoleRaycaster.git
    cd 3dConsoleRaycaster
    ```

2. **Compile the program**:
    - Using **g++** (MinGW):
      ```bash
      g++ 3dConsole.cpp -o 3dConsole -std=c++11
      ```
    - Using **Visual Studio**:
      - Open the project in the IDE.
      - Build the project directly from Visual Studio.

3. **Run the executable**:
    ```bash
    ./3dConsole
    ```

4. **Set the console window size** to 200x50 if it hasn't been set already.

## Usage

Once the program is running, you'll see a first-person 3D view of a maze rendered in the console. The player can navigate through the environment using the keyboard.

### Controls

- **W**: Move forward
- **S**: Move backward
- **A**: Rotate left
- **D**: Rotate right
- **ESC**: Exit the program

The player's orientation is shown on the mini-map using:
- `v`: Facing downward
- `>`: Facing right
- `^`: Facing upward
- `<`: Facing left

## Map Customization

The map is hardcoded into the source code and can be customized by editing the `map` string in the following section:

```cpp
wstring map;
map += L"################";
map += L"#..............#";
map += L"#####...###....#";
map += L"#..............#";
map += L"################";
```

Each character represents one tile:
- `#`: Wall
- `.`: Walkable space

You can modify this map to create your own custom levels by adjusting the characters.

## Technical Overview

### Raycasting Algorithm

The engine uses **raycasting** to simulate a 3D environment by casting rays from the player's viewpoint into the 2D map. The distance to walls is calculated, and the height of the walls is rendered based on the distance, making closer walls appear taller and farther walls shorter.

### Real-time Rendering

The console output is managed using the **Windows console screen buffer**. The screen is updated in real-time to provide smooth movement and interaction.

### Player Movement and Collision

The player can move and rotate based on keyboard input. Basic collision detection is implemented to ensure the player cannot walk through walls.

### FPS Calculation

The frames per second (FPS) is calculated using the `chrono` library for time measurement, providing a consistent frame rate display in real-time.

## Known Issues

- **Platform Dependency**: The code is Windows-specific due to its reliance on the `windows.h` library.
- **Terminal Font**: Some fonts may not display the shading characters correctly. It's recommended to use **Consolas** or **Lucida Console**.
- **Performance**: Performance may degrade if the screen resolution (width and height) is increased significantly, as the rendering algorithm is basic.

## Contributing

Contributions are welcome! If you have ideas for new features, optimizations, or bug fixes, feel free to fork the repository and submit a pull request.

### Potential Improvements
- Adding textures or more complex shading for walls.
- Expanding the map or adding interactive elements.
- Porting the engine to other platforms (e.g., Linux) by replacing the Windows-specific API with a cross-platform alternative.

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for details.
