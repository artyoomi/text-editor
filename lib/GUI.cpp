#include "GUI.h"

#include <cstdio>

#include <iostream>
#include <sstream>

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "DroidSans_ttf.h"

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void GUI::run()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Editor!!!", nullptr, nullptr);
    if (window == nullptr)
        throw std::runtime_error("Failed to create window");
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Add RU font
    io.Fonts->AddFontFromMemoryCompressedTTF(DroidSans_compressed_data, DroidSans_compressed_size, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Main Loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        showMainMenu(window);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
#ifdef NDEBUG
        glClearColor(0.15f, 0.65f, 0.60f, 1.00f);  // Hatsune Miku????
#else
        glClearColor(0.65f, 0.30f, 0.30f, 1.00f);  // more red if debug build
#endif
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void GUI::showMainMenu(GLFWwindow *window)
{
    // Calculate the screen size
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    // Define window sizes and positions
    ImVec2 bigTextWindowSize(screenSize.x * 0.5f, screenSize.y * 0.5f);
    ImVec2 operationsWindowSize(screenSize.x * 0.5f, screenSize.y);
    ImVec2 stackWindowSize(screenSize.x * 0.5f, screenSize.y * 0.5f);
    

    // ---------------------------------
    // TEXT WINDOW
    // ---------------------------------
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Position at top-left
    ImGui::SetNextWindowSize(bigTextWindowSize); // Set size to half the screen
    ImGui::Begin("Text Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    std::string textDisplay = _rope.to_string();

    // Show field with text
    ImGui::BeginChild("##multilineoutput0", ImVec2(0, 0), true);
    ImGui::TextUnformatted(textDisplay.c_str());
    ImGui::EndChild();

    ImGui::End(); // Text Window end


    // ---------------------------------
    // UNDO / REDO WINDOW
    // ---------------------------------
    ImGui::SetNextWindowPos(ImVec2(0, screenSize.y * 0.5f)); // Position at bottom-left
    ImGui::SetNextWindowSize(stackWindowSize); // Set size to half the screen
    ImGui::Begin("Undo / Redo", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    std::vector<op> ops_vec = _history.get_list().to_vector();

    // Show Difference List
    ImGui::Text("Difference List:");
    if (!ops_vec.empty()) {
        int current_index = _history.get_current_index();
        // std::cout << "Current pointer to oper is " << current_index << std::endl;

        // we need to output this vector in deque-like view, so we
        // will show elements of vector in reverse order
        for (int i = ops_vec.size() - 1; i >= 0; --i) {
            ImGui::BeginChild("Separator0", ImVec2(0, 0));
            ImGui::Separator();

            if (i == current_index)
                ImGui::Text("-> %s (%d)", ops_vec[i].to_string().c_str(), i + 1);
            else
                ImGui::Text("%s (%d)", ops_vec[i].to_string().c_str(), i + 1);

            ImGui::Separator();
            ImGui::EndChild();
        }
    } else {
        ImGui::Text("Empty");
    }

    ImGui::End(); // Undo / Redo Window end


    // --------------------------------------
    // HANDLE OPERATIONS WINDOW
    // --------------------------------------
    ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, 0)); // Position at top-right
    ImGui::SetNextWindowSize(operationsWindowSize); // Size same as the text window
    ImGui::Begin("Operations", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    static char appendBuffer[255] = "";

    static char insIndex[255] = "";
    static char insText[255]  = "";

    static char delBegIndex[255] = "";
    static char delEndIndex[255] = "";

    static char findPattern[255] = "";
    static std::string occurences_str;

    ImGui::Text("%s\n", "Input text to append:");
    ImGui::InputTextMultiline("##multilineinput0", appendBuffer, IM_ARRAYSIZE(appendBuffer), ImVec2(-1, 200), ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Append") && strlen(appendBuffer) > 0) {
        // std::cerr << "Append buffer after input: " << appendBuffer << '\n';
        std::size_t old_rope_length = _rope.length();

        //_rope.append(std::string(appendBuffer));
        _history.add(op(op_type::INSERT, std::string(appendBuffer), old_rope_length, old_rope_length + strlen(appendBuffer)), _rope);
        appendBuffer[0] = '\0';
    }

    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("%s\n", "Input index for insert:");
    ImGui::InputText("##singlelineinput0", insIndex, IM_ARRAYSIZE(insIndex));
    ImGui::Text("%s\n", "Input text for insert:");
    ImGui::InputTextMultiline("##multilineinput1", insText, IM_ARRAYSIZE(insText), ImVec2(-1, 200), ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Insert") && strlen(insText) > 0) {
        try {
            std::size_t insIndexInt = std::stoi(insIndex);

            //_rope.insert(std::stoi(insIndex), std::string(insText));
            _history.add(op(op_type::INSERT, std::string(insText), insIndexInt, insIndexInt + strlen(insText)), _rope);
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }

        insIndex[0] = '\0';
        insText[0] = '\0';
    }

    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("%s\n", "Input start index of deletion:");
    ImGui::InputText("##singlelineinput1", delBegIndex, IM_ARRAYSIZE(delBegIndex));
    ImGui::Text("%s\n", "Input end index of deletion:");
    ImGui::InputText("##singlelineinput2", delEndIndex, IM_ARRAYSIZE(delEndIndex));
    
    if (ImGui::Button("Delete") && strlen(delBegIndex) > 0 && strlen(delEndIndex) > 0) {
        try {
            std::size_t beg = std::stoi(delBegIndex);
            std::size_t end = std::stoi(delEndIndex);

            if (end < beg)
                throw std::invalid_argument("Wrong beg and end index, try again");

            std::string delText = _rope.substr(beg, end - beg);

            _history.add(op(op_type::DELETE, delText, beg, end), _rope);
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }

        delBegIndex[0] = '\0';
        delEndIndex[0] = '\0';
    }

    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::Text("%s\n", "Input pattern to find in text:");
    ImGui::InputText("##singlelineinput3", findPattern, IM_ARRAYSIZE(findPattern));
    
    if (ImGui::Button("Find") && strlen(findPattern) > 0) {
        occurences_str.clear();
        try {
            std::vector<int> occurences = rabin_karp(std::string(findPattern), _rope.to_string());
            
            std::stringstream ss;
            for (std::size_t i = 0, size = occurences.size(); i < size; ++i) {
                ss << occurences[i] << ' ';
            }
            
            occurences_str = ss.str();
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }

        findPattern[0] = '\0';
    }
    if (!occurences_str.empty())
        ImGui::Text("Occurences: %s", occurences_str.c_str());

    ImGui::End(); // Operations Window end


    // ----------------------------------------
    // HANDLE CTRL + Z and CTRL + SHIFT + Z
    // ----------------------------------------
    ImGuiIO& io = ImGui::GetIO();
    
    // Check for Ctrl + Shift + Z (Redo)
    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
    {
        // Logic for Ctrl + Shift + Z
        _history.redo(_rope);
        
        // printf("Ctrl + Shift + Z pressed\n");
    }

    // Check for Ctrl + Z (Undo)
    else if (io.KeyCtrl && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
    {
        // Logic for Ctrl + Z
        _history.undo(_rope);

        // printf("Ctrl + Z pressed\n");
    }
}