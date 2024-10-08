# Little Vulkan Game Engine

## Overview
This project is a lightweight 3D Vulkan Game Engine developed in C++. It features a complete graphics pipeline and provides efficient handling of rendering tasks, game loops, and user input. The engine is designed for flexibility and performance, leveraging Vulkan's powerful API to deliver advanced graphics techniques.
This project is soley for education. I made this to learn how modern graphics API's work. 

## Features

- **Graphics Pipeline**: Implemented a robust pipeline with 3D transformations, diffuse shading, Phong-Blinn model lighting, and specular lighting.
- **Rendering Techniques**: Supports alpha blending, transparency, and dynamic viewports for flexible scene rendering.
- **Lighting Systems**: Integrated point light and multiple light systems to enhance scene realism.
- **3D Model Loading**: Capable of loading 3D OBJ models with support for push constants and descriptor sets.
- **Game Loops and Input**: Designed for efficient game loops with flexible user input handling and command buffer management.

## Showcase
Here are some examples of the engine's features in action:

### 1. 3D Transformations and Model Loading
*Example of loading a 3D model and applying transformations (translation, rotation, scaling)*
![3D Model Loading](https://github.com/sjpjoshi/Little-Vulkan-Game-Engine/blob/main/Example1.gif)

### 2. Dynamic Viewports and Transparency
*Example demonstrating viewport changes and transparency effects*
![Dynamic Viewport](https://github.com/sjpjoshi/Little-Vulkan-Game-Engine/blob/main/Example2.gif)

### 3. Lighting and Shading
*Example showcasing the Phong-Blinn lighting model with point lights and specular highlights*
![Lighting System](https://github.com/sjpjoshi/Little-Vulkan-Game-Engine/blob/main/Example3.gif)

## Getting Started
1. Clone the repository:
    ```bash
    git clone https://github.com/sjpjoshi/Little-Vulkan-Game-Engine.git
    ```
2. Build the project using CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    make
    ```
3. Run the engine:
    ```bash
    ./LittleVulkanGameEngine
    ```

## Roadmap
- [ ] Textures
- [ ] Add more advanced rendering techniques (e.g., shadow mapping, HDR).
- [ ] Improve performance and optimize memory usage.
- [ ] Add support for additional model formats.
- [ ] Rigidbody, and other physics are soon to come
