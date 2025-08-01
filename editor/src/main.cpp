#include "editor.h"

Editor* Editor::instance = new Editor();

int main() {

   GLFWwindow* window = imgui::init();
   if (window == NULL) {
      utils::log_info(ERROR, "init failed");
   } else 
      utils::log_info(INFO, "init window");
   ImVec4 clear_color = ImVec4(0, 0, 0, 0);

   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
      imgui::new_frame(); 
      Editor::get_instance()->draw();
      imgui::render(window, clear_color);
   }

   imgui::cleanup(window);

   return 0;
}
