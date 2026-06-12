#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "cameraClass.h"


void processInput(GLFWwindow* window);
// mouse callback function and scroll callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


const auto SCR_HIGHT = 600u;
const auto SCR_WIDTH = 800u;


// oppacity of images put together 
auto oppacity = 0.2f;

// variables to track time between frames
// makes sure movement is not frame rate dependent 
auto deltaTime = 0.0f;
auto lastFrame = 0.0f;

// for first mouse detection event first mouse = true (used in mouse callback function)
auto firstMouse = true;
auto lastX = SCR_WIDTH / 2.0f, lastY = SCR_HIGHT / 2.0f;


// globals for camera controls

glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);

Camera camera(cameraPosition, cameraUp);


int main() {
	// initialises GLFW
	glfwInit();


	//  GLFW what major and minor version we are using so that it knows
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// tells GLFW which profile (set of functions) we are using
	// we use CORE_PROFILE which contains all the modern functions
	// there is also a COMPATABILITY_PROFILE which contains old functions aswell
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	float cube[] = {
		// positions	  // texture
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	glm::vec3 cubePositions[]
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// creates a pointer to a GLFWwindow object which contains information about the window
	// contains the resolution of the window and the window name
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HIGHT, "OpenGL window", NULL, NULL);
	
	//error check if the window fails to create
	if (window == NULL) {
		std::cout << "Failed to create window " << std::endl;
		glfwTerminate();
		return -1;
	}

	// adds the window into the current context
	glfwMakeContextCurrent(window);

	// sets static fps cursor when on application
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// sets mouse callback function, this function gets called everytime mouse input is detected
	glfwSetCursorPosCallback(window, mouse_callback);
	// sets mouse scroll callback function that activates everytime you scroll
	glfwSetScrollCallback(window, scroll_callback);
	
	// loads glad so it configures OpenGL
	gladLoadGL();

	// bottom left to bottom right of window
	glViewport(0, 0, SCR_WIDTH, SCR_HIGHT);

	
	// uses shaderClass to generate shader program
	Shader shaderProgramTexture("texture.vert", "texture.frag");

	// stb loads the image from top left instead of top right so needs to be vertically flipped
	stbi_set_flip_vertically_on_load(true);

	// generates and binds new texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// sets settings for currently bound texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// creates a pointer to the data of a image with height width and color channels
	int width, height, nrChannels;
	unsigned char *data = stbi_load("xin_pao.jpg", &width, &height, &nrChannels, 0);
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << width << "x" << height
			<< " channels: " << nrChannels << '\n';

	}
	else 
	{
		throw std::runtime_error("IMAGE::FAILED::TO::LOAD\n");

	}
	// deletes data after it has been processed into a texture
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// sets settings for currently bound texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// creates a pointer to the data of a image with height width and color channels

	data = stbi_load("tttsahur.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{ // remember to check how many color channels --> 4 channels means use GL_RGBA instead of GL_RGB
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // Mipmap used to downscale images and conserve memory for smaller use cases

		std::cout << width << "x" << height
			<< " channels: " << nrChannels << '\n';
	}
	else
	{
		throw std::runtime_error("IMAGE::FAILED::TO::LOAD\n");

	}
	// deletes data after it has been processed into a texture
	stbi_image_free(data);


	// to tell openGL which texture unit each shader sampler belongs
	// activate shaderProgram before setting uniforms 
	shaderProgramTexture.Activate();
	shaderProgramTexture.setInt("texture1", 0);
	shaderProgramTexture.setInt("texture2", 1);	

	// lets make vertex buffer object to send to gpu
	// two seperate vaos and vbos for triangles
	GLuint VAO, VBO, EBO;
	//generates vertex array before generating buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup for rectangle
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
	// Attribute pointer for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Attribute pointer for texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// Enables openGL to check if something is in front of something else using depth testing before rendering
	// enables z buffer 
	glEnable(GL_DEPTH_TEST); 



	// while the window is not being closed events are polled to ensure the window doesent close instantly
	while (!glfwWindowShouldClose(window))
	{

		// gets time between last and current frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// checks if any buttons have been pressed that do something
		processInput(window);
	
		// sets the background color each frame
		glClearColor(1.0f, 0.8f, 0.34f, 1.0f);
		// clears color bit and depth buffer bit each frame (so that the correct depth is shown and changed each frame)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// uses the shader program and VAO and draws the triangle each frame
		shaderProgramTexture.Activate();

		// MAKING EVERYTHING 3D
		// to move from world space to view space (USING VIEW MATRIX)
		// move everything back to move the object in view
		glm::mat4 view = glm::mat4(1.0f);

		// look at matrix --> view matrix that looks at some target based on camera positon
		view = camera.GetViewMatrix();

		// to move from view to projection space (PROJECTION MATRIX)
		// adds perspective by calculating some bullshit

		glm::mat4 projection;
		// degrees -> FOV,  second is width/height of screen,  then its close and far plane of rendered objects
		projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		// must initialise the matrix inside the while loop so the matrix is continuously changed instead
		// of re-using the identity matrix

		// in the shader the matricies are multiplyed in order to get the new display on the screen
		// in the order that  V_clip(clipped screen) =  projection * view * model * local 
		// done in order right to left 

		// now send all the matricies to create 3d display
		//shaderProgramTexture.setMat4("model", model);
		shaderProgramTexture.setMat4("view", view);
		shaderProgramTexture.setMat4("projection", projection);

		// changes oppacity of images 
		shaderProgramTexture.setFloat("oppacity", oppacity);
		
	
		// binds texture so that the texture is displayed
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// binds another texture to the second active texure slot so both can be displayed
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		// loop over positions array to draw several cubes

		for (unsigned int i = 0; i < 10; i++)
		{	
			// first we have object in local space --> world space (USING MODEL MATRIX)
			glm::mat4 model = glm::mat4(1.0f); // identity matrix which is changed
			// sets cube positon from poisitons list
			model = glm::translate(model, cubePositions[i]);
			// makes cube spin at some angle depending on position in list
			float angel = 20.0f * i;
			if (i%3 == 0)
			{// makes every third cube rotate with time 
				angel = glfwGetTime() * 25.0f;
			}
			model = glm::rotate(model, glm::radians(angel), glm::vec3(1.0f, 0.3f, 0.5f));
			// sends model matrix to shader
			shaderProgramTexture.setMat4("model", model);
			// draws cube
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		// binds vertex array to 0 to make sure nothing is edited after this point 
		glBindVertexArray(0);
		
		// swaps buffers so you can see triangles
		glfwSwapBuffers(window);

		// take care of all GLFW events
		glfwPollEvents();

	}


	// deletes the vertex array, buffer and shader program after it is no longer being used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shaderProgramTexture.Delete();


	// destroys the window and terminates GLFW before ending the program 
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}


