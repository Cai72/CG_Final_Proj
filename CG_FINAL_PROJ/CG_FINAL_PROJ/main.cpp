#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void loadOurModel(Shader ModelShader);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// model

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FinalProject", NULL, NULL);
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

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader HouseShader("../shaders/6.1.skybox.vs", "../shaders/6.1.skybox.fs");
	Shader ModelShader("../shaders/model_loading.vs", "../shaders/model_loading.fs");

	Model bedModel("res/bed/Bed.obj");
	Model chairModel("res/chair/HSM0002.obj");
	Model girlModel("res/girl/Girl_in_sleepwear.obj");
	Model bedsideModel("res/bedside/Bedside Table D.obj");
	Model clockModel("res/clock/Clock_obj.obj");
	Model tableModel("res/table/ModernDeskOBJ.obj");
	Model doorModel("res/door/Door.obj");
	Model imacModel("res/mac/imac.obj");
	Model keyboardModel("res/keyboard/clavier_imac.obj");
	Model mouseModel("res/mouse/mouse_imac.obj");
	Model lightModel("res/light/lightbulbfinal.obj");
	Model lampModel("res/lamp/Lamp.obj");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	float HouseVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int HouseVAO, HouseVBO;
	glGenVertexArrays(1, &HouseVAO);
	glGenBuffers(1, &HouseVBO);
	glBindVertexArray(HouseVAO);
	glBindBuffer(GL_ARRAY_BUFFER, HouseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(HouseVertices), &HouseVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------

	vector<std::string> faces
	{
		("../resources/textures/flower.jpg"),//right
		("../resources/textures/flower.jpg"),//left
		("../resources/textures/wood.png"),//top
		("../resources/textures/wood.png"),//down
		("../resources/textures/flower.jpg"),//front
		("../resources/textures/flower.jpg")//back
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------

	HouseShader.use();
	HouseShader.setInt("skybox", 0);

	// render loop
	// -----------
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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// render normal objects
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		// render house
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		HouseShader.use();
		view = glm::mat4(camera.GetViewMatrix()); // remove translation from the view matrix
		HouseShader.setMat4("view", view);
		HouseShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(HouseVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// draw all models

		// draw bed model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.56f, -0.73f, -0.29f));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
			ModelShader.setMat4("model", model);
			bedModel.Draw(ModelShader);
		}

		// draw chair model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.4f, -1.0f, 0.4f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
			model = glm::scale(model, glm::vec3(0.0055f, 0.0055f, 0.0055f));
			ModelShader.setMat4("model", model);
			chairModel.Draw(ModelShader);
		}

		// draw girl model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
			ModelShader.setMat4("model", model);
			girlModel.Draw(ModelShader);
		}

		// draw bedside model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.15f, -1.0f, -1.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
			ModelShader.setMat4("model", model);
			bedsideModel.Draw(ModelShader);
		}

		// draw clock model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, -1.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			ModelShader.setMat4("model", model);
			clockModel.Draw(ModelShader);
		}

		
		// draw table model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.85f, -1.0f, 0.55f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(0.014f, 0.014f, 0.014f));
			ModelShader.setMat4("model", model);
			tableModel.Draw(ModelShader);
		}
		
		// draw door model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.7f, -1.05f, -1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			ModelShader.setMat4("model", model);
			doorModel.Draw(ModelShader);
		}

		// draw imac model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.9f, -0.485f, 0.4f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			ModelShader.setMat4("model", model);
			imacModel.Draw(ModelShader);
		}

		// draw keyboard model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.66f, -0.515f, 0.22f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			ModelShader.setMat4("model", model);
			keyboardModel.Draw(ModelShader);
		}

		// draw mouse model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, -0.555f, 0.55f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			ModelShader.setMat4("model", model);
			mouseModel.Draw(ModelShader);
		}

		// draw light model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.68f, 0.0f));
			model = glm::scale(model, glm::vec3(0.002f, 0.002f, 0.002f));
			ModelShader.setMat4("model", model);
			lightModel.Draw(ModelShader);
		}

		// draw lamp model
		{
			ModelShader.use();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			ModelShader.setMat4("projection", projection);
			ModelShader.setMat4("view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.08f, -0.628f, -0.8f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			ModelShader.setMat4("model", model);
			lampModel.Draw(ModelShader);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &HouseVAO);
	glDeleteBuffers(1, &HouseVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow * window)
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
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
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	return textureID;
}



void loadOurModel(Shader ModelShader) {

	
}