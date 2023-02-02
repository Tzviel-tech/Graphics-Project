#define _USE_MATH_DEFINES
#define GLM_ENABLE_EXPERIMENTAL
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include <nfd.h>

#include <stdio.h>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "ImguiMenus.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "Utils.h"
float lightx;
float lightz;
float lighty;
bool normals;
bool material = false;
bool drawlight = false;
bool box;
bool wbox;
bool rec;
bool newlight = false;
bool addlight = false;
static float nornalscale;
static float cleft = 1;
static float cright = 1;
static float down = -1;
static float up = 1;
static float cnear = 1;
static float cfar = -1;
static float grey = 1;
bool x = false, y = false, z = false, xW = false, yW = false, zW = false;
bool WORLD_TRANSFOM;
bool orthogonalP;
bool movecamera;
bool cameraworld;
bool cameralocal;
bool Z_buff;
/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "Graphics yeah";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;

ImGuiIO* imgui;
GLFWwindow* window;

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus(Scene & scene);
void HandleImguiInput();

int main(int argc, char **argv)
{

	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();
	Camera camera; 
	scene->AddCamera(camera);
	Utils u;
	scene->AddLight(std::make_shared<PointLight>(glm::vec3(15, 1, 10), glm::vec3(1, 1, 1)));
	scene->AddLight(std::make_shared<PointLight>(glm::vec3( 0, 5, 5),  glm::vec3(0, 0, 0)));
	scene->AddLight(std::make_shared<PointLight>(glm::vec3(-5, 0, 0),  glm::vec3(0, 0, 0)));
	std::shared_ptr<MeshModel>model = u.LoadMeshModel("C:/Users/Tzviel/Desktop/MODELS/homer_simpson.obj");
	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTextures();
	scene->AddModel(model);
	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus(*scene);
		ImGui::Render();
		HandleImguiInput();
		scene->GetCamera(0).SetPTransform(cleft, cright, down, up, cnear, cfar);
		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard)
	{
		if (imgui->KeysDown[49]) // 1
		{
			scene->GetActiveModel()->rotate.x+=(M_PI / 200);
		}

		if (imgui->KeysDown[50]) // 2
		{
			scene->GetActiveModel()->rotate.x += (-M_PI / 200);
		}

		if (imgui->KeysDown[51]) // 3
		{
			scene->GetActiveModel()->rotate.x += (M_PI / 200);
		}

		if (imgui->KeysDown[52]) // 4
		{
			scene->GetActiveModel()->rotate.x += (-M_PI / 200);
		}

		if (imgui->KeysDown[53]) // 5
		{
			scene->GetActiveModel()->rotate.x += (M_PI / 200);
		}

		if (imgui->KeysDown[54]) // 6
		{
			scene->GetActiveModel()->rotate.x += (-M_PI / 200);
		}

		if (imgui->KeysDown[45]) // -
		{
			scene->GetActiveModel()->rotate.x += (1 / 1.01);
		}

		if (imgui->KeysDown[61]) // +
		{
			scene->GetActiveModel()->rotate.x += (1.01);
		}

		if (imgui->KeysDown[65]) // a
		{
			scene->GetActiveModel()->trans += (glm::vec3(-0.02, 0, 0));
		}

		if (imgui->KeysDown[68]) // d
		{
			scene->GetActiveModel()->trans += (glm::vec3(0.02, 0, 0));
		}

		if (imgui->KeysDown[83]) // s
		{
			scene->GetActiveModel()->trans += (glm::vec3(0, 0.02,0));
		}

		if (imgui->KeysDown[87]) // w
		{
			scene->GetActiveModel()->trans += (glm::vec3(0, -0.02, 0));
		}
	}

	
}

void Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);

}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}

