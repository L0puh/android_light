#include "editor.h"
#include "imgui.h"
#include <string>

void Editor::edit_item_frame_layout(){
}
void Editor::edit_item_contraint_layout(){
}
void Editor::edit_item_relative_layout(){


}
void Editor::edit_item_linear_layout(){
   static bool input_size = false;
   static int size[] = {0, 0};
   if (editing_index != -1) {
      ImGui::Checkbox("intput size (in dp)", &input_size);
      if (input_size){
         ImGui::InputInt2("SIZE", size);
         std::string height = std::to_string(size[0]) + "dp";
         std::string width= std::to_string(size[1]) + "dp";
         elements[editing_index].attr.linear.height = height;
         elements[editing_index].attr.linear.width  = width;
         
         elements[editing_index].attr.linear.height_int = size[0];
         elements[editing_index].attr.linear.width_int  = size[1];

      } else {
         const char* items[] = { "wrap_content", "match_parent"};
         static int height_indx = 0, width_indx = 0;
         ImGui::Combo("height:", &height_indx, items, IM_ARRAYSIZE(items));
         ImGui::Combo("width:",  &width_indx, items, IM_ARRAYSIZE(items));
         elements[editing_index].attr.linear.height = items[height_indx];
         elements[editing_index].attr.linear.width  = items[width_indx];

         elements[editing_index].attr.linear.height_int = -1; 
         elements[editing_index].attr.linear.width_int  = -1; 
      }
      static int margin = 0;
      ImGui::InputInt("MARGIN", &margin);

      elements[editing_index].attr.linear.margin = std::to_string(margin) + "dp";
      elements[editing_index].attr.linear.margin_int = margin; 
   }


}

void Editor::edit_window_layout(){
   const char* orients[] = {"horizontal", "vertical"};
   const char* gravity[] = {"center", "center_vertical",
      "center_horizontal", "fill", "top", "bottom", "start", "end"};
   static int orientation_indx = 0, gravity_indx = 0;
   layout_t layout;
   ImGui::Combo("orientation", &orientation_indx, orients, IM_ARRAYSIZE(orients));
   ImGui::Combo("gravity", &gravity_indx, gravity, IM_ARRAYSIZE(gravity));
   layout.orientation = orients[orientation_indx];
   layout.gravity = gravity[gravity_indx];
   
   this->layout_settings = layout;

}
