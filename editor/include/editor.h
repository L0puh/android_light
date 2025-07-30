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


namespace utils {
   static void glfw_error_callback(int error, const char* description){
       fprintf(stderr, "GLFW error %d: %s\n", error, description);
   }
};

#endif /* ifndef EDITOR_H */
