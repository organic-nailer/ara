// ara.cpp : Defines the entry point for the application.
//

#include "ara.h"
#include "html_tokenizer.h"
#include "dom_generator.h"

#define SK_GL

#include "GLFW/glfw3.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/core/SkFont.h"
#include <stdio.h>
#include <stdlib.h>
#include "include/effects/Sk2DPathEffect.h"


using namespace std;

GrContext* sContext = nullptr;
SkSurface* sSurface = nullptr;

void errorCallback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void initSkia(int w, int h) {
 	GrContextOptions options;
	sContext = GrContext::MakeGL(nullptr, options).release();

	GrGLFramebufferInfo framebufferInfo;
	framebufferInfo.fFBOID = 0;
	framebufferInfo.fFormat = GL_RGBA8;

	SkColorType colorType;
	colorType = kRGBA_8888_SkColorType;
	GrBackendRenderTarget backendRenderTarget(w, h, 0, 0, framebufferInfo);

	sSurface = SkSurface::MakeFromBackendRenderTarget(
		sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr
	).release();
	if (sSurface == nullptr) abort();
}

void cleanupSkia() {
	delete sSurface;
	delete sContext;
}

const int kWidth = 960;
const int kHeight = 640;

int main(int argc, char *argv[])
{
	GLFWwindow* window;
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);

	window = glfwCreateWindow(kWidth, kHeight, "Simple Example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	initSkia(kWidth, kHeight);

	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyCallback);

	SkCanvas* canvas = sSurface->getCanvas();

	const char message[] = "Hello, Skia!";

	int angle = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		canvas->rotate(SkIntToScalar(++angle));
		SkPaint paint;
		paint.setColor(SK_ColorBLACK);
		//canvas->drawPaint(paint);
		SkScalar scale = 10.0f;
		SkPath path;
		static const int8_t pts[] = { 2, 2, 1, 3, 0, 3, 2, 1, 3, 1, 4, 0, 4, 1,
								 5, 1, 4, 2, 4, 3, 2, 5, 2, 4, 3, 3, 2, 3 };
		path.moveTo(2 * scale, 3 * scale);
		for (size_t i = 0; i < sizeof(pts) / sizeof(pts[0]); i += 2) {
			path.lineTo(pts[i] * scale, pts[i + 1] * scale);
		}
		path.close();
		SkMatrix matrix = SkMatrix::Scale(4 * scale, 4 * scale);
		paint.setPathEffect(SkPath2DPathEffect::Make(matrix, path));
		paint.setAntiAlias(true);
		canvas->clear(SK_ColorWHITE);
		SkRect bounds{ -4 * scale, -4 * scale, kWidth, kHeight };
		canvas->drawRect(bounds, paint);
		/*paint.setColor(SK_ColorBLUE);
		canvas->drawRect({ 100,200,300,500 }, paint);

		paint.setColor(SK_ColorRED);
		SkFont font;
		font.setSubpixel(true);
		font.setSize(72);
		canvas->drawSimpleText(message, strlen(message), SkTextEncoding::kUTF8, kWidth / 2, kHeight / 2, font, paint);
		*/
		sContext->flush();

		glfwSwapBuffers(window);
	}

	cleanupSkia();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
/*	cout << "Hello CMake." << endl;

	if (argc < 2) {
		cout << "too short arguments" << endl;
		//return -1;
	}
	string path = "C:\\Users\\hykwy\\Downloads\\abehiroshi.html"; //argv[1];

	auto input = ifstream(path);
	auto tokenizer = HtmlTokenizer(&input);

	auto generator = DomGenerator(&tokenizer);

	generator.generate();

	auto dom = generator.getDom();

	dom.printString(0);*/

	return 0;
}
