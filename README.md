# minecraft-clone

A basic Minecraft-style clone written in C using [GLFW](https://www.glfw.org/) for windowing and OpenGL rendering, built with CMake and GCC on Windows.

---

## 🛠 Requirements

- [GCC (MinGW-w64)](https://winlibs.com/)
- [CMake](https://cmake.org/)
- [GLFW](https://github.com/glfw/glfw) (included as a git submodule)

---

## 🚀 Build & Run

### First time setup:
```bash
git submodule update --init --recursive
```

### Build the project
```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc
cmake --build build
```

### Run 
```bash
build\minecraft-clone.exe
```

### Quick Run
```bash
.\run.bat
```