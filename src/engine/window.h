#ifndef WINDOW_H
#define WINDOW_H

#include "engine_config.h"
#include <GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void initialize_glfw();
GLFWwindow *make_window();

#endif

