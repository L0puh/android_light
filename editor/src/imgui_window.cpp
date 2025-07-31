#include "editor.h"
#include "imgui.h"


namespace imgui {

   GLFWwindow* init() {

      GLFWwindow* window;

      if (!glfwInit()) { 
         error_and_exit("glfw init failed");
      }

      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
      window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), 
            "Android editor", nullptr, nullptr);

      if (window == NULL) error_and_exit("window init failed");

      glfwMakeContextCurrent(window);
      glfwSwapInterval(1);

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

      ImGui::StyleColorsDark();

      ImGuiStyle& style = ImGui::GetStyle();
      style.ScaleAllSizes(main_scale);
      style.FontScaleDpi = main_scale;
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init();

      glfwSetKeyCallback(window, utils::key_callback);
      glfwSetErrorCallback(utils::glfw_error_callback);
      glfwSetFramebufferSizeCallback(window, utils::framebuffer_callback);

      return window;
   }



   void cleanup(GLFWwindow* window) {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

      glfwDestroyWindow(window);
      glfwTerminate();

   }

   void new_frame(){
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

   }
   void render(GLFWwindow* window, ImVec4 clear_color) {
      ImGui::Render();
      glClearColor(clear_color.x * clear_color.w, clear_color.y *
            clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window);
   }

};
