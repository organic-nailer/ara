#include "ara_window.h"

AraWindow::AraWindow(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	initGlfw();
	initSkia();
}

AraWindow::~AraWindow() {
	delete sSurface;
	delete sContext;
	glfwDestroyWindow(glWindow);
	glfwTerminate();
}

void errorCallback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void AraWindow::initGlfw() {
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);

	glWindow = glfwCreateWindow(
		windowWidth, windowHeight, 
		"Ara", NULL, NULL);

	if (!glWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);
	glfwSetKeyCallback(glWindow, keyCallback);
}

void AraWindow::initSkia() {
	GrContextOptions options;
	sContext = GrContext::MakeGL(nullptr, options).release();

	GrGLFramebufferInfo framebufferInfo;
	framebufferInfo.fFBOID = 0;
	framebufferInfo.fFormat = GL_RGBA8;

	SkColorType colorType;
	colorType = kRGBA_8888_SkColorType;
	GrBackendRenderTarget backendRenderTarget(windowWidth, windowHeight, 0, 0, framebufferInfo);

	sSurface = SkSurface::MakeFromBackendRenderTarget(
		sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr
	).release();
	if (sSurface == nullptr) abort();
}

void AraWindow::draw() {
	SkCanvas* canvas = sSurface->getCanvas();
	//canvas->rotate(SkIntToScalar(++angle));
	SkPaint paint;
	paint.setColor(SK_ColorBLUE);
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
	SkRect bounds{ -4 * scale, -4 * scale, windowWidth, windowHeight };
	canvas->drawRect(bounds, paint);
	sContext->flush();
}

void AraWindow::drawRect(int left, int top, int right, int bottom, int32_t color) {
	SkCanvas* canvas = sSurface->getCanvas();
	SkPaint paint;
	paint.setColor(color);
	canvas->drawRect({ (float)left, (float)top, (float)right, (float)bottom }, paint);
	sContext->flush();
}

void AraWindow::drawText(const char* text, int left, int top) {
	SkCanvas* canvas = sSurface->getCanvas();
	SkPaint paint;
	paint.setColor(SK_ColorBLACK);
	SkFont font;
	font.setSubpixel(true);
	font.setSize(40);
	canvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8, (float)left, (float)top + 40, font, paint);
	sContext->flush();
}

void AraWindow::pollEvents() {
	glfwPollEvents();
}

void AraWindow::swapBuffers() {
	glfwSwapBuffers(glWindow);
}

bool AraWindow::shouldClose() {
	return glfwWindowShouldClose(glWindow);
}

void AraWindow::clearWhite() {
	SkCanvas* canvas = sSurface->getCanvas();
	canvas->clear(SK_ColorWHITE);
}
