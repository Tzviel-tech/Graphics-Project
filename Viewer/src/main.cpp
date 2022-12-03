#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <fstream>
#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <sstream>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


static float cleft = -1;
static float cright = 1;
static float down = -1;
static float up = 1;
static float cnear = 1;
static float cfar = 1;
bool x = false, y = false, z = false,xW=false,yW=false,zW=false;
bool WORLD_TRANSFOM;
bool orthogonalP;
bool movecamera;
bool cameraworld;
bool cameralocal;
/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}
//MeshModel& load(const char* filename)
//{
//	    std::vector<glm::vec3> vertices;
//		std::vector<Face> faces;
//		std::string line;
//		std::vector<std::string> lines;
//		std::ifstream in(filename);
//		if (!in.is_open()) {
//			printf("Cannot load model %s\n", filename);
//			exit;
//		}
//		while (!in.eof()) {
//			std::getline(in, line);
//			lines.push_back(line);
//		}
//		in.close();
//		float a, b, c;
//		for (std::string& line : lines) 
//		{
//			if (line[0] == 'v') {
//				sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
//				vertices.push_back(glm::vec3( a, b, c));
//			}
//			else if (line[0] == 'f') {
//			
//				faces.push_back(Face(std::istringstream(line)));
//			}
//		}
//		string name=lines.at(0);
//		MeshModel * model = new MeshModel(faces, vertices,vertices,name);
//		return *model;
//}
int main(int argc, char** argv)
{
	int windowWidth = 1920, windowHeight = 1080;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;
	Utils u;
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	shared_ptr<MeshModel>model = u.LoadMeshModel("C:/Users/Tzviel/Desktop/MODELS/teapot.obj");
	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	shared_ptr<Camera>c(new Camera());
	Scene scene = Scene();
	scene.AddModel(model);
	scene.AddCamera(c);
	scene.SetActiveCameraIndex(0);
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	//std::cout << *model;
	//std::cout << *model;
	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		c->SetPTransform(cleft, cright, down, up, cnear, cfar);
		RenderFrame(window, scene, renderer, io);
		

	}
	
	
	Cleanup(window);
	
	return 0;
}








