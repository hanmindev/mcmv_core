@echo off
cd ../libs

mkdir imgui
git clone https://github.com/ocornut/imgui.git imgui

mkdir imguifiledialog
git clone https://github.com/aiekick/ImGuiFileDialog.git imguifiledialog

mkdir glfw
git clone https://github.com/glfw/glfw.git glfw

echo DONE!


