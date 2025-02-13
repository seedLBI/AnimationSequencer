#ifndef ENGINE_H
#define ENGINE_H
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window/Manager/WindowManager.h"

enum BufferBit {
    COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
    DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
    STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
};

class Engine
{
public:
	Engine();
	Engine(const std::string& title, glm::vec2 sizeWindow);
	~Engine();

	void EnableVSync();
	void DisableVSync();
	void ToggleVSync();

	double GetDeltaTime();
	void SetTargetFPS(const int& target);
	int GetTargetFPS();


	glm::vec2 GetMonitorScreenSize();

	void BeginDraw();
	void EndDraw();

	WindowManager* GetWindowManager();

private:
	bool vsyncMode = false;
	WindowManager* windowManager = nullptr;


	struct FPSLimiter {
        double Start = 0.0, End = 0.0, Delta = 0.0;

        LARGE_INTEGER frequency;
        LARGE_INTEGER lastTime;
        double frameTime = 0.0;
        double targetFPS = 120.0;
        
        FPSLimiter();
        ~FPSLimiter();
        void wait();

	} TimerDrawing;


};

#endif
