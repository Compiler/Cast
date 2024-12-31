#include <iostream>
#include <thread>
#include <chrono>
#include <Core.h>

int main() {
    //Error is due to opengl stuff called before its ready.
    Core engineCore{};
    if(engineCore.init() != 0) return -1;
    float targetFPS = 250;
    int frequency = targetFPS;
    float frameLengths;
    float fpss;
    int count = 1;
    std::cout << "Init done, moving to render/update loop\n";
    while (!engineCore.shouldClose()) {
        auto now = std::chrono::high_resolution_clock::now();

        engineCore.update();
        engineCore.render();

        auto delta = std::chrono::high_resolution_clock::now() - now;
        float frameTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() ;
        float frameTimeMcs = std::chrono::duration_cast<std::chrono::microseconds>(delta).count() ;
        float fps = 1000.0f / frameTimeMs;
        engineCore.setDelta(frameTimeMs);
        frameLengths += frameTimeMs;
        fpss += fps;
        count ++;
        if(count == frequency){
            std::cout << "Frame length: \t" << frameLengths / frequency << "ms\t(" << frameTimeMs << ") " << "ms\t Frame Length Micro: " << frameTimeMcs << std::endl;
            std::cout << "FPS: \t\t" << std::round(fpss/frequency) << "\t(" << fps << ")" << std::endl;
            count = 1;

            frameLengths = 0;
            fpss = 0;
        }
        // Ensure each frame contributes equal time
        auto sleepDuration = (int)(1000 / targetFPS - frameTimeMs);
        if(sleepDuration > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
        }
    }
    std::cout << "Should close hit\n";
    engineCore.shutdown();

    return 0;
}

