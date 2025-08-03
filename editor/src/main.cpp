#include "editor.h"

Editor* Editor::instance = new Editor();

int main() {

   GLFWwindow* window = imgui::init();
   ImVec4 clear_color = ImVec4(0, 0, 0, 0);

   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();

      imgui::handle_keys(window);
      imgui::new_frame(); 
      Editor::get_instance()->draw();
      imgui::render(window, clear_color);
   
   }

   imgui::cleanup(window);

   return 0;
}
