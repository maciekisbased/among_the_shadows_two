
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "mesh/meshClass.h"

void processInput(GLFWwindow* window);
// mouse callback function, keyboaurd callback and scroll callback
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


const unsigned int SCR_HIGHT = 600u;
const unsigned int SCR_WIDTH = 800u;

// flashlight starts as off
bool flashlightOn = false;

// buffer for if f key is held down
bool heldDown = false;


// oppacity of images put together 
float oppacity = 0.2f;

// variables to track time between frames
// makes sure movement is not frame rate dependent 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// for first mouse detection event first mouse = true (used in mouse callback function)
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HIGHT / 2.0f;


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


	Vertex cube[] = {
		// positions                                // normal                   // texture
		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  0.0f) },

		Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f) },

		Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f) },

		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f) },

		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f) },

		Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  1.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  0.0f) },
		Vertex{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  1.0f) },
	};

	GLuint indices[] = {

		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
		24,25,26,27,28,29,30,31,32,33,34,35
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
	// sets keybourd callback function that activates everytime you press a keybourd button
	glfwSetKeyCallback(window, key_callback);

	// loads glad so it configures OpenGL
	gladLoadGL();

	// bottom left to bottom right of window
	glViewport(0, 0, SCR_WIDTH, SCR_HIGHT);


	// stb loads the image from top left instead of top right so needs to be vertically flipped
	stbi_set_flip_vertically_on_load(true);


	Texture textures[] =
	{ // creates texture array needed for mesh 
		Texture("resources/tttsahur.jpg", "diffuse", 0),
		Texture("resources/tttsahurSpecMap.jpg", "specular", 1)
	};


	// uses shaderClass to generate shader program
	Shader shaderProgramLighting("shaders/lightingCube.vert", "shaders/multiLight.frag");
	std::vector <Vertex> verts(cube, cube + sizeof(cube) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh box(verts, ind, tex);

	Shader shaderProgramLightbulb("shaders/lightingCube.vert", "shaders/light.frag");
	std::vector <Texture> lTex;
	Mesh lightBox(verts, ind, lTex);

	
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
		// spotlight
		shaderProgramLighting.setVec3("spotLight.position", camera.Position);
		shaderProgramLighting.setVec3("spotLight.direction", camera.Front);

		switch (flashlightOn)
		{
		case true:
			shaderProgramLighting.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
			shaderProgramLighting.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
			shaderProgramLighting.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		
			break;

		case false:
			// if false then spotlight values are 0
			shaderProgramLighting.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			shaderProgramLighting.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
			shaderProgramLighting.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
			break;
				
		}
	
		shaderProgramLighting.setFloat("spotLight.constant", 1.0f);
		shaderProgramLighting.setFloat("spotLight.linear", 0.09f);
		shaderProgramLighting.setFloat("spotLight.quadratic", 0.032f);
		shaderProgramLighting.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(10.5f)));
		shaderProgramLighting.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(16.5f)));
		// sets the number of  point lights 
		shaderProgramLighting.setInt("pointLightCount", 4);
		// sets shininess and view direction
		shaderProgramLighting.setVec3("viewPos", camera.Position);
		shaderProgramLighting.setFloat("material.shininess", 32.0f);


		shaderProgramLighting.setMat4("view", view);
		shaderProgramLighting.setMat4("projection", projection);

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
			
			box.Draw(shaderProgramLighting);
		};
		// shader for light bulb is used
		shaderProgramLightbulb.Activate();

		shaderProgramLightbulb.setMat4("view", view);
		shaderProgramLightbulb.setMat4("projection", projection);

		for (unsigned int i = 0; i < 4; i++)
		{
			// moves the model matrix by vector, scales the lightbulb cube down to 0.2 times size
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			
			shaderProgramLightbulb.setMat4("model", model);

			lightBox.Draw(shaderProgramLightbulb);
		}

		// swaps buffers so you can see triangles
		glfwSwapBuffers(window);

		// take care of all GLFW events
		glfwPollEvents();

	}


	// deletes the vertex array, buffer and shader program after it is no longer being used
	
	shaderProgramLighting.Delete();
	shaderProgramLightbulb.Delete();


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


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{ // stops the flashlight from continually turning on and off when key is held down with the
		// heldDown buffer bool variable

		switch (heldDown)
		{
		case false:

			flashlightOn = !flashlightOn;
		
			heldDown = true;
			
			break;

		case true:

			break;

		}
	}

	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		heldDown = false;
	}

}


