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
   const uint red   = IM_COL32(255, 0, 0, 255);
   const uint green = IM_COL32(0, 255, 0, 255);
}

namespace imgui {

   GLFWwindow* init();
   void cleanup(GLFWwindow* window);
   void render(GLFWwindow* window, ImVec4 clear_color);
   void new_frame();
   void handle_keys(GLFWwindow* window);
};



struct linear_attr_t {
   std::string width;
   std::string height;
   std::string margin;

   int width_int  = -1;
   int height_int = -1;
   int margin_int = 0;
};
struct relative_attr_t{
   bool align_parent_top    = false;
   bool align_parent_left   = false;
   bool align_parent_right  = false;
   bool align_parent_bottom = false;

   std::string layout_below; /* specify ids of elements */
   std::string layout_above;
   std::string to_left_of;
   std::string to_right_of;
   std::string to_end_of;
   std::string to_start_of;
};

struct attributes_t {
   linear_attr_t linear;
   relative_attr_t relative;
};

struct element_t{
    std::string id;
    std::string type;
    std::string text="";
    std::string background;
    ImVec2 pos;
    ImVec2 rect_end;
    attributes_t attr;

    std::string input_type = "text"; // for EditText
    std::string hint = "enter text";                                 
};

enum layout_type {
   linear_layout,
   relative_layout,
   constraint_layout,
   frame_layout
};


struct layout_t {
   layout_type type = layout_type::linear_layout;
   std::string orientation = "vertical";
   std::string width = "match_parent";
   std::string height= "match_parent";
   std::string gravity="center";
   std::string background="white";
};

class State {
   public:
      float background_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
   protected:
      bool verbose_on = false;
      bool debug_mode = false;
      State() {} 
      static State* instance;
   public:
      static State* get_instance() {
         if (instance == NULL) instance = new State();
         return instance;
      }

      void set_verbose(bool on) { 
         this->verbose_on = on;
      }
      void set_debug(bool on) {
         this->debug_mode = on;
      }
      bool is_verbose() {
         return verbose_on;
      }
      bool is_debug() {
         return debug_mode;
      }


};

class Editor {

   std::vector<element_t> elements; 
   ImVec2 phone_size = ImVec2(420, 934);
   bool accept_flag = false;

   int editing_index = -1;
   ImVec2 canvas_pos;
   
   layout_t layout_settings;
   bool dragging = 0;
   
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
      void draw_based_on_type(ImDrawList* draw_list, element_t element, ImVec2 pos, ImVec2 rect_end);
      const ImVec2 get_phone_size() { return phone_size; }

   private:
     /* MENU */ 
      void add_item_popup();
      void draw_elements();
      void draw_menu();
      void draw_settings();
      void open_accept_if_any();
      void edit_element();
      ImVec2 draw_canvas();
      void add_drag_and_drop(const char* type);
      

   private:
      /* FILES */
      void save_file(std::string& filename);
      void read_file(std::string& filename);
      void open_file();
      void create_new_file();

   private:
      /* LAYOUTS */
      void layout_menu();
      void edit_window_layout();
      void edit_item_linear_layout();
      void edit_item_relative_layout();
      void edit_item_contraint_layout();
      void edit_item_frame_layout();
      ImVec2 get_aligment_pos(std::string& type, ImVec2 text_size, ImVec2 canvas);

   private:

      void update_element(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos);
      void update_linear(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos);
      void update_relative(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos);
      void update_frame(element_t el, ImVec2 *current_pos, ImVec2 *rect_end, ImVec2* pos);
      element_t find_widget_by_id(std::string);
};


void serialize_xml(const std::vector<element_t> data, std::string& filename, layout_t& layout);
void parse_xml(std::string& filename, layout_t *parsed_layout, std::vector<element_t> *parsed_elements);

enum log_type {
   INFO,
   ERROR,
   WARN
};
namespace utils {
   std::string get_hex_color(float color[4]);
   ImU32 get_color(float color[4]);
   void log_info(log_type type, const char* format, ...);
   void glfw_error_callback(int error, const char* description);
   void key_callback(GLFWwindow* window, int key, int code, int action, int mods);
   void framebuffer_callback(GLFWwindow* w, int width, int height);

};

#endif /* ifndef EDITOR_H */
