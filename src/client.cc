/*
 * Copyright (c) 2023 Harry Chou
 */

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
}

Client::~Client() {

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
  if (ImGui::Button("load/edit")) {
    // copy patient data to user_info
    *user_info = all_user[item_current_idx];
  }
  ImGui::SameLine();

  // delete button
  if (ImGui::Button("delete")) {
    db->del_user(all_user[item_current_idx].id);

    // update patient list
    update_all_user_info();
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

void Client::render_camera_data() {
    // camera data
    ImGui::BeginChild("CameraDataChild", ImVec2(1080, 800), true);
    // title
    ImGui::PushFont(opensans_reg_font_l);
    ImGui::Text("Camera");
    ImGui::PopFont();

    ImGui::EndChild();
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
    render_camera_data();

    // Retrieve the captured log messages as a string
    ImGui::PushFont(opensans_reg_font_s);
    ImGui::BeginChild("Log system", ImVec2(780, 100), true,
                        ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::Text("Log system");
    //std::string capturedLogs = logCaptureStream.str();
    //ImGui::TextUnformatted(capturedLogs.c_str());
    ImGui::TextUnformatted(log.c_str());
    ImGui::SetScrollY(ImGui::GetScrollMaxY());
    ImGui::EndChild();
    ImGui::PopFont();

    // exit button
    if (ImGui::Button("Exit")) {
      exit_requested = true;
    }

    ImGui::End();
}
