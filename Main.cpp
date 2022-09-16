#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Asset.h"
#include "Texture.h"

int main() {
	setlocale(LC_ALL, "ru");

	Application& app = Application::get();

	app.init();
	app.start();

	system("pause");

	return 0;
}