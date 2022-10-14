#include "GBufferPass.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CommonProcess.h"

void GBufferPass::Draw(std::list<CommonProcess*> gameobject)
{

	for (auto obj : gameobject) {
		obj->Draw();
	}
}
