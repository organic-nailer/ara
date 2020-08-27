#pragma once

#define SK_GL

#include "GLFW/glfw3.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/core/SkFont.h"
#include "include/effects/Sk2DPathEffect.h"

class AraWindow {
private:
	GLFWwindow* glWindow;
	GrContext* sContext;
	SkSurface* sSurface;
	void initGlfw();
	void initSkia();
	//void errorCallback(int error, const char* description);
	//void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	int windowWidth;
	int windowHeight;
	AraWindow(int w, int h);
	~AraWindow();
	void draw();
	void pollEvents();
	void swapBuffers();
	bool shouldClose();
	void drawRect(int left, int top, int right, int bottom, int32_t color);
	void drawText(const char* text, int left, int top);
	void clearWhite();
};
