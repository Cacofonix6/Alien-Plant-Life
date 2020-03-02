//GLEW
//#include <GL/glew.h>  
#include "Plant.h"
#include "TextureLoader.h"
#include <GLFW/glfw3.h> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
Mesh* createQuad(float scale);

// camera
Camera camera(glm::vec3(0.0f, 8.0f, 20.0f));
float lastX = Window::WIDTH / 2.0f;
float lastY = Window::HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

int main(void)
{
	///////////////////////// Window and glad init /////////////////////////
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(Window::WIDTH, Window::HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	TextureLoader texLoad;

	GLuint bark = texLoad.loadTexture("resources/Textures/Bark2.png", "bark");
	GLuint leaf = texLoad.loadTexture("resources/Textures/Leaf2.png", "leaf"); 
	GLuint alienBark = texLoad.loadTexture("resources/Textures/AlienBark.png", "AlienBark");
	GLuint alienLeaf = texLoad.loadTexture("resources/Textures/AlienLeaf.png", "AlienLeaf");

	// load models
	// -----------

	Plant *plant1 = new Plant(bark, leaf, glm::vec3(-5, 0, 0), glm::vec3(0, 12, 0));
	Plant *plant2 = new Plant(alienBark, alienLeaf, glm::vec3( 5, 0, 0), glm::vec3(10, 12, 0));

	
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		

		// render
		// ------
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				
		plant1->update(camera, deltaTime);
		plant2->update(camera, deltaTime);
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate(); 
	
	delete plant1;
	delete plant2;
	//delete mesh;
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

Mesh* createQuad(float scale)
{
	Mesh* mesh = new Mesh();

	glm::vec3 points1[4] = {
		glm::vec3(-scale, -scale,  scale),
		glm::vec3( scale, -scale,  scale),
		glm::vec3( scale,  scale,  scale),
		glm::vec3(-scale,  scale,  scale)
	};
	mesh->addQuad(points1);

	glm::vec3 points2[4] = {
		glm::vec3( scale, -scale,   scale),
		glm::vec3( scale, -scale,  -scale),
		glm::vec3( scale,  scale,  -scale),
		glm::vec3( scale,  scale,   scale)
	};
	mesh->addQuad(points2);

	glm::vec3 points3[4] = {
		glm::vec3( scale, -scale,  -scale),
		glm::vec3(-scale, -scale,  -scale),
		glm::vec3(-scale,  scale,  -scale),
		glm::vec3( scale,  scale,  -scale)
	};
	mesh->addQuad(points3);

	glm::vec3 points4[4] = {
		glm::vec3(-scale, -scale, -scale),
		glm::vec3(-scale, -scale,  scale),
		glm::vec3(-scale,  scale,  scale),
		glm::vec3(-scale,  scale, -scale)
	};
	mesh->addQuad(points4);

	glm::vec3 points5[4] = {
		glm::vec3(-scale, -scale,  scale),
		glm::vec3( scale, -scale,  scale),
		glm::vec3( scale, -scale, -scale),
		glm::vec3(-scale, -scale, -scale)
	};
	mesh->addQuad(points5);

	glm::vec3 points6[4] = {
		glm::vec3(-scale,  scale,  scale),
		glm::vec3( scale,  scale,  scale),
		glm::vec3( scale,  scale, -scale),
		glm::vec3(-scale,  scale, -scale)
	};
	mesh->addQuad(points6);
	mesh->initMesh();

	return mesh;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}