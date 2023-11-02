#include <main.hpp>

GLFWwindow* window;  
const char* glsl_version = "#version 130";
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool exit_requested = false;

// glfw error callback
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// glfw initialize function
int glfw_initialize() {
  // set glfw error callback function
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
      return 1;

  // glfw window hint 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // hide glfw
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Create window with graphics context
  window = glfwCreateWindow(1, 1, "VisionEve", nullptr, nullptr);
  if (window == nullptr)
      return 1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  return 0;
}

int imgui_initialize() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
  io.ConfigViewportsNoAutoMerge = true;
  //io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  return 0;
}

void glfw_cleanup() {
  // cleanup glfw
  glfwDestroyWindow(window);
  glfwTerminate();

}

void imgui_cleanup() {
  // cleanup imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void main_UI_render() {
    // main window
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    ImGui::Begin("VisionEve Server");
    ImGui::Text("This is a test sentence");

    // exit button    
    if (ImGui::Button("Exit")) {
      exit_requested = true;
    }
    
    ImGui::End();
}

int main() {

  // initialize glfw
  if (glfw_initialize()) {
    // FIXME: log failed to initialize glfw
    glfw_cleanup();
    return 1;
  }

  // initialize imgui
  imgui_initialize();

  // server
  Server server;
  server.run();

  // main graphic loop
  while (!glfwWindowShouldClose(window)) {
    // graphic window
    glfwPollEvents();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // main UI
    main_UI_render();
 
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);

    // check exit request
    if (exit_requested)
      break;
  }

  // cleanup
  imgui_cleanup();
  glfw_cleanup();

  return 0;
}

