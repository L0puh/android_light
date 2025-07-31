#include "editor.h"


Editor* Editor::instance = new Editor();

int main() {

   GLFWwindow* window = imgui::init();
   if (window == NULL) {
      error_and_exit("init failed");
   }
   ImVec4 clear_color = ImVec4(0.573, 0.573, 0.71, 0.5f);

   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
      imgui::new_frame(); 
      Editor::get_instance()->draw();
      imgui::render(window, clear_color);
   }

   imgui::cleanup(window);

   return 0;
}
