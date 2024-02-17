/*
 * Copyright (c) 2023 Harry Chou
 */

#define STB_IMAGE_IMPLEMENTATION
#include "./client.h"

Client::Client(std::shared_ptr<spdlog::logger> l) {
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

  // logger
  logger = l;

  // database
  db = new Database(l);
  all_user = db->get_user();

  // camera
  glGenTextures(1, &textureID_CamL);
  glGenTextures(1, &textureID_CamR);
  cam = new Camera(0, 2, l);
  cam->run();

  // logo
  textureID_logo = load_texture("../test_data/logo/renewoptics_logo.png");

  // FIXME: camera class
  textureID_L = load_texture("../test_data/eye_images/eye-sample-l.png");
  textureID_R = load_texture("../test_data/eye_images/eye-sample-r.png");

  // FIXME: LBS class
  textureID_LBS_L = load_texture("../test_data/lbs_content/eye_chart/L.png");
  textureID_LBS_R = load_texture("../test_data/lbs_content/eye_chart/R.png");
  // for vertical flip
  cv::Mat image = cv::imread("../test_data/lbs_content/eye_chart/L.png");
  cv::flip(image, image, 0); 
  load_texture(textureID_LBS_VF_L, image);
  image = cv::imread("../test_data/lbs_content/eye_chart/R.png");
  cv::flip(image, image, 0); 
  load_texture(textureID_LBS_VF_R, image);

  // detect number of monitor
  Display* display = XOpenDisplay(nullptr);
  n_of_monitor = XScreenCount(display);
  XCloseDisplay(display);
}

Client::~Client() {
  delete cam;
  //delete cam_L;
  //delete cam_R;
}

void Client::run() {

}

void Client::stop() {

}

void Client::update_all_user_info() {
  all_user = db->get_user();
}

