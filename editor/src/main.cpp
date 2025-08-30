#include "editor.h"

Editor* Editor::instance = new Editor();
State*  State::instance  = new State();

int main(int argc, char* argv[]) {

   GLFWwindow* window = imgui::init();
   ImVec4 clear_color = ImVec4(0, 0, 0, 0);

   for (int i = 0; i < argc; i++){
      std::string arg = argv[i];
      if (arg == "-v") State::get_instance()->set_verbose(true);
      if (arg == "-d") State::get_instance()->set_debug(true);
   }

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