void processInput(GLFWwindow* window)

{
	// camera speed limited by frame rate
	float cameraSpeed = 2.0f * deltaTime;

	// if escape pressed then exits program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	// if 0 pressed turns on wirefram mode
	else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// if 1 pressed turns on fill mode
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// changes how big gradient is
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		// oppacity does not go above 1.0 
		oppacity += 0.001f;
		if (oppacity >= 1.0f) 
		{
			oppacity = 1.0f;
		}

	}

	// changes how big gradient is
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{	//makes sure oppacity does not go below 0.0
		oppacity -= 0.001f;
		if (oppacity <= 0.0f)
		{
			oppacity = 0.0f;
		}
		
			
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))  
		// if else for cheking directional movment with sprinting or walking
	{
		// moves image right at running speed
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(RIGHT, RUNNING, deltaTime);
		}
		// moves image left at running speed
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(LEFT, RUNNING,deltaTime);
		}
		// moves image up at running speed
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(FORWARD, RUNNING, deltaTime);
		}
		// moves image down at running speed
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(BACKWARD, RUNNING, deltaTime);
		}
	}

	else
	{
		// moves image right at walking speed
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(RIGHT, WALKING, deltaTime);
		}
		// moves image left at walking speed
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(LEFT, WALKING, deltaTime);
		}
		// moves image up at walking speed
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(FORWARD, WALKING, deltaTime);
		}
		// moves image down at walking speed
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(BACKWARD, WALKING, deltaTime);
		}

	}

};

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
	// explicit type conversion from double to float (mouse callback takes double for mouse position)
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		// center is set as starter mouse position when program is opend so there is no jump on startup
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed because y goes from bottom to top
	
	lastX = xpos;
	lastY = ypos; // sets last position as the current position for next frame

	camera.ProcessMouseMovement(xoffset, yoffset); // sends mouse offset data to be processed in the camera class

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



