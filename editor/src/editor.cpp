#include "editor.h"
#include "imgui.h"

 Editor* Editor::get_instance() {
   if (instance == NULL) {
      utils::log_info(ERROR, "init instance of editor before using");
   }
   return instance;
}


bool Editor::is_hovered(ImVec2 p, ImVec2 top_left, ImVec2 bottom_right){
   return (p.x >= top_left.x && p.x <= bottom_right.x) && (p.y >= top_left.y && p.y <= bottom_right.y);
}


void Editor::save_file(std::string& filename){
   std::ofstream ofs(filename);
   if (ofs.is_open())
      utils::log_info(INFO, "writing to data to %s", filename.c_str());
   else {
      utils::log_info(WARN, "failed to write data to %s", filename.c_str());
      return;
   }

   serialize_xml(elements, filename, layout_settings);
   ofs.close();
}
void Editor::create_new_file(){
   if (ImGui::BeginPopup("Create new file")){
      static char buffer[256] = "";

      ImGui::InputText("Filename:", buffer, IM_ARRAYSIZE(buffer));
      
      if (ImGui::Button("Ok")){
         std::string file = (std::string)buffer + ".txt";
         std::ofstream ofs(file);
         utils::log_info(INFO, "creating new file %s", file.c_str());
         ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
   }
         
}

void Editor::open_file() { 
   if (ImGuiFileDialog::Instance()->Display("choose file to read", ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_AlwaysAutoResize, ImVec2(200, 200))) {
      if (ImGuiFileDialog::Instance()->IsOk()) { 
         std::string name =  ImGuiFileDialog::Instance()->GetFilePathName();
         read_file(name);
      }
      ImGuiFileDialog::Instance()->Close();
   }
   else if (ImGuiFileDialog::Instance()->Display("choose file to write", ImGuiWindowFlags_NoCollapse || ImGuiWindowFlags_AlwaysAutoResize, ImVec2(200, 200))) {
      if (ImGuiFileDialog::Instance()->IsOk()) { 
         std::string name = ImGuiFileDialog::Instance()->GetFilePathName();
         save_file(name);
      }
      ImGuiFileDialog::Instance()->Close();
   }
}
void Editor::read_file(std::string& filename){
   std::ifstream ifs(filename);
   if (ifs.is_open()) utils::log_info(INFO, "open file to read %s", filename.c_str());
   else utils::log_info(ERROR, "failed to open file to read %s", filename.c_str());

   parse_xml(filename, &layout_settings, &elements);
   ifs.close();
}



void Editor::edit_element(){
   ImGui::BeginChild("MENU");

   static char input_buffer[256];
   static char id_buffer[256];
   static bool is_moving = false;
   if (editing_index != -1){
      // FIXME: based on current layout
      // i.e for relative: add siblings
      // for constraint:   add parents 
      // for frame:       fixed coordinates
      if (layout_settings.type == frame_layout){
         static ImVec2 pos;
         ImGui::InputFloat2("Position on canvas", &pos.x, "%.3f");
         elements[editing_index].pos = pos;
      } 
      if(ImGui::InputText("edit id", id_buffer, IM_ARRAYSIZE(input_buffer))){
         elements[editing_index].id = id_buffer;
      }
      if(ImGui::InputText("edit text", input_buffer, IM_ARRAYSIZE(input_buffer))){
         elements[editing_index].text = input_buffer;
      }
      if (elements[editing_index].type == "EditText"){
         const char* types[] = {"text", "number", "phone", "datetime", "date", "time", "numberPassword"};
         static int indx = 0;
         ImGui::Combo("input types", &indx, types, IM_ARRAYSIZE(types));
         elements[editing_index].input_type = types[indx];
         static char buffer[256];
         if (ImGui::InputText("hint (placeholder)", buffer, IM_ARRAYSIZE(buffer))){
            elements[editing_index].hint = buffer;
         }
      }
      if (ImGui::Button("OK")){
         if (*id_buffer == 0) { elements[editing_index].id = "my_id"; }
         editing_index = -1;
         *input_buffer = 0;
         *id_buffer = 0;
      } ImGui::SameLine();
      if (ImGui::Button("delete")){
         elements.erase(elements.begin() + editing_index);
         editing_index = -1;
         *input_buffer = 0;
         *id_buffer = 0;
      }

   }
      
   ImGui::EndChild();
}

