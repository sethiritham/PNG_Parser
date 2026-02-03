# PNG Parser & Image Editor

A small, low-level PNG parser and lightweight image editor written in modern C++ with OpenGL (via GLFW) and Dear ImGui. This project is educational: it implements PNG parsing, DEFLATE/zlib handling, PNG filtering/unfiltering, and a minimal real-time image editor/display pipeline from scratch.

This README explains what this repository contains, how to build and run the editor, limitations, and next steps.

---

## Highlights / Features

- Custom PNG parsing (reads PNG signature, IHDR, IDAT, IEND)
- Concatenates IDAT chunks and performs zlib/DEFLATE decompression
- Implements PNG filter unfiltering (including Paeth predictor)
- Minimal GUI built with Dear ImGui (real-time display + basic editing controls)
- OpenGL texture upload and display (via GLFW + OpenGL)
- Save/export support implemented using a small compression helper (miniz)
- Minimal external dependencies — intended for learning and experimentation

---

## Repository layout

- src/
  - main.cpp                 — application entry (initializes UI and launches editor)
  - parser/
    - PNGloader.h/.cpp       — PNG parsing, zlib/IDAT handling, unfilter/reconstruct
  - generator/
    - png_generator.hpp/.cpp — functions for writing PNG files (IHDR, IDAT, IEND)
    - comp_filter_png.hpp    — helper(s) used when composing compressed PNG data
    - bmp_generator.hpp      — (aux/experimental) BMP-writing helpers
  - graphics/
    - UI.hpp/.cpp            — GLFW + ImGui initialization and teardown
    - editor.hpp/.cpp        — editor window/controls and processing flow
    - texture.hpp/.cpp       — OpenGL texture creation / update
  - vendor/
    - imgui/                 — Dear ImGui sources (bundled)
    - miniz.h                — single-file miniz header (used for compression)
  - utils/
    - utils.hpp              — small utilities (big-endian reads, helpers)
- build/                      — generated dependency files (from Makefile)
- Makefile
- assets/                     — example images used by the editor (e.g. assets/sculpture.png)

---

## Requirements / Dependencies

The project targets Linux (or systems with the equivalent libs available). Install the following development packages:

- g++ (C++ compiler with at least C++11 support)
- OpenGL (GL headers / lib)
- GLFW (development headers / library)
- zlib (development headers / library)
- dl (usually present with libc; used at link time)
- make (to use the included Makefile)

On Debian / Ubuntu / Pop!_OS you can install the required packages with:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libgl1-mesa-dev zlib1g-dev
```

---

## Build

Always run `make` from the project root (not inside `src/`).

To build:

```bash
make
```

This will compile the sources and produce the executable `./editor` in the project root.

To clean build artifacts:

```bash
make clean
```

If you prefer to compile manually (this is essentially what the Makefile automates):

```bash
g++ src/main.cpp src/parser/PNGloader.cpp \
    src/generator/png_generator.cpp \
    src/graphics/editor.cpp src/graphics/texture.cpp src/graphics/UI.cpp \
    src/vendor/imgui/imgui.cpp src/vendor/imgui/imgui_draw.cpp \
    src/vendor/imgui/imgui_tables.cpp src/vendor/imgui/imgui_widgets.cpp \
    src/vendor/imgui/imgui_impl_glfw.cpp src/vendor/imgui/imgui_impl_opengl3.cpp \
    -I src -I src/vendor/imgui -I src/vendor \
    -lz -lglfw -lGL -ldl -o editor
```

(Adjust source list as needed if additional files are added; using the Makefile is recommended.)

---

## Run

After building, run:

```bash
./editor
```

By default, `src/main.cpp` loads `assets/sculpture.png`. The editor's UI provides basic controls (brightness/contrast/saturation are in-progress). Use the GUI to load other images or extend the UI for additional controls.

---

## What the parser supports / Known limitations

- The parser currently supports 8-bit PNG images only (bit depth = 8). Other bit depths are not supported.
- Supported color types:
  - Color type 2 — Truecolor (RGB, 3 channels)
  - Color type 6 — Truecolor with alpha (RGBA, 4 channels)
  - Other PNG color types (palette, grayscale, etc.) are not implemented.
- IDAT chunks are concatenated into a zlib stream, then decompressed via zlib/miniz.
- After decompression, PNG filter bytes are unfiltered per scanline (including Paeth predictor).
- This is primarily an educational implementation and is not hardened for malformed PNGs or security-hardened parsing for untrusted inputs.
- If you need robust, production-grade PNG support, use a well-tested library (libpng). This project is for learning and experimentation.

---

## High-level implementation notes

- PNG signature validation is performed first in `PNGloader`.
- `processChunks` reads chunk length/type, processes IHDR to extract width/height/bit depth/color type, concatenates IDAT chunk bodies into `image.zlibStream`, and stops at IEND.
- Decompression produces filtered raw scanline data; `reconstructImage` removes PNG filters and produces pixel buffer(s).
- `png_generator` contains helpers to write IHDR, compress filtered image data (with miniz/zlib), write IDAT, and write IEND — enabling saving edited images back to PNG.
- `graphics/*` modules manage the GLFW window, ImGui UI, and uploading pixel buffers to OpenGL textures for display.

---

## Example: programmatic usage (library-style)

The repository's code is currently organized as an application, but the parser and generator are contained in separate headers and source files so they can be used from other programs. Example usage (conceptual):

- Create an Image struct instance.
- Call PNGloader::Load("file.png", image) to populate image.data/pixels.
- Manipulate `image.editedPixels` or `image.pixels`.
- Call save_png("out.png", edited_image) to write changes.

Refer to:
- `src/parser/PNGloader.h` and `src/parser/PNGloader.cpp`
- `src/generator/png_generator.hpp` and `src/generator/png_generator.cpp`

---

## Development notes / Ideas & TODOs

Planned and suggested improvements:

- Add support for additional PNG bit depths and color types (palette/grayscale).
- Add robust error handling and validation for malformed PNGs.
- Add unit tests for the parser (compare against libpng-read results).
- Improve UI: implement full editing pipeline (brightness/contrast/saturation controls), history/undo, and save/export dialogs.
- Improve performance for large images (optimize decompression/unfiltering and OpenGL upload).
- Add a license (e.g., MIT) and contribution guidelines if you want others to contribute.

---

## Contributing

Contributions are welcome. A suggested workflow:

- Fork the repository.
- Create a branch for your feature/fix.
- Open a pull request with a clear description and, if applicable, a small test image showing behavior.

Please open issues for bugs, feature requests, or to discuss design/approach.

---

## License

No license file is included in this repository as of this README. If you want to make this project open-source, consider adding a LICENSE file (MIT, Apache-2.0, etc.) and update this section.

---

## Author

Ritham Sethi (GitHub: @sethiritham)

Learning systems programming, graphics, and file-format internals through hands-on implementation.

---
