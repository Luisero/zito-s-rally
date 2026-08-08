# Zito's Rally

A rally game created for studying OpenGL and Jolt Physics.

If you want to contribute, clone the repository using SSH:

```bash
git clone git@github.com:Luisero/zito-s-rally.git
```

If you just want to build and play, you can clone it using HTTP:

```bash
git clone https://github.com/Luisero/zito-s-rally.git
```

## Setup CMake

```bash
cmake -B build -S .
```

## Build

```bash
cmake --build build/
```

## Run

```bash
./build/Zito-s-Rally
```

---

## 🛠️ Technologies and Libraries Used

This project was built using the following C/C++ libraries:

* **[OpenGL 3.3](https://www.opengl.org/)**: Main API for graphics rendering.
* **[SFML](https://www.sfml-dev.org/)**: Window management, OpenGL context creation, and player input handling.
* **[Jolt Physics](https://github.com/jrouwe/JoltPhysics)**: High-performance multithreaded physics engine.
* **[Assimp](https://github.com/assimp/assimp)**: Open Asset Import Library, used to load 3D models into the engine.
* **[GLAD](https://glad.dav1d.de/)**: OpenGL function pointer loader.
* **[Dear ImGui](https://github.com/ocornut/imgui)**: Graphical user interface library used for debug tools and control panels.
* **[stb_image](https://github.com/nothings/stb)**: Lightweight image decoder used for loading textures.
* **[GLM](https://github.com/g-truc/glm)**: Mathematics library focused on graphics, including matrices, vectors, and quaternions.

---

## 🎮 Controls

| Key       | Action                                                                       |
| :-------- | :--------------------------------------------------------------------------- |
| `W` / `S` | Accelerate / Brake (or move the camera forward/backward in Free Camera mode) |
| `A` / `D` | Turn left / right (or move the camera sideways in Free Camera mode)          |
| `Space`   | Move the camera up (Free Camera mode only)                                   |
| `LShift`  | Move the camera down (Free Camera mode only)                                 |
| `C`       | Toggle between Chase Camera and Free Camera                                  |
| `Esc`     | Return to the Main Menu / Exit                                               |
