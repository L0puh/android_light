#include "editor.h"
#include "imgui.h"
#include <string>

void Editor::edit_item_frame_layout(){
}
void Editor::edit_item_contraint_layout(){
}
void Editor::edit_item_relative_layout(){

   static int align = 0;
   static bool align_to = 0;
   static char below[256] = "";
   static char above[256] = "";
   static char left_to[256] = "";
   static char right_to[256] = "";
   static char end_to[256] = "";
   static char start_to[256] = "";

   // TODO: add ability to click on object and get the id to align to
   if (editing_index != -1){

      relative_attr_t attr = elements[editing_index].attr.relative;
      ImGui::RadioButton("align parent top", &align, 0);
      ImGui::RadioButton("align parent left", &align, 1);
      ImGui::RadioButton("align parent right", &align, 2);
      ImGui::RadioButton("align parent bottom", &align, 3);
      attr.align_parent_top    = 0;
      attr.align_parent_left   = 0;       
      attr.align_parent_right  = 0;
      attr.align_parent_bottom = 0;      
      
      if (align == 0) attr.align_parent_top = 1;
      if (align == 1) attr.align_parent_left = 1;
      if (align == 2) attr.align_parent_right = 1;
      if (align == 3) attr.align_parent_bottom = 1;

      if (ImGui::InputText("id below", below, IM_ARRAYSIZE(below))){
         attr.layout_below = below;
      }
      if (ImGui::InputText("id above", above, IM_ARRAYSIZE(above))){
         attr.layout_above = above;
      }
      if (ImGui::InputText("id to left of", left_to, IM_ARRAYSIZE(left_to))){
         attr.to_left_of = left_to;
      }
      if (ImGui::InputText("id to right of", right_to, IM_ARRAYSIZE(right_to))){
         attr.to_right_of = right_to;
      }
      if (ImGui::InputText("id to end of", end_to, IM_ARRAYSIZE(end_to))){
         attr.to_end_of = end_to;
      }
      if (ImGui::InputText("id to start of", start_to, IM_ARRAYSIZE(start_to))){
         attr.to_start_of = start_to;
      }
      
      elements[editing_index].attr.relative = attr;
   }

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
      "center_horizontal", "top", "bottom"};
   static int orientation_indx = 0, gravity_indx = 0;
   layout_t layout;
   ImGui::Combo("orientation", &orientation_indx, orients, IM_ARRAYSIZE(orients));
   ImGui::Combo("gravity", &gravity_indx, gravity, IM_ARRAYSIZE(gravity));
   layout.orientation = orients[orientation_indx];
   layout.gravity = gravity[gravity_indx];
   
   this->layout_settings = layout;

}
