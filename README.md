# Software Renderer

A 3D renderer written from scratch in C++. It draws every pixel itself, on the CPU: no OpenGL calls, no GPU shaders, no graphics or math libraries. [SFML](https://www.sfml-dev.org/) is used only to open a window, read the keyboard and mouse, and put the finished image on screen.

Load a 3D model, light it, texture it, and fly around it, or straight through it, with the mouse and keyboard.

<!-- TODO: after uploading the demo video, replace this line with:  [▶ Watch the demo video](https://...)  -->
![The renderer drawing a lit, textured model](docs/images/screenshot-1.png)

Built with C++17 and SFML 2.6.2.

## Contents

- [What it does](#what-it-does)
- [Screenshots](#screenshots)
- [Quick start](#quick-start)
- [Controls](#controls)
- [Building from source](#building-from-source)
  - [Windows 11](#windows-11)
  - [Linux](#linux)
  - [macOS](#macos)
  - [Using an SFML you already have](#using-an-sfml-you-already-have)
  - [Build types: Release and Debug](#build-types-release-and-debug)
  - [Running it](#running-it)
- [Troubleshooting](#troubleshooting)
- [Using your own models and textures](#using-your-own-models-and-textures)
- [How it works](#how-it-works)
- [Performance](#performance)
- [Project layout](#project-layout)
- [Limitations and next steps](#limitations-and-next-steps)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## What it does

- **Framebuffer and pixels.** One array of RGBA bytes that the whole renderer writes into, one pixel at a time.
- **Triangle rasterisation** using edge functions and barycentric coordinates, with the top-left fill rule, so triangles that share an edge never overlap and never leave a gap.
- **Its own math library.** `Vec2`, `Vec3`, `Vec4` and a 4×4 matrix type with translation, rotation, scale and perspective projection.
- **A z-buffer** (it stores 1/depth), so nearer surfaces hide farther ones in whatever order triangles are drawn.
- **An OBJ model loader.** Vertices, texture coordinates and faces; polygons with more than three corners are split into triangles.
- **Lighting.** A directional light plus ambient light, with smooth (Gouraud) shading from averaged vertex normals.
- **Texture mapping** from PPM images, with perspective-correct interpolation, so textures do not warp as a surface tilts away.
- **A free-fly camera.** Mouse look, WASD to walk, Space and Shift to go up and down.
- **Near-plane clipping.** A triangle that crosses the camera is cut exactly at the near plane, so you can fly through a model without triangles vanishing, exploding or freezing the program.
- **Back-face culling** and an on-screen display of FPS, frame time and camera position.

All of the rendering lives in a single file, [`src/main.cpp`](src/main.cpp).

## Screenshots

<p align="center">
  <img src="docs/images/screenshot-2.png" width="49%" alt="The renderer, second view">
  <img src="docs/images/screenshot-3.png" width="49%" alt="The renderer, third view">
</p>

## Quick start

You need a C++17 compiler, CMake, Git and Ninja, plus a few system libraries on Linux. [Building from source](#building-from-source) has copy-and-paste instructions for Windows and Linux.

```sh
git clone https://github.com/ranvijay7004/Software-renderer.git
cd Software-renderer
cmake -B build -G Ninja
cmake --build build
./build/renderer
```

On Windows, run `./build/renderer.exe` from the MSYS2 terminal, or double-click `build\renderer.exe`.

The first build downloads SFML 2.6.2 and compiles it, which takes a few minutes and needs an internet connection. That happens once. After that, rebuilding the renderer takes seconds.

## Controls

| Input | Action |
|---|---|
| Left click in the window | Lock the mouse (look mode) |
| Mouse | Look around |
| `Esc` | Unlock the mouse |
| `W` / `S` | Walk forward / backward |
| `A` / `D` | Step left / right |
| `Space` / `Left Shift` | Move up / down |
| Window close button | Quit |

Walking follows the direction you are facing on the ground. Looking up or down does not lift you off it; use `Space` and `Shift` for that. The controls are also printed in the bottom-right corner of the window.

## Building from source

### What you need

| Tool | Version | Why |
|---|---|---|
| C++ compiler | C++17 (GCC; tested with GCC 13 on Linux and MinGW-w64 GCC on Windows) | compiles the renderer and SFML |
| CMake | 3.16 or newer (tested with 3.28 and 4.4) | the build system |
| Ninja | any | the build tool CMake drives |
| Git | any | CMake uses it to download SFML |
| Internet connection | first build only | to download SFML 2.6.2 |
| Disk space | about 200 MB | the `build/` folder (it is not part of the repository) |

**You do not install SFML yourself.** `CMakeLists.txt` downloads the exact version, SFML 2.6.2, and builds it for your compiler. That avoids the usual problems of mismatched compilers, missing DLLs and the wrong SFML version. The result is a single executable with SFML built in.

SFML 2.6.2 is used on purpose. SFML 3 has a different API, so the code in this project does not compile against it. This matters on some Linux distributions: do **not** install their `sfml` package (Arch ships SFML 3, for example). Let CMake build 2.6.2 for you.

### Windows 11

These steps use [MSYS2](https://www.msys2.org/), which gives you a modern MinGW-w64 GCC, CMake, Ninja and Git in a few commands.

1. **Install MSYS2.** Download the installer from [msys2.org](https://www.msys2.org/) and run it with the default options.

2. **Open the right terminal.** In the Start menu, open **MSYS2 UCRT64**. It must be the UCRT64 one, not "MSYS2 MSYS" or "MSYS2 MINGW64".

3. **Update MSYS2.**

   ```sh
   pacman -Syu
   ```

   If it asks you to close the terminal, close it, open **MSYS2 UCRT64** again, and run `pacman -Syu` once more.

4. **Install the tools.**

   ```sh
   pacman -S --needed git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
   ```

5. **Download the project into a folder without spaces in its path.** For example `C:\dev`. (In MSYS2 that folder is `/c/dev`.)

   ```sh
   mkdir -p /c/dev && cd /c/dev
   git clone https://github.com/ranvijay7004/Software-renderer.git
   cd Software-renderer
   ```

6. **Build.**

   ```sh
   cmake -B build -G Ninja
   cmake --build build
   ```

   You should see `-- Build type: Release` during the first command. A CMake warning saying "Compatibility with CMake < 3.10 will be removed" is normal; it comes from SFML's own build files and is harmless.

7. **Run.**

   ```sh
   ./build/renderer.exe
   ```

The program is one self-contained `.exe`. It only needs the DLLs that every Windows installation already has, so you can copy `build/renderer.exe` together with the `assets` folder to another Windows PC and it will run.

If you already have a MinGW-w64 compiler, CMake and Ninja on your `PATH`, you can skip MSYS2 and run steps 5 to 7 in any terminal. Visual Studio (MSVC) is not tested.

### Linux

Install the build tools and the development libraries SFML needs: X11, Xrandr, Xcursor, udev, OpenGL and FreeType. (The audio and network parts of SFML are switched off, so none of their libraries are needed.)

**Ubuntu / Debian**

```sh
sudo apt update
sudo apt install build-essential cmake ninja-build git \
    libx11-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev libfreetype-dev
```

(On older releases the FreeType package is called `libfreetype6-dev`.)

**Fedora**

```sh
sudo dnf install gcc-c++ cmake ninja-build git \
    libX11-devel libXrandr-devel libXcursor-devel systemd-devel mesa-libGL-devel freetype-devel
```

**Arch**

```sh
sudo pacman -S --needed base-devel cmake ninja git libx11 libxrandr libxcursor systemd-libs mesa libglvnd freetype2
```

Do not install Arch's `sfml` package; it is SFML 3.

Then build and run:

```sh
git clone https://github.com/ranvijay7004/Software-renderer.git
cd Software-renderer
cmake -B build -G Ninja
cmake --build build
./build/renderer
```

The Ubuntu commands were tested on a clean Ubuntu 24.04. The Fedora and Arch package names are the standard ones for the same libraries; if a name has changed on your release, search your package manager for the library name (for example "xcursor").

**Wayland (Hyprland, Sway, GNOME, KDE).** SFML 2.6 talks to the X11 protocol, so on Wayland it runs through XWayland. Most desktops enable XWayland by default. If the window does not open and you see `Failed to open X11 display`, install and enable XWayland (the package is usually called `xorg-xwayland` or `xwayland`).

### macOS

Untested. In principle it needs the Xcode command line tools (`xcode-select --install`), then `brew install cmake ninja`, and the same `cmake -B build -G Ninja` and `cmake --build build` commands.

### Using an SFML you already have

If an SFML **2.6.x** is already installed (Fedora, for example, ships 2.6.2 as the `SFML-devel` package), you can skip the download and the SFML compile:

```sh
cmake -B build -G Ninja -DUSE_SYSTEM_SFML=ON
cmake --build build
```

This builds in a few seconds. It only works with SFML 2.6.x; CMake will stop with an error if it only finds SFML 3. In that case leave the option off and let CMake download 2.6.2.

### Build types: Release and Debug

`CMakeLists.txt` builds in **Release** mode by default, which switches the compiler's optimiser on. This matters a lot for a software renderer, because the inner loop runs millions of times per frame. In a benchmark of the triangle-fill loop, the optimised build ran about 3.4 times faster than an unoptimised one on the same machine.

To build without optimisation, for example to step through the code in a debugger:

```sh
cmake -B build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```

CMake remembers the build type inside the `build` folder. If you configured a folder as Debug earlier and want Release, delete the folder (`rm -rf build`) and configure again.

### Running it

Run it from the project folder or from inside `build/`; both work, because the build copies the `assets` folder next to the executable.

```sh
./build/renderer          # from the project folder
cd build && ./renderer    # or from the build folder
```

If you add new model files and run from the project folder, they are picked up immediately. If you run from `build/`, rebuild first so the new files are copied.

To update later: `git pull`, then `cmake --build build`.

## Troubleshooting

**CMake cannot download SFML** (`Could not resolve host`, a Git or `FetchContent` error). The first configure needs an internet connection and `git`. If you are behind a firewall or proxy, install SFML 2.6.x another way and use [`-DUSE_SYSTEM_SFML=ON`](#using-an-sfml-you-already-have).

**`Could NOT find X11`, `UDev`, `OpenGL` or `Freetype` (Linux).** A development package is missing. Install the list for your distribution from [Linux](#linux), then delete the `build` folder and configure again.

**`Failed to open X11 display` (Wayland).** XWayland is not running. See the Wayland note under [Linux](#linux).

**A window never appears, and the terminal says `couldn't load model`, `couldn't load texture` or `couldn't load font`.** The program could not find a file. Run it from the project folder or from `build/` (see [Running it](#running-it)), and check that the `assets/` folder is there with the files inside it.

**It is slow.** Make sure the build is Release: the configure step prints `-- Build type: Release`. If it says Debug, or prints nothing, delete the `build` folder and configure again. The renderer also draws at half resolution on purpose (see [Performance](#performance)).

**I changed `CMakeLists.txt` or switched build type and nothing changed.** CMake caches its settings. Delete the `build` folder and configure again.

**A CMake warning mentions "Compatibility with CMake < 3.10".** Harmless. It comes from SFML's own build files, not from this project.

**Windows: odd path or build errors.** Move the project to a folder whose path has no spaces, for example `C:\dev\Software-renderer`, and delete the `build` folder.

**Windows: the program complains about a missing DLL.** The build normally needs no extra DLLs. Make sure you opened the **MSYS2 UCRT64** terminal, and that the build finished without errors.

## Using your own models and textures

The sample meshes (`.obj`) and textures (`.ppm`) are in [`assets/`](assets/). To load different ones, open `src/main.cpp`, find the `loadobj(` and `loadppm(` calls in `main()`, change the two file paths, and rebuild.

**Models** (`.obj`)

- Each vertex is a `v x y z` line, each texture coordinate a `vt u v` line, and each face an `f` line such as `f 1/1 2/2 3/3` (vertex number / texture-coordinate number).
- A model needs `vt` lines to be textured.
- Faces with more than three corners are split into triangles.
- Normals in the file are ignored; the renderer works them out from the geometry.
- Indices must be positive (negative, relative indices are not supported).

**Conventions to know about.** The `y` axis points **down**, like the screen, and a triangle is drawn only when its corners appear **clockwise** on screen (that is the back-face test). Models exported from tools that use a y-up axis and counter-clockwise faces, such as Blender's defaults, may therefore appear upside-down or inside-out. If that happens, flip the y axis and reverse each face's winding (swap two indices per face) in the exporter or in the loader.

**Textures** (`.ppm`)

- The binary PPM format ("P6"), 8 bits per channel.
- Convert a PNG with ImageMagick: `magick texture.png texture.ppm` (older versions: `convert texture.png texture.ppm`). In GIMP, use **File → Export As**, name it `.ppm`, and choose **Raw**.
- The `u` coordinate wraps (so textures can repeat around a model); `v` should stay between 0 and 1.

## How it works

The whole pipeline, from a file on disk to a pixel on screen:

```
.obj file ──► vertices, texture coordinates, triangles
                 │
   model matrix (spin)  ──►  view matrix (camera)  ──►  perspective matrix
                 │
   clip every triangle against the near plane (new corners where it crosses)
                 │
   divide by w  ──►  screen pixel coordinates
                 │
   for each triangle, test every pixel in its bounding box:
        ├─ edge functions: is the pixel inside?  (also gives the barycentric weights)
        ├─ z-buffer: is it the nearest thing at this pixel so far?
        ├─ Gouraud shading: light brightness blended across the triangle
        ├─ perspective-correct texture lookup
        └─ write the colour into the framebuffer
                 │
   framebuffer ──► SFML texture ──► window (stretched 2×)
```

**1. The framebuffer.** A flat array of bytes, four per pixel (red, green, blue, alpha). Pixel `(x, y)` starts at byte `(y * WIDTH + x) * 4`. Drawing anything means writing to this array. Once per frame it is copied into an SFML texture and shown. SFML uses OpenGL internally to put that one 2D image in the window; none of the 3D work happens on the GPU.

**2. Math.** Positions and directions are `Vec3`s. They are turned into `Vec4`s (with a fourth number, `w`) so that one 4×4 matrix can both rotate and move a point. Matrices are multiplied together once per frame, and the combined matrix is applied to every vertex.

**3. The camera.** The renderer only knows how to look from the origin straight down +z. To "move the camera", the *world* is moved the opposite way: first slide everything by minus the camera's position, then turn everything the opposite way from how the camera turns (the yaw and pitch rotations). Order matters: slide first, so that the rotations happen around the camera.

**4. Perspective.** The projection matrix copies depth into `w`. Dividing `x` and `y` by `w` makes distant things smaller. This is the only step that needs a division.

**5. Clipping.** A triangle with a corner behind the camera has a meaningless `w`. Each triangle is therefore cut at the near plane (depth `NEAR_W`) *before* the divide. Walking round the triangle's three edges produces 0, 3 or 4 corners. The new corners are found by linear interpolation along the cut edge, and the same fraction is used for the texture coordinates and the brightness. A 4-corner result is split into two triangles.

**6. Rasterisation.** For each pixel in the triangle's bounding box, three *edge functions* say which side of each edge the pixel is on. If all three agree, the pixel is inside. Divided by their sum, they are also the barycentric coordinates: how much of each corner the pixel is made of. The top-left rule decides which triangle owns a pixel that lies exactly on a shared edge.

**7. Depth.** The z-buffer stores `1/depth` for every pixel, because `1/depth` (unlike depth) changes evenly across the screen. A pixel is drawn only if its value is larger, meaning nearer, than what is already stored.

**8. Lighting.** Each vertex normal is the average of the normals of the faces around it. Brightness at a vertex is ambient light plus the dot product of the normal with the light direction (Lambert's law). The three vertex brightnesses are blended across the triangle.

**9. Textures.** Texture coordinates `u` and `v` cannot be blended linearly across the screen once perspective is involved. Instead `u/w`, `v/w` and `1/w` are blended, and the pixel divides them at the end. That gives perspective-correct textures.

**10. Back-face culling.** A triangle whose corners appear counter-clockwise on screen faces away from the camera. It is skipped before any pixel is tested, which roughly halves the work for a closed model and removes it entirely when you are inside one.

## Performance

Everything runs on one CPU core, so the cost is roughly (pixels tested per frame) × (work per pixel). Three things keep it smooth, including on a low-end laptop:

- **An optimised build.** Release mode is the default (see [Build types](#build-types-release-and-debug)).
- **Half-resolution rendering.** The renderer draws at 960 × 540 and the window stretches the image 2×, which is four times fewer pixels than drawing at 1920 × 1080.
- **Skipping work.** Triangles facing away are skipped before they are scanned, and each triangle's bounding box is clamped to the screen.

The on-screen display shows FPS and frame time. Flying close to a surface is the slow case, because one triangle can then cover a large part of the screen.

<!-- TODO: add your measured numbers here, e.g. "On <your laptop>: ~XX FPS at the start, ~XX FPS close to the model." -->

## Project layout

```
Software-renderer/
├── CMakeLists.txt        build instructions (downloads and builds SFML 2.6.2)
├── README.md
├── src/
│   └── main.cpp          the whole renderer
└── assets/
    ├── *.obj             sample meshes
    ├── *.ppm             sample textures
    ├── DejaVuSansMono.ttf   font for the on-screen text
    └── FONT-LICENSE.txt     the font's license
```

## Limitations and next steps

- It is single-threaded and not SIMD-optimised; large triangles close to the camera are the slow case.
- Texture sampling is nearest-neighbour only (no filtering, no mipmaps).
- There is one model and one directional light; no shadows or specular highlights.
- Only OBJ meshes and binary PPM textures are supported.

Planned: profiling and optimising the inner loop, several objects in one scene loaded from a scene file, Phong shading (specular highlights), shadow mapping, a simple particle system, and a WebAssembly build so it runs in a browser.

## Acknowledgements

- **[SFML](https://www.sfml-dev.org/)** for the window and input layer.
- **[DejaVu Sans Mono](https://dejavu-fonts.github.io/)** for the on-screen text, under the Bitstream Vera license (see `assets/FONT-LICENSE.txt`).
- **Claude**, an AI assistant from Anthropic. I built this project to learn computer graphics from first principles, and I used Claude as a tutor: it explained concepts, helped me debug, and helped write parts of the implementation (such as near-plane clipping, the camera controls and the on-screen display) as well as the build setup and this README.

## License

Released under the MIT License. See the `LICENSE` file.
