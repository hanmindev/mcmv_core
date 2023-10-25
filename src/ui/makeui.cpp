#include "makeui.h"


// setup glfw window context
int MakeUI::SetupGLFW() 
{
  if (!glfwInit())
      return 1;


  // GL 3.0 + GLSL 130
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
  // Create window with graphics context
  this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);
  if (window == nullptr)
      return 1;

  glfwMakeContextCurrent(this->window);
  glfwSwapInterval(VSYNC); // Enable vsync
  glfwSetWindowAttrib(this->window, GLFW_RESIZABLE, 1); // make window non-resizeable
  // glfwWindowHint(GLFW_SCALE_TO_MONITOR, 1); // make window non-resizeable
  return 0;
}

// setup imgui context settings
void MakeUI::SetupImGuiContext()
{
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigWindowsResizeFromEdges = true; //do not allow window resize
  io.IniFilename = NULL; //do not drop ini config file

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init(this->glsl_version.c_str());
}

//destroy window object
void MakeUI::DestroyWindow()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(this->window);
  glfwTerminate();
}

void MakeUI::BeginFrame()
{
  // Poll and handle events (inputs, window resize, etc.)
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  glfwGetWindowSize(this->window, &this->width, &this->height);
  ImGui::NewFrame();
}

void MakeUI::EndFrame()
{
  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(this->window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(this->clear_color.x * this->clear_color.w, this->clear_color.y * this->clear_color.w, this->clear_color.z * this->clear_color.w, this->clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  
  glfwSwapBuffers(this->window);
}



// boot main gui window
int MakeUI::MakeWindow(int width, int height, std::string title, void (*Frame)(MakeUI*))
{
  this->width = width;
  this->height = height;
  this->title = title;

  this->SetupGLFW();
  this->SetupImGuiContext();

  while (!glfwWindowShouldClose(window))
  {
    this->BeginFrame();

    // render the entire frame
    // callback here
    Frame(this);

    this->EndFrame();
  }

  this->DestroyWindow();
  return 0;
}


