#!/bin/bash

@echo off
cd ../libs

mkdir imgui imguifiledialog glfw

git clone https://github.com/ocornut/imgui.git imgui

git clone https://github.com/aiekick/ImGuiFileDialog.git imguifiledialog

git clone https://github.com/glfw/glfw.git glfw

echo DONE!


