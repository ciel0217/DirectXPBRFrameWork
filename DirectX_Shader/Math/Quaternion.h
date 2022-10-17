#pragma once
#include "../FrameWork/LowLevel/DirectX.h"

//�n���ꂽQua��Vec�������擾
D3DXVECTOR3 Transform(const D3DXVECTOR3 vec, const D3DXQUATERNION qua);

//�n���ꂽVec������Rot��]
D3DXQUATERNION RotateAxis(const D3DXVECTOR3 vec, float rot, const D3DXQUATERNION qua1);

//from����to�̌����ւ�Qua���擾
D3DXQUATERNION FromToRotation(D3DXVECTOR3 from, D3DXVECTOR3 to);