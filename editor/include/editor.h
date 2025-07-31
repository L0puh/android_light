#ifndef EDITOR_H
#define EDITOR_H 

#include <cstdio>
#include <cstdlib>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

#include <GLFW/glfw3.h>


#define error_and_exit(msg) { printf("[-] error[%s:%s:%d]%s\n", __FILE__, __func__, __LINE__, msg); exit(-1);}

namespace imgui {

   GLFWwindow* init();
   void cleanup(GLFWwindow* window);
   void render(GLFWwindow* window, ImVec4 clear_color);
   void new_frame();


};


class Editor {
   protected:
      ImGuiWindowFlags window_flags = 
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoMove | 
               ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoCollapse;

      static Editor* instance;
      Editor() {}
   public:

      static Editor* get_instance() {
         if (instance == NULL) {
            error_and_exit("Init instance of editor");
         }
         return instance;
      }

      void set_flag(ImGuiWindowFlags f) { window_flags |= f; }
      void remove_flag(ImGuiWindowFlags f) { window_flags &= ~f; }

      void draw();

   private:
      void add_item_popup();
      void draw_menu();
      void draw_workspace();
};


namespace utils {
   void glfw_error_callback(int error, const char* description);
   void key_callback(GLFWwindow* window, int key, int code, int action, int mods);
   void framebuffer_callback(GLFWwindow* w, int width, int height);

};

#endif /* ifndef EDITOR_H */
