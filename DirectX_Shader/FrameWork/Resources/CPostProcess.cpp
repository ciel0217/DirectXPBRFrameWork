#include "CPostProcess.h"

bool CPostProcess::Release()
{
	delete this;
	return true;
}
