#include "editor.h"
#include "imgui.h"
#include <string>

void print_attributes_relative(std::string cur_option, const relative_attr_t& attr);
void set_attr_option(std::string cur_option, std::string chosen_id, relative_attr_t* attr);


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

   if (editing_index != -1){
      ImVec2 mouse_pos = ImGui::GetMousePos(); 
      ImVec2 window_pos = ImGui::GetWindowPos(); 
      ImVec2 relative_mouse_pos = ImVec2(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);

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
      
      std::string options[] = { "id below", "id above", "id left of", "id right of", "id end of", "id start of"};
      static std::string chosen_id = "";
      static std::string cur_option = "";
      for (const std::string& o: options){
         if (ImGui::Button(o.c_str())){
            cur_option = o;
         }
         print_attributes_relative(o, attr);
      }
      if (cur_option == "") return; 
      else ImGui::Text("Click to an object to get id");
      for (int indx = 0; indx < elements.size(); indx++){
         element_t el = elements[indx];
         if(is_hovered(relative_mouse_pos, el.pos, el.rect_end) &&
                           layout_settings.type == relative_layout && editing_index !=- 1
                           && ImGui::IsMouseClicked(0) && editing_index != indx)
         {
            chosen_id = el.id;
         } 
      }
      if (cur_option != "" && chosen_id != ""){
         set_attr_option(cur_option, chosen_id, &attr);
         elements[editing_index].attr.relative = attr;
      }
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
void print_attributes_relative(std::string cur_option, const relative_attr_t& attr){
   if (cur_option == "id below"){
      ImGui::Text("%s", attr.layout_below.c_str());
      return;
   }
   if (cur_option == "id above"){
      ImGui::Text("%s", attr.layout_above.c_str());
      return;
   }
   if (cur_option == "id left of"){
      ImGui::Text("%s", attr.to_left_of.c_str());
      return;
   }
   if (cur_option == "id right_of"){
      ImGui::Text("%s", attr.to_right_of.c_str());
      return;
   }
   if (cur_option == "id end of"){
      ImGui::Text("%s", attr.to_end_of.c_str());
      return;
   }
   if (cur_option == "id start of"){
      ImGui::Text("%s", attr.to_start_of.c_str());
      return;
   }
}
void set_attr_option(std::string cur_option, std::string chosen_id, relative_attr_t* attr){
   if (cur_option == "id below"){
      attr->layout_below = chosen_id;
   }
   if (cur_option == "id above"){
      attr->layout_above= chosen_id;
   }
   if (cur_option == "id left of"){
      attr->to_left_of= chosen_id;
   }
   if (cur_option == "id right_of"){
      attr->to_right_of = chosen_id;
   }
   if (cur_option == "id end of"){
      attr->to_end_of = chosen_id;
   }
   if (cur_option == "id start of"){
      attr->to_start_of = chosen_id;
   }
}