static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	

	MeshModel& model = scene.GetModel(0);
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// Controll translation
		if (io.KeysDown['A'])
		{
			
			model.trans.x -= 5;
		}
		if (io.KeysDown['D'])
		{

			model.trans.x += 5;
		}
		if (io.KeysDown['W'])
		{

			model.trans.y += 5;
		}
		if (io.KeysDown['S'])
		{

			model.trans.y -= 5;
		}
		if (io.KeysDown['H'])
		{

			model.transW.x -= 5;
		}
		if (io.KeysDown['K'])
		{

			model.transW.x += 5;
		}
		if (io.KeysDown['U'])
		{

			model.transW.y += 5;
		}
		if (io.KeysDown['J'])
		{

			model.transW.y -= 5;
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0] && x)
		{

			model.rotate.x++;

		}
		if (io.MouseDown[0] && y)
		{

			model.rotate.y++;

		}
		if (io.MouseDown[0] && z)
		{

			model.rotate.z++;

		}
		if (io.MouseDown[0] && xW)
		{

			model.rotateW.x++;

		}
		if (io.MouseDown[0] && yW)
		{

			model.rotateW.y++;

		}
		if (io.MouseDown[0] && zW)
		{

			model.rotateW.z++;

		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModelAtindex0(Utils::LoadMeshModel(outPath));
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
	MeshModel& model = scene.GetModel(0);
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
	ImGui::SliderFloat("scale", &model.scalex, 0, 500);
	ImGui::SliderFloat("translate x asix", &model.trans.x, -1000, 1500);
	ImGui::SliderFloat("translate y asix", &model.trans.y, -1000, 1000);
	ImGui::SliderFloat("translate z asix", &model.trans.z, -1000, 1000);
	ImGui::SliderFloat("rotate x asix", &model.rotate.x, -360, 360);
	ImGui::SliderFloat("rotate y asix", &model.rotate.y, -360, 360);
	ImGui::SliderFloat("rotate z asix", &model.rotate.z, -360, 360);
	ImGui::Text("Press left mouse to rotate and decide rotation:");
	ImGui::Checkbox("Rotate x", &x);
	ImGui::Checkbox("Rotate y", &y);
	ImGui::Checkbox("Rotate z", &z);
	ImGui::Checkbox("World transfom", &WORLD_TRANSFOM);
	ImGui::Checkbox("view volume", &orthogonalP);
	ImGui::Checkbox("move camera", &movecamera);
	ImGui::Checkbox("local cameraT", &cameralocal);
	ImGui::Checkbox("world cameraT", &cameraworld);
	if (ImGui::Button("Reset all to zero"))  
	{
		model.rotate.x = 0;
		model.rotate.y = 0;
	    model.rotate.z = 0;
		model.trans.x = 0;
		model.trans.y = 0;
		model.trans.z = 0;
		model.scalex = 1;
	}
	if(WORLD_TRANSFOM)
	{
		ImGui::Begin("decide world transfom:");
		ImGui::Text("TO MOVE BY KEYS AND MOUSE:\n press 'U' to move up\n press 'J' to move down\n press 'H' to move left\n press 'K' to move right");
		ImGui::SliderFloat("scale", &model.scalexW, 0, 2);
		ImGui::SliderFloat("translate x asix", &model.transW.x, -1000, 1500);
		ImGui::SliderFloat("translate y asix", &model.transW.y, -1000, 1000);
		ImGui::SliderFloat("translate z asix", &model.transW.z, -1000, 1000);
		ImGui::SliderFloat("rotate x asix", &model.rotateW.x, -360, 360);
		ImGui::SliderFloat("rotate y asix", &model.rotateW.y, -360, 360);
		ImGui::SliderFloat("rotate z asix", &model.rotateW.z, -360, 360);
		ImGui::Text("Press left mouse to rotate and decide rotation:");
		ImGui::Checkbox("Rotate x", &xW);
		ImGui::Checkbox("Rotate y", &yW);
		ImGui::Checkbox("Rotate z", &zW);
		if (ImGui::Button("Reset all to zero"))
		{
			model.rotateW.x = 0;
			model.rotateW.y = 0;
			model.rotateW.z = 0;
			model.transW.x = 0;
			model.transW.y = 0;
			model.transW.z = 0;
			model.scalexW = 1;
		}
		ImGui::End();
	}
	if (orthogonalP)
{
	ImGui::Begin("decide view volume");
	ImGui::SliderFloat("left", &cleft, -10, 10);
	ImGui::SliderFloat("right", &cright, -10, 10);
	ImGui::SliderFloat("down", &down, -10, 10);
	ImGui::SliderFloat("up", &up, -10,10);
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
				ImGui::SliderFloat("translate x asix", &c.transW.x, -1000, 1500);
				ImGui::SliderFloat("translate y asix", &c.transW.y, -1000, 1000);
				ImGui::SliderFloat("translate z asix", &c.transW.z, -1000, 1000);
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
				ImGui::SliderFloat("translate x asix", &c.trans.x, -1000, 1500);
				ImGui::SliderFloat("translate y asix", &c.trans.y, -1000, 1000);
				ImGui::SliderFloat("translate z asix", &c.trans.z, -1000, 1000);
				ImGui::SliderFloat("rotate x asix", &c.rotate.x, -360, 360);
				ImGui::SliderFloat("rotate y asix", &c.rotate.y, -360, 360);
				ImGui::SliderFloat("rotate z asix", &c.rotate.z, -360, 360);
				ImGui::Text("Press left mouse to rotate and decide rotation:");
				ImGui::Checkbox("Rotate x", &x);
				ImGui::Checkbox("Rotate y", &y);
				ImGui::Checkbox("Rotate z", &z);
				ImGui::End();
				if (ImGui::Button("Reset all to zero"))
				{
					c.rotate.x = 0;
					c.rotate.y = 0;
					c.rotate.z = 0;
					c.trans.x = 0;
					c.trans.y = 0;
					c.trans.z = 0;
					c.scalex = 1;
				}
			}
	//	}
	
	
	
	ImGui::End();
}