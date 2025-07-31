#include "editor.h"


void Editor::draw() {

   draw_workspace();
   draw_menu();
}


void Editor::add_item_popup(){
   if (ImGui::BeginPopup("Add item", ImGuiWindowFlags_AlwaysAutoResize)){
      if(ImGui::Button("Text")){
      }
      if(ImGui::Button("Button")){
      }
      if(ImGui::Button("Input text")){
      }
      ImGui::EndPopup();
   }
}

void Editor::draw_menu(){
   ImVec2 display_size = ImGui::GetIO().DisplaySize;
   ImVec2 window_pos = ImVec2(0.0f, 10.0f);

   ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);

   ImGui::Begin("EDITOR", nullptr, window_flags |
         ImGuiWindowFlags_NoBackground |
         ImGuiWindowFlags_AlwaysAutoResize);
   if (ImGui::Button("New")) {
   }
   ImGui::SameLine();
   if (ImGui::Button("Open")) {
   }
   ImGui::SameLine();
   if (ImGui::Button("Add widget")) {
      ImGui::OpenPopup("Add item");
   }
   ImGui::SameLine();
   if (ImGui::Button("Delete widget")) {
   }
   ImGui::SameLine();
   if (ImGui::Button("Save xml file")) {
   }
   ImGui::SameLine();
   if (ImGui::Button("Settings")) {
   }

   add_item_popup();
   ImGui::End();

}

void Editor::draw_workspace(){
   ImVec2 phone_size = ImVec2(400, 800);
   ImVec2 display_size = ImGui::GetIO().DisplaySize;
   ImVec2 window_pos = ImVec2((display_size.x - phone_size.x)/2,
         (display_size.y - phone_size.y)/2);

   ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
   ImGui::SetNextWindowSize(phone_size, ImGuiCond_Always);


   ImGui::Begin("WORKSPACE", nullptr, window_flags);
   ImGui::End(); 
}

