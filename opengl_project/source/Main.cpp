#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/shaderClass.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Cameras/cameraClass.h"
#include "textureLoader/textureClass.h"


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
		// positions	  // normal				// texture
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

	// positions all containers
	glm::vec3 cubePositions[] = {
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
	// positions of the point lights
	glm::vec3 lightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
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
	Shader shaderProgramLighting("shaders/lightingCube.vert", "shaders/multiLight.frag");
	Shader shaderProgramLightbulb("shaders/lightingCube.vert", "shaders/light.frag");

	// stb loads the image from top left instead of top right so needs to be vertically flipped
	stbi_set_flip_vertically_on_load(true);

	// lets make vertex buffer object to send to gpu
	// two seperate vaos and vbos for triangles
	GLuint VAO, VBO, EBO, lightingVAO;
	//generates vertex array before generating buffers
	glGenVertexArrays(1, &lightingVAO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// setup for rectangle
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
	// Attribute pointer for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Attribute pointer for normal of faces
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Attribute pointer for the texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// binds VBO to lighting VAO
	glBindVertexArray(lightingVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Attribute pointer for position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// loads texture using texture class
	shaderProgramLighting.Activate();
	Texture tungtungSahur("resources/tttsahur.jpg");

	Texture tungtungSahurSpecMap("resources/tttsahurSpecMap.jpg");


	// bind texture location
	shaderProgramLighting.setInt("material.diffuse", 0);

	// bind texture location for spec map
	shaderProgramLighting.setInt("material.specular", 1);

	


	
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
		glClearColor(0.0f, 0.f, 0.0f, 1.0f);
		// clears color bit and depth buffer bit each frame (so that the correct depth is shown and changed each frame)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

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


		// uses the shader program and VAO and draws the triangle each frame
		shaderProgramLighting.Activate();
		// sets lighting color and object color to calculate final color of object in shader

		// directional light
		shaderProgramLighting.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shaderProgramLighting.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shaderProgramLighting.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shaderProgramLighting.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		shaderProgramLighting.setVec3("pointLights[0].position", lightPositions[0]);
		shaderProgramLighting.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgramLighting.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgramLighting.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shaderProgramLighting.setFloat("pointLights[0].constant", 1.0f);
		shaderProgramLighting.setFloat("pointLights[0].linear", 0.09f);
		shaderProgramLighting.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		shaderProgramLighting.setVec3("pointLights[1].position", lightPositions[1]);
		shaderProgramLighting.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgramLighting.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgramLighting.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shaderProgramLighting.setFloat("pointLights[1].constant", 1.0f);
		shaderProgramLighting.setFloat("pointLights[1].linear", 0.09f);
		shaderProgramLighting.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		shaderProgramLighting.setVec3("pointLights[2].position", lightPositions[2]);
		shaderProgramLighting.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgramLighting.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgramLighting.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shaderProgramLighting.setFloat("pointLights[2].constant", 1.0f);
		shaderProgramLighting.setFloat("pointLights[2].linear", 0.09f);
		shaderProgramLighting.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		shaderProgramLighting.setVec3("pointLights[3].position", lightPositions[3]);
		shaderProgramLighting.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shaderProgramLighting.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		shaderProgramLighting.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shaderProgramLighting.setFloat("pointLights[3].constant", 1.0f);
		shaderProgramLighting.setFloat("pointLights[3].linear", 0.09f);
		shaderProgramLighting.setFloat("pointLights[3].quadratic", 0.032f);
		// sets the number of lights 
		shaderProgramLighting.setInt("pointLightCount", 4);
		// sets shininess and view direction
		shaderProgramLighting.setVec3("viewPos", camera.Position);
		shaderProgramLighting.setFloat("material.shininess", 32.0f);


		shaderProgramLighting.setMat4("view", view);
		shaderProgramLighting.setMat4("projection", projection);


		glActiveTexture(GL_TEXTURE0);
		tungtungSahur.Bind2D();

		// set active specmap texture
		glActiveTexture(GL_TEXTURE1);
		tungtungSahurSpecMap.Bind2D();

		// binds array we want to edit before for loop 
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]); // translates model matrix to positon of cube
			float angle = 20.0f * i;
			if (i % 3 == 0)
			{// every third cube spins
				angle = currentFrame* 25.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgramLighting.setMat4("model", model);
			// draws cubes
			glDrawArrays(GL_TRIANGLES, 0, 36);

		};
		// shader for light bulb is used
		shaderProgramLightbulb.Activate();

		shaderProgramLightbulb.setMat4("view", view);
		shaderProgramLightbulb.setMat4("projection", projection);

		// binds array we want to edit before for loop 
		glBindVertexArray(lightingVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			// moves the model matrix by vector, scales the lightbulb cube down to 0.2 times size
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			
			shaderProgramLightbulb.setMat4("model", model);

			// draws light
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
	glDeleteVertexArrays(1, &lightingVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shaderProgramLighting.Delete();
	tungtungSahur.Delete();


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
		// moves image up
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(UP, RUNNING, deltaTime);
		}
		// moves image down
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(DOWN, RUNNING, deltaTime);
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
		// moves image up
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(UP, WALKING, deltaTime);
		}
		// moves image down
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			camera.ProcessKeyboaurdInput(DOWN, WALKING, deltaTime);
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



