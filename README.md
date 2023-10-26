# MCMV GUI
I have created a Gui that is more user friendly than using a command line,
the gui is implemented using ImGui with a GLFW backend, using a file dialog addon to allow for ease of file browsing.

The Cmake file was edited to allow for compiling both mcmv_core and mcmv_gui separately or together.

I still have a few remaining features to implement alongside helping with the command line tool.

## Build

Building the application yourself is very simple

first you'll need cmake to build this project

- [[Cmake Download](https://cmake.org/download/)]

1) clone the project.
```bat
git clone https://github.com/hanmangokiwi/mcmv_core.git MCMV_Core
```

2) open a terminal and move into the cloned directory,
```bat
cd MCMV_Core
```

3) using cmake run,
```bat
cmake -S . -B build
```

4) while still in the same directory run this command to compile the executable.
 - you can also build the command line version by swapping ```mcmv_gui``` with ```mcmv_core```.

```bat
cmake --build build --config Release --target mcmv_gui
```
5) your output file will be located in the **./build/Release** file.



## Sources

 - GLFW -            https://github.com/glfw/glfw
 - ImGui -           https://github.com/ocornut/imgui/tree/master
 - ImGuiFileDialog - https://github.com/aiekick/ImGuiFileDialog
 - Pthread -         https://sourceware.org/pthreads-win32/
