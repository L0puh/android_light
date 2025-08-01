#include "editor.h"


const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

namespace utils {

   void log_info(log_type type, const char* format, ...) {
      std::string color, prefix;
      switch (type) {
         case INFO:
            color = BLUE;
            prefix = "INFO";
            break;
         case ERROR:
            color = RED;
            prefix = "ERROR";
            break;
         case WARN:
            color = YELLOW;
            prefix = "WARN";
            break;
            break;
      }

      va_list args;
      printf("%s[%s] ", color.c_str(), prefix.c_str());
      va_start(args, format);
      vprintf(format, args);
      va_end(args);
      printf("%s\n", RESET.c_str());
      if (type == ERROR){
         exit(-1);
      }
   }

   void key_callback(GLFWwindow* window, int key, int code, int action, int mods){
      switch(key) {
         case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            return;
      }
   }

   void glfw_error_callback(int error, const char* description){
      log_info(ERROR, "GLFW ERROR: %d: %s\n", error, description);
   }

   void framebuffer_callback(GLFWwindow* w, int width, int height){
      glViewport(0, 0, width, height);
   }

};
