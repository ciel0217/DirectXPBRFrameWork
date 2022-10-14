#pragma once
#include <list>
#include <typeinfo>
#include <type_traits>
#include "Camera.h"
#include "../../enum.h"
#include "../Manager/ManagerCollsionDetection.h"

class CScene
{
private:

	ManagerCollisionDetection* m_ManagerCollisionDetection;

protected:
	std::list<Camera*> m_SceneCameras;
	Camera* m_CurrentMainCamera;
	std::list<CommonProcess *> m_GameObjects[eMaxLayer];				//ゲームオブジェクトのすべて

public:
	
	CScene()
	{ 
		m_ManagerCollisionDetection = ManagerCollisionDetection::GetInstance();
	}
	virtual void SetObject() = 0;
	virtual void Config();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void HitCheck();
	virtual void Draw();

	template <typename T>
	T* AddGameObject(int layer) {
		T* obj = new T();

		if (dynamic_cast<Camera*>(obj)) {
			m_SceneCameras.push_back((Camera*)obj);
			return obj;
		}

		if (typeid(*obj) == typeid(SkyBox)) {
			for (auto camera : m_SceneCameras) {
				camera->SetSkyBox((SkyBox*)obj);
			}
			//return obj;
		}

		m_GameObjects[layer].push_back(obj);

		return obj;
	}

	template <typename T>
	T* AddPostProcess(int index, PostProcessType type) {
		T* obj = new T();

		m_SceneCameras[index]->SetPostProcess(obj, type);

		return obj;
	}

	/*template <typename T>
	T* AddLightObject() {
		T* obj = new T();

		for (auto camera : m_SceneCameras) {

			camera->SetLightObject(obj);

		}

		return obj;
	}*/

	template <typename T>
	T* GetGameObject(int Layer)
	{
		for (auto gameobject : m_GameObjects[Layer]) {
			if (typeid(*gameobject) == typeid(T))//型を調べる
			{
				return (T*)gameobject;
			}
		}

		return NULL;
	}

	std::list<CommonProcess *>* GetGameObjectAll() { return m_GameObjects; }

	Camera* GetCurrentMainCamera() { return m_CurrentMainCamera; }
};

