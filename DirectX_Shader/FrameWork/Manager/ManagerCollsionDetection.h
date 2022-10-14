#pragma once
#include "../../enum.h"
#include "../Resources/ccollisiondetection.h"
#include <list>


class CommonProcess;

class ManagerCollisionDetection
{
private:
	static ManagerCollisionDetection* _instance;
	CCollisionDetection* m_CollisionDetection[eCollisionTypeMax][eCollisionTypeMax];

public:
	ManagerCollisionDetection();
	~ManagerCollisionDetection();

	static void Create() 
	{
		if (!_instance) {
			_instance = new ManagerCollisionDetection();
		}
	}
	
	static void Release()
	{
		delete _instance;
	}

	static ManagerCollisionDetection* GetInstance() { return _instance; }

	void CollisionCheck(std::list<CommonProcess*> gameobject);
};