#pragma once
#include "FrameWork/LowLevel/DirectX.h"

struct OCEAN_CBUFFER
{
	D3DXCOLOR MixColor;
	float Time;
	float WaveSpeed;

	float Dummy[2];
};