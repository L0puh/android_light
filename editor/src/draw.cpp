#include "editor.h"
#include "imgui.h"


void Editor::draw() {

   ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
   ImGui::Begin("EDITOR", nullptr, window_flags | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
      draw_menu();
      draw_canvas();
      draw_elements();
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

      add_drag_and_drop("TextView");
      add_drag_and_drop("Button");
      add_drag_and_drop("InputText");
      ImGui::EndPopup();
   }
}

void Editor::open_accept_if_any(){
   if (ImGui::BeginPopup("Accept", ImGuiWindowFlags_AlwaysAutoResize)){
      ImGui::Text("Are you sure?");
      if (ImGui::Button("Yes")){
         accept_flag = true;
         ImGui::CloseCurrentPopup();
      } 
      if (ImGui::Button("No")){
         accept_flag = false;
         ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
   }
}
void Editor::draw_menu(){

   ImGui::BeginChild("MENU", ImVec2(0, 0),
                     ImGuiChildFlags_AlwaysAutoResize | 
                     ImGuiChildFlags_AutoResizeX |
                     ImGuiChildFlags_AutoResizeY);
   ImGui::Text("Menu");
   ImGui::Separator();


   if (ImGui::Button("New") || accept_flag) {
      ImGui::OpenPopup("Accept");
      if (accept_flag) {
         accept_flag = false;
         ImGui::OpenPopup("Create new file");
         elements.clear(); 
      }
   }
  
   if (ImGui::Button("Open")) {
      IGFD::FileDialogConfig config;
      config.path = ".";
      ImGuiFileDialog::Instance()->OpenDialog("choose file to read", "Choose File", ".xml", config);
   }
  
   if (ImGui::Button("Add widget")) {
      ImGui::OpenPopup("Add item");
   }
  
   if (ImGui::Button("Delete widget")) {
   }
   if (ImGui::Button("Save xml file")) {
      IGFD::FileDialogConfig config;
      config.path = ".";
      ImGuiFileDialog::Instance()->OpenDialog("choose file to write", "Choose File", ".xml", config);
   }
   if (ImGui::Button("Settings")) {
   }
   
   create_new_file();
   open_file();
   add_item_popup();
   layout_menu();
   open_accept_if_any();
   ImGui::EndChild();

}

ImVec2 Editor::get_aligment_pos(std::string& type, ImVec2 text_size, ImVec2 canvas_pos){
   ImVec2 pos = canvas_pos;
   if (type == "center"){
      pos.x += (phone_size.x - text_size.x) / 2.0f;
      pos.y += (phone_size.y - text_size.y) / 2.0f;
      return pos;
   }
   if (type == "center_vertical"){
      pos.y += (phone_size.y - text_size.y) / 2.0f;
      return pos;
   }
   if (type == "center_horizontal"){
      pos.x += (phone_size.x - text_size.x) / 2.0f;
      return pos;
   }
   if (type == "top"){
      return pos;
   }
   if (type == "bottom"){
      pos.y = canvas_pos.y + phone_size.y - text_size.y - 10.0f;
      return pos;
   }
   return pos;
}

void Editor::update_relative(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos){
   ImVec2 text_size; 
   text_size = ImGui::CalcTextSize(el.text.c_str());
  
   //FIXME: stacking of widgets is wrong:
   if (el.attr.relative.align_parent_bottom){
      pos->x = current_pos->x;
      pos->y = current_pos->y + phone_size.y - text_size.y * 4.0f;
   } else if (el.attr.relative.align_parent_right) {
      pos->x = current_pos->x + phone_size.x - text_size.x;
      pos->y = current_pos->y  + phone_size.y/2.0f;
   }
   else if (el.attr.relative.align_parent_left) {
      pos->x = current_pos->x;
      pos->y = current_pos->y  + phone_size.y/2.0f;
   }
   else {
      *pos = *current_pos;
   }

   if (el.attr.relative.layout_above != ""){
      element_t above = find_widget_by_id(el.attr.relative.layout_above);
      pos->x = current_pos->x;
      pos->y = above.pos.y - text_size.y * 5;

      //FIXME:
      ImGui::GetWindowDrawList()->AddLine(*pos, above.pos, color::white, 2.0f); 
   }

   if (el.attr.relative.layout_below != ""){
      element_t below= find_widget_by_id(el.attr.relative.layout_above);
      pos->x = current_pos->x;
      pos->y = below.pos.y + text_size.y * 5;
   }
        


   if (layout_settings.orientation == "horizontal"){
      current_pos->x += text_size.x * 2.0f;
   } else {
      current_pos->y += text_size.y * 4.0f;
   }
   *rect_end = ImVec2(pos->x + text_size.x + 9.0f, pos->y + text_size.y * 3.0f);
}

void Editor::update_linear(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos){
   float margin;
   ImVec2 text_size;
   if (el.attr.linear.width_int != -1 && el.attr.linear.height_int != -1){
      text_size = ImVec2(el.attr.linear.height_int, el.attr.linear.width_int);
   } else 
      text_size = ImGui::CalcTextSize(el.text.c_str());

   margin = el.attr.linear.margin_int;
   pos->x = margin; 
   pos->y = margin;

   ImVec2 p = get_aligment_pos(layout_settings.gravity, text_size, *current_pos); 
   pos->x += p.x;
   pos->y += p.y;
   if (layout_settings.orientation == "horizontal"){
      current_pos->x += text_size.x * 2.0f + margin;
   } else {
      current_pos->y += text_size.y * 4.0f + margin;
   }
   *rect_end = ImVec2(pos->x + text_size.x + 9.0f, pos->y + text_size.y * 3.0f);
}
void Editor::update_element(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos){

   if (current_layout == linear_layout) update_linear(el, current_pos, rect_end, pos);
   if (current_layout == relative_layout) update_relative(el, current_pos, rect_end, pos);
}

void Editor::draw_based_on_type(ImDrawList* draw_list, element_t element, ImVec2 pos, ImVec2 rect_end){
   if (element.type == "Button"){
      draw_list->AddRectFilled(pos, rect_end, color::blue, 1.0f); 
   }
   if (element.type == "TextView"){
      draw_list->AddRectFilled(pos, rect_end, color::red, 1.0f); 
   }
   if (element.type == "InputText"){
      draw_list->AddRectFilled(pos, rect_end, color::green, 1.0f); 
      draw_list->AddRect(pos, rect_end, color::white, 1.0f, ImDrawFlags_None, 3.0f);
      draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(pos.x + 5, pos.y + 10.0f),
            color::grey, element.text.c_str());

      return;
   }
   draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
         ImVec2(pos.x + 5, pos.y + 10),
         color::white, element.text.c_str());
}

