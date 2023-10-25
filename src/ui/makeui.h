#ifndef MCMV_CORE_MAKE_UI_
#define MCMV_CORE_MAKE_UI_
#include "joint_headers.h"


#define VSYNC 1

#define DARK 1
#define LIGHT 0

// Render Main Viewing Window

class MakeUI{
  public:
    int width;
    int height;
    std::string title;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  private:
    std::string glsl_version = "#version 130";
    GLFWwindow *window;

  private:
    int SetupGLFW();
    void SetupImGuiContext();
    void DestroyWindow();
    void BeginFrame();
    void EndFrame();

  public:
    int MakeWindow(int width, int height, std::string title, void (*Frame)(MakeUI*));
};


void RenderFrame(MakeUI* ui);

#endif