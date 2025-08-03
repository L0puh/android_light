#ifndef EDITOR_H
#define EDITOR_H 

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <GLFW/glfw3.h>

#include <ImGuiFileDialog.h>
#include <ImGuiFileDialogConfig.h>



namespace color {
   const uint grey  = IM_COL32(50, 50, 50, 255);
   const uint blue  = IM_COL32(0, 150, 255, 255);
   const uint white = IM_COL32(200, 200, 200, 255);
}

namespace imgui {

   GLFWwindow* init();
   void cleanup(GLFWwindow* window);
   void render(GLFWwindow* window, ImVec4 clear_color);
   void new_frame();
   void handle_keys(GLFWwindow* window);
};


struct element_t{
    std::string id;
    std::string type;
    std::string text="";
    ImVec2 pos;
};

class Editor {

   std::vector<element_t> elements; 
   const ImVec2 phone_size = ImVec2(420, 934);

   int editing_index = -1;
   ImVec2 canvas_pos;
   
   protected:
      ImGuiWindowFlags window_flags = 
               ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoMove | 
               ImGuiWindowFlags_NoTitleBar |
               ImGuiWindowFlags_NoCollapse;

      static Editor* instance;
      Editor() {}
   public:

      static Editor* get_instance();
      void set_flag(ImGuiWindowFlags f) { window_flags |= f; }
      void remove_flag(ImGuiWindowFlags f) { window_flags &= ~f; }

      void draw();
      const ImVec2 get_phone_size() { return phone_size; }

   private:
      
      void add_item_popup();
      void draw_elements();
      void draw_menu();
      void edit_element();
      ImVec2 draw_canvas();
      void add_drag_and_drop(const char* type);

      void save_file(std::string& filename);
      void read_file(std::string& filename);
      void open_file();
      void create_new_file();
};


void serialize_xml(const std::vector<element_t> data, std::string& filename);

enum log_type {
   INFO,
   ERROR,
   WARN
};
namespace utils {
   void log_info(log_type type, const char* format, ...);
   void glfw_error_callback(int error, const char* description);
   void key_callback(GLFWwindow* window, int key, int code, int action, int mods);
   void framebuffer_callback(GLFWwindow* w, int width, int height);

};

#endif /* ifndef EDITOR_H */