void Editor::draw_elements(){
   ImDrawList* draw_list = ImGui::GetWindowDrawList();
   ImVec2 mouse_pos = ImGui::GetMousePos(); 
   ImVec2 window_pos = ImGui::GetWindowPos(); 
   ImVec2 relative_mouse_pos = ImVec2(mouse_pos.x - window_pos.x, mouse_pos.y - window_pos.y);
   ImVec2 current_pos = canvas_pos;
   int indx = 0;
   for (auto& el : elements) {
      ImVec2 rect_end, pos;
      update_element(el, &current_pos, &rect_end, &pos);
      el.rect_end = rect_end;
      draw_based_on_type(draw_list, el, pos, rect_end);

      bool is_mouse_over = (relative_mouse_pos.x >= pos.x && relative_mouse_pos.x <= rect_end.x &&
            relative_mouse_pos.y >= pos.y && relative_mouse_pos.y <= rect_end.y);

      if (ImGui::IsMouseClicked(0) && is_mouse_over && editing_index == -1) {
         editing_index = indx;
      }
      indx++;
   }
}


void Editor::layout_menu(){
   const char* l[] = { "linear", "relative", "constraint", "frame"};

   ImGui::SeparatorText("Layout attributes");
   edit_window_layout();
   ImGui::Combo("layout", &current_layout, l, IM_ARRAYSIZE(l));

   switch((layout_type)current_layout){
      case linear_layout:
         edit_item_linear_layout();
         break;
      case relative_layout:
         edit_item_relative_layout();
         break;
      case constraint_layout:
         edit_item_contraint_layout();
         break;
      case frame_layout:
         edit_item_frame_layout();
         break;
   }
}

ImVec2 Editor::draw_canvas(){
   ImDrawList* draw_list = ImGui::GetWindowDrawList();
   ImGui::SameLine();
   ImGui::BeginGroup();
   ImGui::Text("Canvas");
   ImGui::Separator();

   canvas_pos = ImGui::GetCursorScreenPos();
   ImVec2 canvas_size = phone_size;
   ImVec2 mouse_pos = ImVec2(ImGui::GetMousePos().x - canvas_pos.x, ImGui::GetMousePos().y - canvas_pos.y);


   ImGui::InvisibleButton("canvas", phone_size);
   bool hovered = ImGui::IsItemHovered();
   
   ImVec2 sz = ImVec2(canvas_size.x + canvas_pos.x, canvas_size.y + canvas_pos.y);
   draw_list->AddRectFilled(canvas_pos, sz, color::grey); 
   draw_list->AddRect(canvas_pos, sz ,color::white , 0.0f, ImDrawFlags_None, 2.0f); 

   if (ImGui::BeginDragDropTarget()){
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TextView")){
         elements.push_back({"my_id", "TextView", "my text", "white", mouse_pos});
      }
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Button")){
         elements.push_back({"my_id", "Button", "button", "white", mouse_pos});
      }
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("InputText")){
         elements.push_back({"my_id", "InputText", "input", "white", mouse_pos}); // FIXME: is it the right type?
      }
      ImGui::EndDragDropTarget();
   }
   
   return canvas_pos;
}
