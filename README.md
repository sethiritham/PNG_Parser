# PNG Parser & Image Editor

A low-level PNG parser and lightweight image editor written in C++ with OpenGL + GLFW + Dear ImGui.  
This project focuses on understanding image formats, binary parsing, and real-time image manipulation from scratch.

---

## Features

- Custom PNG file parsing
- zlib decompression for PNG image data
- Real-time image display using OpenGL
- GUI built with Dear ImGui
- Brightness / Contrast / Saturation editing (work in progress)
- Minimal dependency usage
- Educational, low-level implementation

---

## Project Structure

```

PNG_Parser/
├── src/
│ ├── main.cpp
│ ├── api/
│ ├── parser/
│ │ └── PNGloader.cpp
│ └── vendor/
│ └── imgui/
├── build/ (generated)
├── editor (generated executable)
└── Makefile

```
---

## Dependencies

Make sure these are installed on your system:

- g++
- OpenGL
- GLFW
- zlib
- dl (usually comes with libc)

On Ubuntu / Pop!_OS:

```bash
sudo apt install g++ libglfw3-dev libgl1-mesa-dev zlib1g-dev
```
## Build Instructions
Always run make from the project root, not inside src.

**make**
This will compile everything and produce:
./editor
To run:
```
./editor
```

To clean build files:
```
make clean
```

## How the Makefile Works
Compiles each .cpp file into an object file
Links them together with:
-lz for PNG decompression
-lglfw for windowing
-lGL for OpenGL
-ldl for dynamic linking

Keeps build fast by recompiling only changed files


Why This Project Exists
This is not just an image viewer.
It is a learning project to understand:

How PNG files are structured

How compression works

How pixels are manipulated mathematically

How OpenGL displays raw image buffers

How real C++ projects are built without heavy frameworks


Example Build Command (Without Makefile)
This is what the Makefile automates:
```
g++ main.cpp parser/PNGloader.cpp \
vendor/imgui/imgui.cpp vendor/imgui/imgui_draw.cpp vendor/imgui/imgui_tables.cpp vendor/imgui/imgui_widgets.cpp \
vendor/imgui/imgui_impl_glfw.cpp vendor/imgui/imgui_impl_opengl3.cpp \
-lz -lglfw -lGL -ldl \
-o editor
```
Future Plans
- Full image editing pipeline
- Image format conversion pipeline
- Support for more image formats
- Save edited PNG output

Author
**Ritham Sethi**
Learning systems programming, graphics, and file format internals through hands-on implementation.
