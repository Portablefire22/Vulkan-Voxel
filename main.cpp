#include "main.h"

#include <iostream>

#include "world/Chunk.h"



Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool firstMouse = true;
float lastX;
float lastY;

int main() {
    try {
        //init::app->run();
        entryPoint::engine.init();
        entryPoint::engine.run();
        entryPoint::engine.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


