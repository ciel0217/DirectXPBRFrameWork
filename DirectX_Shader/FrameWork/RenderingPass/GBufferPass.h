#pragma once
#include <list>

class CommonProcess;

class GBufferPass {

public:
	GBufferPass() {};
	~GBufferPass() {};

	void Draw(std::list<CommonProcess*> gameobject);
};