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

Client::Client(std::shared_ptr<spdlog::logger> logger) {
  // create client socket
  int m_client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_client_socket == -1) {
      perror("Client socket creation failed");
      exit(1);
  }

  struct sockaddr_in m_server_address;
  m_server_address.sin_family = AF_INET;
  m_server_address.sin_port = htons(PORT); // Use the same port as the server
  m_server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

  // user data
  user_info = new User();
}

Client::~Client() {

}

void Client::run() {

}

void Client::stop() {

}

void Client::connect_to_server() {
  // connect to server
  if (connect(m_client_socket, (struct sockaddr*)&m_server_address, sizeof(m_server_address)) == -1) {
      perror("Client connection failed");
      exit(1);
  }
}

void Client::disconnect_to_server() {
  close(m_client_socket);
}

void Client::send_cmd() {
  //const char* message = "Hello, Server!";
  //send(clientSocket, message, strlen(message), 0);
  //while(1) {
  // FIXME check connection
  //std::cout << "Please enter a phrase or [q] to exit: ";
  std::string strInput;
  //std::cin >> strInput;

  if (strInput.compare("q")==0) return;

  // convert the std::string to a char*
  char* msg = new char[strInput.length()+1];  // +1 for the null terminator

  // copy
  std::strcpy(msg, strInput.c_str());
  send(m_client_socket, msg, strlen(msg), 0);
  //}
}

void Client::render_patient_data() {
    // patient data
    ImGui::BeginChild("PatientDataChild", ImVec2(400, 800), true);
    // title
    ImGui::PushFont(opensans_reg_font_l);
    ImGui::Text("Personal Info");
    ImGui::PopFont();

    int input_width = 230;
    int input_offset = 150;
    // id
    ImGui::Text("ID:"); ImGui::SameLine(input_offset);
    ImGui::Text("##%015d", user_info->id);
    // first name
    ImGui::Text("First Name:"); ImGui::SameLine(input_offset); 
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##FirstName", user_info->first_name, IM_ARRAYSIZE(user_info->first_name));
    // last name
    ImGui::Text("Last Name:"); ImGui::SameLine(input_offset);
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##LastName", user_info->last_name, IM_ARRAYSIZE(user_info->last_name));
    // age
    ImGui::Text("Age:"); ImGui::SameLine(input_offset);
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputInt("##Age", &user_info->age);
    // gender
    ImGui::Text("Gender:"); ImGui::SameLine(input_offset);
    ImGui::RadioButton("male", &user_info->gender, 0); ImGui::SameLine(input_offset+120);
    ImGui::RadioButton("female", &user_info->gender, 1);
    // race
    ImGui::Text("Race:"); ImGui::SameLine(input_offset);
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##Race", user_info->race, IM_ARRAYSIZE(user_info->race));
    // eye color
    ImGui::Text("Eye color:"); ImGui::SameLine(input_offset);
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##EyeColor", user_info->eye_color, IM_ARRAYSIZE(user_info->eye_color));
    // medical condition
    ImGui::Text("Medical Condition:");
    ImGui::InputTextMultiline("##MedicalCondition", user_info->medical_condition, sizeof(user_info->medical_condition), ImVec2(370.0f, 100.0f));
    // eye condition
    ImGui::Text("Eye condition:");
    ImGui::InputTextMultiline("##EyeCondition", user_info->eye_condition, sizeof(user_info->eye_condition), ImVec2(370.0f, 100.0f));
    // user note
    ImGui::Text("User note:");
    ImGui::InputTextMultiline("##UserNote", user_info->user_note, sizeof(user_info->user_note), ImVec2(370.0f, 100.0f));

    ImGui::EndChild();
}

void Client::render() {
    // main window
    ImGui::SetNextWindowSize(ImVec2(1920, 1080));
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin("VisionEve Client", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // patient data
    render_patient_data();

    // Retrieve the captured log messages as a string
    ImGui::PushFont(opensans_reg_font_s);
    ImGui::BeginChild("Log system", ImVec2(780, 100), true,
                        ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("Log system");
    std::string capturedLogs = logCaptureStream.str();
    ImGui::TextUnformatted(capturedLogs.c_str());
    ImGui::SetScrollY(ImGui::GetScrollMaxY());
    ImGui::EndChild();
    ImGui::PopFont();

    // exit button
    if (ImGui::Button("Exit")) {
      exit_requested = true;
    }

    ImGui::End();
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
  client->run();

  // initialize database
  Database* db = new Database(logger);

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
    client->render();
    //main_UI_render();

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
    if (exit_requested)
      break;
  }

  // stop server
  client->stop();
  delete client;

  // cleanup
  imgui_cleanup();
  glfw_cleanup();

  return 0;

  //// create client socket
  //int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  //if (clientSocket == -1) {
  //    perror("Client socket creation failed");
  //    exit(1);
  //}

  //struct sockaddr_in serverAddress;
  //serverAddress.sin_family = AF_INET;
  //serverAddress.sin_port = htons(PORT); // Use the same port as the server
  //serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

  //// connect to server
  //if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
  //    perror("Client connection failed");
  //    exit(1);
  //}

  //// sending data
  ////const char* message = "Hello, Server!";
  ////send(clientSocket, message, strlen(message), 0);
  //while(1) {
  //  std::cout << "Please enter a phrase or [q] to exit: ";
  //  std::string strInput;
  //  std::cin >> strInput;

  //  if (strInput.compare("q")==0) break;

  //  // convert the std::string to a char*
  //  char* msg = new char[strInput.length()+1];  // +1 for the null terminator

  //  // copy
  //  std::strcpy(msg, strInput.c_str());
  //  send(clientSocket, msg, strlen(msg), 0);
  //}

  //// close socket
  //close(clientSocket);

  return 0;
}

