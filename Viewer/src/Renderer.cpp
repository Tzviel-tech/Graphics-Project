#include "Renderer.h"
#include "MeshModel.h"
#include "ShaderProgram.h"
#include <imgui/imgui.h>
#include <vector>
#include <memory>
#include <algorithm>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::Render(const std::shared_ptr<Scene>& scene)
{
	int cameraCount = scene->GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene->GetModelCount();
		const Camera& camera = scene->GetActiveCamera();
	
		for(int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene->GetModel(currentModelIndex);
			currentModel->PlanarTexture();
			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetWorldTransformation() * currentModel->GetModelTransformation());
			colorShader.setUniform("view", camera.GetViewTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("camerapos", camera.eye);
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("lightposition",scene->GetLight(0)->GetPosition( ));

			// Set 'texture1' as the active texture at slot #0
			texture1.bind(0);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);

			// Unset 'texture1' as the active texture at slot #0
			texture1.unbind(0);

			colorShader.setUniform("color", glm::vec3(1,1,1));
			colorShader.setUniform("lightcolor", glm::vec3(1, 1, 1));
			colorShader.setUniform("ambientcolor", glm::vec3(1, 0, 0));

			//// Drag our model's faces (triangles) in line mode (wireframe)
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glBindVertexArray(currentModel->GetVAO());
			//glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			//glBindVertexArray(0);
		}
	}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
}

void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("bin\\Debug\\fb.jpg", true))
	{
		texture1.loadTexture("bin\\Release\\fb.jpg", true);
	}
}