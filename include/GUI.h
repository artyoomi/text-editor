#ifndef GUI_H
#define GUI_H

#include <GLFW/glfw3.h>

#include "Rope/Rope.h"
#include "History.h"

class GUI {
public:
    void run();
    void showMainMenu(GLFWwindow *window);

private:
    Rope    _rope;
    History _history;
};

#endif