#include "main.h"


#include "vk_engine.h"
#include "world/chunk.h"



Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX;
float lastY;

int main() {
    //init::app = new mainApp();
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


