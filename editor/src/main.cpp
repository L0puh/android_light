#include "editor.h"

int main() {

   GLFWwindow* window = imgui::init();
   if (window == NULL) {
      error_and_exit("init failed");
   }
   ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.94f);

   while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
      imgui::new_frame(); 
      imgui::render(window, clear_color);
   }

   imgui::cleanup(window);

   
   return 0;
}