float GetAspectRatio()
{
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void DrawImguiMenus(Scene & scene)
{
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}
	Camera& c = scene.GetActiveCamera();
	std::shared_ptr<MeshModel> model =scene.GetModel(0);
	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}




	ImGui::Begin("decide local or world transfom:");
	ImGui::Text("TO MOVE BY KEYS AND MOUSE:\n press 'W' to move up\n press 'S' to move down\n press 'D' to move left\n press 'A' to move right");
	ImGui::SliderFloat("scale", &model->scalex, 1, 50);
	ImGui::SliderFloat("translate x asix", &model->trans.x, -10, 15);
	ImGui::SliderFloat("translate y asix", &model->trans.y, -10, 10);
	ImGui::SliderFloat("translate z asix", &model->trans.z, -10, 10);
	ImGui::SliderFloat("rotate x asix", &model->rotate.x, -360, 360);
	ImGui::SliderFloat("rotate y asix", &model->rotate.y, -360, 360);
	ImGui::SliderFloat("rotate z asix", &model->rotate.z, -360, 360);
	ImGui::SliderFloat("scale normal", &nornalscale, 1, 50);

	ImGui::Text("Press left mouse to rotate and decide rotation:");
	ImGui::Checkbox("Rotate x", &x);
	ImGui::Checkbox("Rotate y", &y);
	ImGui::Checkbox("Rotate z", &z);
	ImGui::Checkbox("World transfom", &WORLD_TRANSFOM);
	ImGui::Checkbox("view volume", &orthogonalP);
	ImGui::Checkbox("move camera", &movecamera);
	ImGui::Checkbox("local cameraT", &cameralocal);
	ImGui::Checkbox("world cameraT", &cameraworld);
	ImGui::Checkbox("normals", &normals);
	ImGui::Checkbox("prespective projection", &c.pres);
	ImGui::Checkbox("bounding_boxlocal", &box);
	ImGui::Checkbox("bounding_boxworld", &wbox);
	ImGui::Checkbox("rectangle", &rec);
	ImGui::Checkbox("change material", &material);
	ImGui::Checkbox("Show grey Z:", &Z_buff);
	ImGui::Checkbox("change light pos:", &addlight);
	ImGui::Checkbox("drawlight rays", &drawlight);
	if (ImGui::Button("Reset all to zero"))
	{
		model->rotate.x = 0;
		model->rotate.y = 0;
		model->rotate.z = 0;
		model->trans.x = 0;
		model->trans.y = 0;
		model->trans.z = 0;
		model->scalex = 1;
	}
	if (WORLD_TRANSFOM)
	{
		ImGui::Begin("decide world transfom:");
		ImGui::Text("TO MOVE BY KEYS AND MOUSE:\n press 'U' to move up\n press 'J' to move down\n press 'H' to move left\n press 'K' to move right");
		ImGui::SliderFloat("scale", &model->scalexW, 0, 2);
		ImGui::SliderFloat("translate x asix", &model->transW.x, -10, 15);
		ImGui::SliderFloat("translate y asix", &model->transW.y, -10, 10);
		ImGui::SliderFloat("translate z asix", &model->transW.z, -10, 10);
		ImGui::SliderFloat("rotate x asix", &model->rotateW.x, -360, 360);
		ImGui::SliderFloat("rotate y asix", &model->rotateW.y, -360, 360);
		ImGui::SliderFloat("rotate z asix", &model->rotateW.z, -360, 360);
		ImGui::Text("Press left mouse to rotate and decide rotation:");
		ImGui::Checkbox("Rotate x", &xW);
		ImGui::Checkbox("Rotate y", &yW);
		ImGui::Checkbox("Rotate z", &zW);
		if (ImGui::Button("Reset all to zero"))
		{
			model->rotateW.x = 0;
			model->rotateW.y = 0;
			model->rotateW.z = 0;
			model->transW.x = 0;
			model->transW.y = 0;
			model->transW.z = 0;
			model->scalexW = 1;
		}
		ImGui::End();
	}
	
	if (orthogonalP)
	{
		ImGui::Begin("decide view volume");
		ImGui::SliderFloat("left", &cleft, -10, 10);
		ImGui::SliderFloat("right", &cright, -10, 10);
		ImGui::SliderFloat("down", &down, -10, 10);
		ImGui::SliderFloat("up", &up, -10, 10);
		ImGui::SliderFloat("near", &cnear, -360, 360);
		ImGui::SliderFloat("far", &cfar, -360, 360);
		ImGui::End();
	}
	if (movecamera)
	{
		ImGui::Begin("move camera");
		ImGui::SliderFloat("a", &c.a, -10, 10);
		ImGui::SliderFloat("b", &c.b, -10, 10);
		ImGui::SliderFloat("c", &c.c, -10, 10);
		ImGui::End();
	}

	//if (ImGui::BeginMenu("camera"))
	//{
	if (cameraworld)
	{

		ImGui::Begin("decide camera world transfom:");
		ImGui::SliderFloat("scale", &c.scalexW, 0, 2);
		ImGui::SliderFloat("translate x asix", &c.transW.x, -10, 15);
		ImGui::SliderFloat("translate y asix", &c.transW.y, -10, 10);
		ImGui::SliderFloat("translate z asix", &c.transW.z, -10, 10);
		ImGui::SliderFloat("rotate x asix", &c.rotateW.x, -360, 360);
		ImGui::SliderFloat("rotate y asix", &c.rotateW.y, -360, 360);
		ImGui::SliderFloat("rotate z asix", &c.rotateW.z, -360, 360);
		ImGui::Text("Press left mouse to rotate and decide rotation:");
		ImGui::Checkbox("Rotate x", &xW);
		ImGui::Checkbox("Rotate y", &yW);
		ImGui::Checkbox("Rotate z", &zW);
		if (ImGui::Button("Reset all to zero"))
		{
			c.rotateW.x = 0;
			c.rotateW.y = 0;
			c.rotateW.z = 0;
			c.transW.x = 0;
			c.transW.y = 0;
			c.transW.z = 0;
			c.scalexW = 1;
		}
		ImGui::End();

	}
	if (cameralocal)
	{

		ImGui::Begin("decide camera local transfom:");
		ImGui::SliderFloat("scale", &c.scalex, 0, 2);
		ImGui::SliderFloat("translate x asix", &c.trans.x, -10, 15);
		ImGui::SliderFloat("translate y asix", &c.trans.y, -10, 10);
		ImGui::SliderFloat("translate z asix", &c.trans.z, -10, 10);
		ImGui::SliderFloat("rotate x asix", &c.rotate.x, -360, 360);
		ImGui::SliderFloat("rotate y asix", &c.rotate.y, -360, 360);
		ImGui::SliderFloat("rotate z asix", &c.rotate.z, -360, 360);
		ImGui::Text("Press left mouse to rotate and decide rotation:");
		ImGui::Checkbox("Rotate x", &x);
		ImGui::Checkbox("Rotate y", &y);
		ImGui::Checkbox("Rotate z", &z);

		if (ImGui::Button("Reset"))
		{
			c.rotate.x = 0;
			c.rotate.y = 0;
			c.rotate.z = 0;
			c.trans.x = 0;
			c.trans.y = 0;
			c.trans.z = 0;
			c.scalex = 1;
		}
		ImGui::End();
	}
	//	}
	

	ImGui::End();
}