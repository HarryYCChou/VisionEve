/*
 * Copyright (c) 2023 Harry Chou
 */

#include "./client.h"

GLFWwindow* window;
const char* glsl_version = "#version 130";
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool exit_requested = false;

// Fonts
ImFont* opensans_reg_font;
ImFont* opensans_reg_font_s;
ImFont* opensans_reg_font_l;

// Create an std::stringstream to capture log messages
std::stringstream logCaptureStream;
std::shared_ptr<spdlog::logger> logger;

// glfw error callback
static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// glfw initialize function
int glfw_initialize() {
  logger->info("GLFW initializing.");
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
  glfwSwapInterval(1);  // Enable vsync

  return 0;
}

int imgui_initialize() {
  logger->info("ImGui initializing.");
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO(); (void)io;
  // Enable Keyboard Control
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  // Enable Multi-Viewport / Platform Windows
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  io.ConfigViewportsNoAutoMerge = true;
  io.ConfigViewportsNoTaskBarIcon = true;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  //  When viewports are enabled we tweak WindowRounding/WindowBg so platform
  //  windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding = 0.0f;
      style.ChildRounding = 10.0f;
      style.FrameRounding = 10.0f;
      style.TabRounding = 10.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Fonts
  opensans_reg_font = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans-Regular.ttf", 30);
  opensans_reg_font_l = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans-Regular.ttf", 40);
  opensans_reg_font_s = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans-Regular.ttf", 16);

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

int main() {
  // Create an ostream_sink using the logCaptureStream
  auto ostreamSink = std::make_shared<spdlog::sinks::ostream_sink_st>(logCaptureStream);
  // Create a logger with the ostream_sink
  logger = std::make_shared<spdlog::logger>("client", ostreamSink);
  spdlog::register_logger(logger);
  // Log a message
  logger->info("Log system is ready.");

  // initialize glfw
  if (glfw_initialize()) {
    // FIXME: log failed to initialize glfw
    logger->error("GLFW failed to initialize.");
    glfw_cleanup();
    return 1;
  }

  // initialize imgui
  imgui_initialize();

  // initialize server
  Client* client = new Client(logger);
  client->opensans_reg_font = opensans_reg_font;
  client->opensans_reg_font_s = opensans_reg_font_s;
  client->opensans_reg_font_l = opensans_reg_font_l;
  client->run();

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
    // load log
    std::string capturedLogs = logCaptureStream.str();
    client->set_log(capturedLogs);
    // render
    client->render();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                    clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);

    // check exit request
    if (client->exit_requested)
      break;
  }

  // stop server
  client->stop();
  delete client;

  // cleanup
  imgui_cleanup();
  glfw_cleanup();

  return 0;
}

