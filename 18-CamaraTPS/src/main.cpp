//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"

//Texture include
#include "Headers/Texture.h"

GLuint VBO, VAO, EBO;
Shader shader;

Texture * texture1;
Texture * texture2;

struct VertexTexture {
	glm::vec3 m_Pos;
	glm::vec2 m_TexCoord;
};

int screenWidth;
int screenHeight;

GLFWwindow * window;
InputManager inputManager;
double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable test depth, must be clear depth buffer bit
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/mvpTexture.vs", "../Shaders/mixTexture.fs");

	// Set up vertex data (and buffer(s)) and attribute pointers
	// This is the correct vertex order according to the rule right hand
	VertexTexture vertices[36] = {
		// Back face
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
	// Front face
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	// Left face
	{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	// Right face
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	// Bottom face
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	// Top face
	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
	{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f) }
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Texture Mapping attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]),
		(GLvoid*) sizeof(vertices[0].m_Pos));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	texture1 = new Texture(GL_TEXTURE_2D, "../Textures/texturaLadrillos.jpg");
	texture1->load();

	texture2 = new Texture(GL_TEXTURE_2D, "../Textures/goku.png");
	texture2->load();
}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	shader.destroy();

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	delete (texture1);
	delete (texture2);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	inputManager.keyPressed(inputManager.toApplicationKey(key), deltaTime,
		inputManager.toApplicationState(action));
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager.mouseMoved(xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	inputManager.mouseClicked(inputManager.toMouseButtonIndex(button), xpos,
		ypos, inputManager.toApplicationState(state));
}

void mouseScrollCallback(GLFWwindow* window, double xoffset,
	double yoffset) {
	inputManager.mouseScroll(yoffset);
}


bool processInput(bool continueApplication) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	glfwPollEvents();
	inputManager.do_movement(deltaTime);
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();
	glm::vec3 cubePositions[] =
	{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-2.0f, -5.0f, -15.0f),
		glm::vec3(-1.5f, 2.2f, -2.5f), glm::vec3(1.8f, 1.0f, -12.3f),
		glm::vec3(4.4f, -0.4f, -3.5f), glm::vec3(-6.7f, 3.0f, -7.5f),
		glm::vec3(-4.3f, -3.0f, -2.5f), glm::vec3(3.5f, 8.0f, -2.5f),
		glm::vec3(-1.7f, -0.7f, -1.5f), glm::vec3(3.3f, 3.0f, -1.5f) };

	float offsetz = 0.0;

	while (psi) {
		psi = processInput(true);
		// This is new, need clear depth buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		shader.turnOn();

		// Bind Texture
		texture1->bind(GL_TEXTURE0);
		glUniform1i(shader.getUniformLocation("textura1"), 0);
		texture2->bind(GL_TEXTURE1);
		glUniform1i(shader.getUniformLocation("textura2"), 1);

		// Create transformations
		// Create transformations
		glm::mat4 view;
		view = glm::rotate(view,
			glm::radians(inputManager.getPitch()),
			glm::vec3(1, 0, 0));
		view = glm::rotate(view,
			glm::radians(inputManager.getYaw()),
			glm::vec3(0, 1, 0));
		glm::vec3 cameraPos = inputManager.getCameraPos();
		view = glm::translate(view,
			glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z));

		glm::mat4 projection;
		projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		// Get their uniform location
		shader.getUniformLocation("model");
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 model;
		model = glm::rotate(glm::radians(inputManager.roty), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model ,glm::vec3(0.0, 0.0, offsetz));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		inputManager.camera_look_at = glm::vec4(glm::rotate(glm::radians(inputManager.roty), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0, 0.0, offsetz, 1.0f));

		offsetz += 0.001;
		
		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

