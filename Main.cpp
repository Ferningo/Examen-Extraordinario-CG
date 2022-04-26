

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


#include "camera.h"

#include <shader_s.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <thread>


using namespace glm;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void animacion_w();
void reset_animation();
void start_animation();
void esperar_un_milisegundo();
float calcular_caida(float milisengundos);
void rebotar();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// camera
Camera camera(glm::vec3(0.0f, 0.0f, 26.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


//animation
float altura = 10.0f;
float total = 0.0f;
float tiempo = 0.0f;
float tiempo_aux = 0.0f;
float diferencia = 0.0f;

int rebotes = 4;



bool fin = false;
bool start = false;
bool rebote = false;




int main()
{








	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("core.vs", "core.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
		// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {

		// positions         // texture coords
		 0.0f, 0.0f, 0.0f,   0.33f, 0.0f,//  A - 0 
		 0.2f, 0.4f, 0.0f,    0.55f, 0.4f,// B - 1 
		 0.3f,  0.4f, 0.0f,    0.66f, 0.4f, // C - 2
		 0.1f,  0.0f, 0.0f,    0.43f, 0.0f,// D - 3  

		 0.25f, 0.25f, 0.0f,   0.91f, 0.2f,//  E - 4
		 0.3f, 0.35f, 0.0f,    0.96f, 0.3f,//  F - 5
		 0.3f, 0.2f, 0.0f,    0.96f, 0.2f,//  G - 6

		 0.25f, 0.08f, 0.0f,   0.91f, 0.08f,//  H - 7
		 0.30f, 0.08f, 0.0f,    0.96f, 0.08f,//  I - 8
		 0.25f, 0.0f, 0.0f,    0.90f, 0.0f,//  J - 9

		 0.5f, 0.4f, 0.0f,    0.95f, 0.4f,//  K - 10
		 0.6f, 0.4f, 0.0f,    1.0f, 0.4f,//  L - 11
		 0.35f, 0.0f, 0.0f,    0.80f, 0.0f,//  M - 12

		 0.0f, 0.05f, 0.0f,    0.0f, 0.05f,//  N - 13
		 0.05f, 0.15f, 0.0f,   0.05f, 0.15f, //  O - 14
		 0.0f, 0.15f, 0.0f,    0.0f, 0.15f,//  P - 15

		 0.0f, 0.4f, 0.0f,    0.0f, 0.4f,//  Q - 16
		 0.05f, 0.4f, 0.0f,    0.05f, 0.4f,//  R - 17

	};




	unsigned int indices[] = {


	0, 1, 3,
	1, 2, 3,

	4,5,6, //HEG
	4,6,7, //EGH
	6,7,8, //GHI
	7,8,9, //HIJ

	9,10,11, //JKL
	9,11,12, //JLM

	13,14,15, //NOP
	15,16,17, //PQR
	14,15,17 //OPR


	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// load and create a texture 
	unsigned int texture1;
	// texture 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("resources/textures/final.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	ourShader.use();
	ourShader.setInt("texture1", 0);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// input
		// -----
		processInput(window);

		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);



		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		ourShader.use();


		//// create transformations
		glm::mat4 model = glm::mat4(1.0f); 
		glm::mat4 projection = glm::mat4(1.0f);

		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


		animacion_w();

		model = translate(model, vec3(0.0f, altura, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);




		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		reset_animation();

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		start_animation();


}



// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void animacion_w() {

	if (start == true) {

		if (fin == false) {
			if (altura <= 0.0f) {
				fin = true;
				start = false;
				rebote = true;
			}

			else {
				esperar_un_milisegundo();
				tiempo += 1;
				diferencia = calcular_caida(tiempo);
				altura -= diferencia;
			
			}
		}
	}

	if (rebote == true) {
		diferencia = 0.0f;
		rebotar();
	}
}

void reset_animation() {
	altura = 10.0f;
	total = 0.0f;
	tiempo = 0.0f;
	diferencia = 0.0f;
	start = false;
	rebotes = 4;
	rebote = false;
	fin = false;

}

void start_animation() {
	start = true;
}

float calcular_caida(float milisegundos) {

	return 0.5 * (0.0098 * milisegundos);

}

void esperar_un_milisegundo() {
	std::chrono::milliseconds dura(1);
	std::this_thread::sleep_for(dura);
}

void rebotar() {
	 

	switch (rebotes) {

	case 4:
		if (altura >= 5.0f) {
			rebotes = 3;
			tiempo_aux = 0;
		}

		else {
			tiempo_aux += 1;
			diferencia += calcular_caida(tiempo_aux);
			altura += diferencia;
		}
		break;

	case 3:
		if (altura <= 0.0f) {
			rebotes = 2;
			tiempo_aux = 0;
		}

		else {
			tiempo_aux += 1;
			diferencia += calcular_caida(tiempo_aux);
			altura -= diferencia;
		}
		break;


	case 2:
		if (altura >= 2.5f) {
			rebotes = 1;
			tiempo_aux = 0;
		}

		else {
			tiempo_aux += 1;
			diferencia += calcular_caida(tiempo_aux);
			altura += diferencia;
		}
		break;

	case 1:
		if (altura <= 0.0f) {
			rebotes = 0;
			tiempo_aux = 0;
		}

		else {
			tiempo_aux += 1;
			diferencia += calcular_caida(tiempo_aux);
			altura -= diferencia;
		}
		break;

	case 0:
		break;
	}

}