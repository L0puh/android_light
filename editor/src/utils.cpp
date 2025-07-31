#include "editor.h"


namespace utils {

   void key_callback(GLFWwindow* window, int key, int code, int action, int mods){
      switch(key) {
         case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            return;
      }
   }

   void glfw_error_callback(int error, const char* description){
       fprintf(stderr, "GLFW error %d: %s\n", error, description);
   }

   void framebuffer_callback(GLFWwindow* w, int width, int height){
      glViewport(0, 0, width, height);
   }

};
