#pragma once
#include "../FrameWork/LowLevel/DirectX.h"

//“n‚³‚ê‚½Qua‚ÌVec•ûŒü‚ðŽæ“¾
D3DXVECTOR3 Transform(const D3DXVECTOR3 vec, const D3DXQUATERNION qua);

//“n‚³‚ê‚½Vec‚ðŽ²‚ÉRot‰ñ“]
D3DXQUATERNION RotateAxis(const D3DXVECTOR3 vec, float rot, const D3DXQUATERNION qua1);

//from‚©‚çto‚ÌŒü‚«‚Ö‚ÌQua‚ðŽæ“¾
D3DXQUATERNION FromToRotation(D3DXVECTOR3 from, D3DXVECTOR3 to);