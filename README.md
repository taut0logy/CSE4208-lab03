# 3D Computer Laboratory (OpenGL)
**Assignment for Lab 03 (Group A1)**

A fully interactive 3D laboratory environment built with C++ and OpenGL 3.3.

## Features
- **Detailed Environment**: A realistic lab with desks, chairs, PCs, whiteboard, and ceiling details.
- **Interactive Elements**:
  - **Fan**: Rotating ceiling fan with octagonal motor housing.
  - **Door**: Swings inward.
  - **Windows**: Sliding glass windows with oak frames (cutouts in walls allow seeing through).
  - **Lights**: Toggleable ceiling lights.
- **Advanced Camera**:
  - WASD + Vertical movement.
  - Mouse look support.
  - Special Orbit and Bird's Eye view modes.

## Controls

### Mouse
- **Move**: Look around
- **Scroll**: Move forward/backward

### Keyboard
| Key | Action |
|:---:|:---|
| **W / S** | Move Forward / Backward |
| **A / D** | Move Left / Right |
| **E / R** | Move Up / Down |
| **X** | Pitch Interact (Look Up/Down manual) |
| **Y** | Yaw Interact (Look Left/Right manual) |
| **Z** | Roll Camera |
| **G** | Toggle **Fan** |
| **L** | Toggle **Lights** |
| **O** | Open/Close **Door** |
| **P** | Open/Close **Windows** |
| **F** | **Orbit** View (around center) |
| **B** | **Bird's Eye** View |
| **ESC** | Exit |

## Build Instructions (Visual Studio)
1. Open `lab03.vcxproj` in **Visual Studio 2022**.
2. Select **Debug** or **Release** configuration.
3. Select **x64** platform.
4. Build Solution (**Ctrl+Shift+B**).
5. Run (**F5**).

## Structure
- `main.cpp`: Entry point & Input handling.
- `lab_scene.h`: Scene composition (arrangement of objects).
- `objects.h`: Object definitions (Desks, PCs, Walls, Fan).
- `camera.h`: Camera logic.
- `cube.h`: Primitive cube rendering.
