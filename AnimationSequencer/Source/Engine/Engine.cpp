#include "Engine.h"


Engine::Engine() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	windowManager = new WindowManager("Empty name", 800, 600, 100, 100);
}

Engine::Engine(const std::string& title, glm::vec2 sizeWindow) {
	glfwInit();
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	windowManager = new WindowManager(title, static_cast<int>(sizeWindow.x), static_cast<int>(sizeWindow.y), 100, 100);
}

Engine::~Engine() {
	windowManager->~WindowManager();
	std::cout << "Engine::~Engine()\n";
	glfwTerminate();
}

WindowManager* Engine::GetWindowManager() {
	return windowManager;
}

double Engine::GetDeltaTime() {
	return TimerDrawing.Delta;
}
void Engine::BeginDraw() {
	TimerDrawing.Start = glfwGetTime();
}

void Engine::EndDraw() {
	glfwSwapBuffers(glfwGetCurrentContext());
	glfwPollEvents();

	TimerDrawing.wait();
	TimerDrawing.End = glfwGetTime();
	TimerDrawing.Delta = TimerDrawing.End - TimerDrawing.Start;

}

void Engine::SetTargetFPS(const int& target) {
	TimerDrawing.targetFPS = target;
	TimerDrawing.frameTime = 1.0 / TimerDrawing.targetFPS;
}
int Engine::GetTargetFPS() {
	return TimerDrawing.targetFPS;
}


void Engine::EnableVSync() {
	vsyncMode = true;
	glfwSwapInterval(1);
}
void Engine::DisableVSync() {
	vsyncMode = false;
	glfwSwapInterval(0);
}
void Engine::ToggleVSync() {
	vsyncMode = !vsyncMode;

	if (vsyncMode)
		EnableVSync();
	else
		DisableVSync();
}


glm::vec2 Engine::GetMonitorScreenSize() {
	static glm::vec2 screenSize(0, 0);
	if (screenSize.x != 0)
		return screenSize;

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		std::cerr << "Failed to get primary monitor" << std::endl;
	}

	// Get the monitor's video mode
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
	if (!mode) {
		std::cerr << "Failed to get video mode" << std::endl;
	}

	screenSize.x = mode->width;
	screenSize.y = mode->height;

	return screenSize;
}


Engine::FPSLimiter::FPSLimiter() {
	frameTime = 1.0 / targetFPS;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&lastTime);
	timeBeginPeriod(1);
}
Engine::FPSLimiter::~FPSLimiter() {
	timeEndPeriod(1);
}

void Engine::FPSLimiter::wait() {
	if (targetFPS == 0)
		return;

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double elapsedTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

	if (elapsedTime < frameTime) {
		DWORD sleepTime = static_cast<DWORD>((frameTime - elapsedTime) * 1000.0);
		if (sleepTime != 0)
			--sleepTime;
		if (sleepTime > 0)
			Sleep(sleepTime);

		do {
			QueryPerformanceCounter(&currentTime);
			elapsedTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
		} while (elapsedTime < frameTime);
	}

	lastTime = currentTime;
}