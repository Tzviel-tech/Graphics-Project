#pragma once

#include <vector>
#include <memory>
#include "Light.h"
#include "Camera.h"
#include "MeshModel.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	void AddModelAtindex0(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index)const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;
	Light& GetLight(int index) const;
	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	void AddLight(const std::shared_ptr<Light>& l);
private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Light>> lights;

	int active_camera_index;
	int active_model_index;
};