void Client::set_log(std::string l) {
  log = l;
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

void Client::render_all_user() {
  // user list
  ImGui::BeginChild("PatientList", ImVec2(400, 800), true);

  // title
  ImGui::PushFont(opensans_reg_font_l);
  ImGui::Text("Patient List");
  ImGui::PopFont();

  static int item_current_idx = 0;
  if (ImGui::BeginListBox("##PatientList", ImVec2(380,685))) {
    for (int i = 0; i < all_user.size(); i++) {
      const bool is_selected = (item_current_idx == i);
     
      std::ostringstream formattedString;
      formattedString << "# " << std::setw(4) << std::setfill('0') << all_user[i].id;
      formattedString << " | " << all_user[i].first_name << " " << all_user[i].last_name;
      std::string list_name = formattedString.str();
      //std::string list_name = std::to_string(all_user[i].id);
      if (ImGui::Selectable(list_name.c_str(), is_selected))
        item_current_idx = i;

      if (is_selected)
        ImGui::SetItemDefaultFocus(); 
    }

    ImGui::EndListBox();
  }

  // new button
  if (ImGui::Button("New")) {
      User* u = new User();

      db->add_user(u);

      // update patient list
      update_all_user_info();
  }
  ImGui::SameLine();

  // load button
  if (ImGui::Button("Load/Edit")) {
    // copy patient data to user_info
    if (all_user.size() > item_current_idx) {
      *user_info = all_user[item_current_idx];
      if (strcmp(user_info->first_name, "New") == 0
          && strcmp(user_info->last_name, "User") == 0) {
        std::strcpy(user_info->first_name, "");
        std::strcpy(user_info->last_name, "");
      }
    }
  }
  ImGui::SameLine();

  // delete button
  if (ImGui::Button("Delete")) {
    db->del_user(all_user[item_current_idx].id);

    // update patient list
    update_all_user_info();
  }
  ImGui::SameLine();

  // export button
  if (ImGui::Button("Export")) {
    for (int i = 0; i < all_user.size(); i++) {
      std::string f_name = "../export/" + std::to_string(all_user[i].id) + ".csv";
      std::ofstream file(f_name, std::ios::out);
      if (!file.is_open()) {
        logger->error("Error open file - {}", f_name);
        continue;
      }
      file << "ID," << all_user[i].id << std::endl;
      file << "Age," << all_user[i].age << std::endl;
      file << "Gender," << all_user[i].gender << std::endl;
      file << "Race/Ethnicity," << all_user[i].race << std::endl;
      file << "Eye Color," << all_user[i].eye_color << std::endl;
      file << "Eye condition," << all_user[i].eye_condition << std::endl;
      file << "Note," << all_user[i].user_note << std::endl;
      file.close();
    }

    logger->info("Exporting files successfully.");
  }

  ImGui::EndChild();
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
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##Gender", user_info->race, IM_ARRAYSIZE(user_info->race));
    // race
    ImGui::Text("Race/Ethnicity:"); ImGui::SameLine(input_offset + 20);
    ImGui::SetNextItemWidth(input_width - 20);
    ImGui::InputText("##Race", user_info->race, IM_ARRAYSIZE(user_info->race));
    // eye color
    ImGui::Text("Eye color:"); ImGui::SameLine(input_offset);
    ImGui::SetNextItemWidth(input_width);
    ImGui::InputText("##EyeColor", user_info->eye_color, IM_ARRAYSIZE(user_info->eye_color));
    // medical condition
    //ImGui::Text("Medical Condition:");
    //ImGui::InputTextMultiline("##MedicalCondition", user_info->medical_condition, sizeof(user_info->medical_condition), ImVec2(370.0f, 100.0f));
    // eye condition
    ImGui::Text("Eye condition:");
    ImGui::InputTextMultiline("##EyeCondition", user_info->eye_condition, sizeof(user_info->eye_condition), ImVec2(370.0f, 100.0f));
    // user note
    ImGui::Text("User note:");
    ImGui::InputTextMultiline("##UserNote", user_info->user_note, sizeof(user_info->user_note), ImVec2(370.0f, 100.0f));

    // save button
    ImGui::SetCursorPos(ImVec2(5, 740));
    if (ImGui::Button("Save", ImVec2(390, 35))) {
      db->update_user(user_info);

      // update patient list
      update_all_user_info();
    }

    ImGui::EndChild();
}

void Client::render_camera_data() {
    // camera data
    ImGui::BeginChild("CameraDataChild", ImVec2(825, 580), true);
    // title
    ImGui::PushFont(opensans_reg_font_l);
    ImGui::Text("Camera");
    ImGui::PopFont();

    ImGui::Text("Left camera device : "); ImGui::SameLine();
    ImGui::Text("/dev/video0");
    ImGui::Text("Right camera device: "); ImGui::SameLine();
    ImGui::Text("/dev/video2");
    // get image from camera
    // try camera L
    if (cam->get_image(0, image_buf) && (!image_buf.empty())) {
      load_texture(textureID_CamL, image_buf);
      ImGui::Image((void*)(intptr_t)textureID_CamL, ImVec2(400, 400));
    } else {
      ImGui::Image((void*)(intptr_t)textureID_L, ImVec2(400, 400));
    }
    ImGui::SameLine();
    // try camera R
    if (cam->get_image(1, image_buf) && (!image_buf.empty())) {
      load_texture(textureID_CamR, image_buf);
      ImGui::Image((void*)(intptr_t)textureID_CamR, ImVec2(400, 400));
    } else {
      ImGui::Image((void*)(intptr_t)textureID_R, ImVec2(400, 400));
    }
    ImGui::Text("                                   L                                                                     R");

    ImGui::EndChild();
}

void Client::render_lbs_control() {
    // lbs control panel
    ImGui::BeginChild("LBSControlChild", ImVec2(825, 470), true);
    // title
    ImGui::PushFont(opensans_reg_font_l);
    ImGui::Text("LBS Control");
    ImGui::PopFont();

    // LBS control
    // brightness
    //ImGui::BeginDisabled(true);
    //ImGui::Text("Brightness : "); ImGui::SameLine();
    //ImGui::SetNextItemWidth(670.0f);
    //ImGui::SliderInt("##Brightness", &lbs_brightness, 0, 100);
    // contrast
    //ImGui::Text("Contrast     : "); ImGui::SameLine();
    //ImGui::SetNextItemWidth(670.0f);
    //ImGui::SliderInt("##Contrast", &lbs_contrast, 0, 100);
    //ImGui::EndDisabled();
    ImGui::Checkbox("Vertical flip LBS content: ", &vertical_flip);
    ImGui::Text(" ");

    // content
    ImGui::Text("LBS Content: "); ImGui::SameLine();
    ImGui::Text("Eye chart");
    // L
    ImGui::Image((void*)(intptr_t)textureID_LBS_L, ImVec2(400, 200)); ImGui::SameLine();
    // R
    ImGui::Image((void*)(intptr_t)textureID_LBS_R, ImVec2(400, 200));
    ImGui::Text("                                   L                                                                     R");


    ImGui::EndChild();
}

void Client::render_side_panel() {
    // side panel
    ImGui::BeginChild("SidePanelChild", ImVec2(250, 1060), true);
    // title
    ImGui::PushFont(opensans_reg_font_l);

    // logo
    ImGui::Image((void*)(intptr_t)textureID_logo, ImVec2(235, 200));

    // toolbox button
    /*
    if (ImGui::Button("Function 1", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 2", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 3", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 4", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 5", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 6", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 7", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 8", ImVec2(235, 80))) {}
    if (ImGui::Button("Function 9", ImVec2(235, 80))) {}
    */

    // Push a modified color for the specific button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.2f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.3f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.1f, 0.7f, 1.0f));
    // exit button
    ImGui::SetCursorPos(ImVec2(0, 960));
    if (ImGui::Button("Exit", ImVec2(235, 80))) {
      exit_requested = true;
    }
    // Pop the modified color after using it
    ImGui::PopStyleColor(3); // Pop 3 times to revert all the changes made above

    ImGui::PopFont();

    ImGui::EndChild();


}

void Client::render_lbs_content() {
    // if LBS doen not connected
    if (n_of_monitor < 3) return;

    // LBS content L
    ImGui::SetNextWindowSize(ImVec2(1920, 0));
    ImGui::Begin("LBS_L", NULL, ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoMove);
    if (vertical_flip) {
      ImGui::Image((void*)(intptr_t)textureID_LBS_VF_L, ImVec2(1024, 600));
    } else {
      ImGui::Image((void*)(intptr_t)textureID_LBS_L, ImVec2(1024, 600));
    }
    ImGui::End();

    // LBS content R
    ImGui::SetNextWindowSize(ImVec2(3000, 0));
    ImGui::Begin("LBS_R", NULL, ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoMove);
    if (vertical_flip) {
      ImGui::Image((void*)(intptr_t)textureID_LBS_VF_R, ImVec2(1024, 600));
    } else {
      ImGui::Image((void*)(intptr_t)textureID_LBS_R, ImVec2(1024, 600));
    }
    ImGui::End();
}

void Client::render() {
    // main window
    /* avoid resize and reset window position when rendering */
    //ImGui::SetNextWindowSize(ImVec2(1920, 1080));
    //ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin("VisionEve Client", NULL, ImGuiWindowFlags_NoResize |
                                           ImGuiWindowFlags_NoTitleBar |
                                           ImGuiWindowFlags_NoMove);


    // patient list 
    render_all_user(); ImGui::SameLine();
    // patient data
    render_patient_data(); ImGui::SameLine();

    // camera data
    ImGui::SetNextWindowPos(ImVec2(825, 10));
    render_camera_data();

    // lbs control panel
    ImGui::SetNextWindowPos(ImVec2(825, 600));
    render_lbs_control();

    // side panel
    ImGui::SetNextWindowPos(ImVec2(1660, 10));
    render_side_panel();

    // log system
    ImGui::SetNextWindowPos(ImVec2(8, 815));
    // Retrieve the captured log messages as a string
    ImGui::PushFont(opensans_reg_font_s);
    ImGui::BeginChild("Log system", ImVec2(810, 255), true,
                        ImGuiWindowFlags_HorizontalScrollbar);
    // title
    ImGui::PushFont(opensans_reg_font_l);
    ImGui::Text("Log system");
    ImGui::PopFont();
    ImGui::TextUnformatted(log.c_str());
    ImGui::SetScrollY(ImGui::GetScrollMaxY());
    ImGui::EndChild();

    ImGui::PopFont();

    // LBS render
    render_lbs_content();


    ImGui::End();
}

void Client::load_texture(GLuint& textureID, Mat& image) {
  // convert image format
  cv::cvtColor(image, image, cv::COLOR_BGR2RGBA);
  // glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows,
                0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
}

GLuint Client::load_texture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        // Handle error
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return textureID;
}
