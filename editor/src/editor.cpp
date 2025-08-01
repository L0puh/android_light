#include "editor.h"
#include "imgui.h"
#include <fstream>
#include <limits>
#include <string>
#include <vector>

 Editor* Editor::get_instance() {
   if (instance == NULL) {
      utils::log_info(ERROR, "init instance of editor before using");
   }
   return instance;
}

void Editor::draw() {
   ImVec2 canvas_pos;

   ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
   ImGui::Begin("EDITOR", nullptr, window_flags | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
      draw_menu();
      canvas_pos = draw_canvas();
      draw_elements(canvas_pos);
      edit_element();
   ImGui::EndGroup();
   ImGui::End(); 
}


void Editor::add_drag_and_drop(const char* type){
   ImGui::Button(type);
   if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){
      ImGui::SetDragDropPayload(type, &type, sizeof(const char*));
      ImGui::Text("Drag & drop widget");
      ImGui::EndDragDropSource();
   }
}
void Editor::add_item_popup(){
   if (ImGui::BeginPopup("Add item", ImGuiWindowFlags_AlwaysAutoResize)){

      add_drag_and_drop("TEXT");
      add_drag_and_drop("BUTTON");
      add_drag_and_drop("INPUT");
      ImGui::EndPopup();
   }
}

void Editor::save_file(std::string& filename){
   std::ofstream ofs(filename);
   if (ofs.is_open())
      utils::log_info(INFO, "writing to data to %s", filename.c_str());
   else {
      utils::log_info(WARN, "failed to write data to %s", filename.c_str());
      return;
   }

   serialize(ofs);
   ofs.close();
}

void Editor::create_new_file(){
   static char buffer[256];
   if (ImGui::BeginPopup("Create new file")){
      if (ImGui::InputText("Filename: ", buffer, sizeof(buffer))){
      }
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

   elements = deserialize(ifs);
   ifs.close();
}
void Editor::draw_menu(){

   ImGui::BeginChild("MENU", ImVec2(0, 0),
                     ImGuiChildFlags_AlwaysAutoResize | 
                     ImGuiChildFlags_AutoResizeX |
                     ImGuiChildFlags_AutoResizeY);
   ImGui::Text("Menu");
   ImGui::Separator();


   if (ImGui::Button("New")) {
      ImGui::OpenPopup("Create new file");
      elements.clear(); //TODO: ask before deleting

   }
  
   if (ImGui::Button("Open")) {
      IGFD::FileDialogConfig config;
      config.path = ".";
      ImGuiFileDialog::Instance()->OpenDialog("choose file to read", "Choose File", ".txt", config);
   }
  
   if (ImGui::Button("Add widget")) {
      ImGui::OpenPopup("Add item");
   }
  
   if (ImGui::Button("Delete widget")) {
   }
   if (ImGui::Button("Save xml file")) {
      IGFD::FileDialogConfig config;
      config.path = ".";
      ImGuiFileDialog::Instance()->OpenDialog("choose file to write", "Choose File", ".txt", config);
   }
   if (ImGui::Button("Settings")) {
   }
   
   create_new_file();
   open_file();
   add_item_popup();
   ImGui::EndChild();

}

void Editor::edit_element(){
   ImGui::BeginChild("MENU");
   
   static char input_buffer[256];
   if (editing_index != -1){
      if(ImGui::InputText("edit", input_buffer, IM_ARRAYSIZE(input_buffer))){
         elements[editing_index].id = input_buffer;
      }
      if (ImGui::Button("done")){
         editing_index = -1;
         *input_buffer = 0;
      }

   }
   ImGui::EndChild();
}

void Editor::draw_elements(ImVec2 w_pos){
   ImDrawList* draw_list = ImGui::GetWindowDrawList();
   ImVec2 mouse_pos = ImGui::GetMousePos(); 
   ImVec2 window_pos = ImGui::GetWindowPos(); 
   ImVec2 relative_mouse_pos = ImVec2(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);

   int indx = 0;
   for (auto& el : elements) {
      ImVec2 pos = ImVec2(w_pos.x + el.pos.x, w_pos.y + el.pos.y);
      ImVec2 text_size = ImGui::CalcTextSize(el.id.c_str());
      ImVec2 rect_end = ImVec2(pos.x + text_size.x + 9.0f, pos.y + 40);

      draw_list->AddRectFilled(pos, rect_end, color::blue, 1.0f);
      draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
            ImVec2(pos.x + 5, pos.y + 10),
            color::white, el.id.c_str());

      bool is_mouse_over = (relative_mouse_pos.x >= pos.x && relative_mouse_pos.x <= rect_end.x &&
            relative_mouse_pos.y >= pos.y && relative_mouse_pos.y <= rect_end.y);

      if (ImGui::IsMouseClicked(0) && is_mouse_over && editing_index == -1) {
         editing_index = indx;
      }
      indx++;
   }
}


ImVec2 Editor::draw_canvas(){
   ImDrawList* draw_list = ImGui::GetWindowDrawList();
   ImGui::SameLine();
   ImGui::BeginGroup();
   ImGui::Text("Canvas");
   ImGui::Separator();

   ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
   ImVec2 canvas_size = phone_size;
   ImVec2 mouse_pos = ImVec2(ImGui::GetMousePos().x - canvas_pos.x, ImGui::GetMousePos().y - canvas_pos.y);

   ImGui::InvisibleButton("canvas", phone_size);
   bool hovered = ImGui::IsItemHovered();
   
   ImVec2 sz = ImVec2(canvas_size.x + canvas_pos.x, canvas_size.y + canvas_pos.y);
   draw_list->AddRectFilled(canvas_pos, sz, color::grey); 
   draw_list->AddRect(canvas_pos, sz ,color::white , 0.0f, ImDrawFlags_None, 2.0f); 

   if (ImGui::BeginDragDropTarget()){
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXT")){
         elements.push_back({"TEXT ITEM", "TEXT", mouse_pos});
      }
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BUTTON")){
         elements.push_back({"BUTTON ITEM", "BUTTON", mouse_pos});
      }
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INPUT")){
         elements.push_back({"INPUT ITEM", "INPUT", mouse_pos});
      }
      ImGui::EndDragDropTarget();
   }
   
   return canvas_pos;
}


void Editor::serialize(std::ostream& os){
   os << elements.size() << "\n";
   for (const auto& e: elements){
      os << e.id << "\n" << e.pos.x << " " << e.pos.y << "\n" << e.type << "\n";
   }
}

std::vector<element_t> Editor::deserialize(std::istream& is){
   std::vector<element_t> e;
   size_t size = 0;
   is >> size;
   is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   for (int i = 0; i < size; i ++){
      element_t element;
      std::getline(is, element.id);
      is >> element.pos.x >> element.pos.y;
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::getline(is, element.type);
      e.push_back(element);
   }

   return e;
}


