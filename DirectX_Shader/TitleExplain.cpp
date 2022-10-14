#include "TitleExplain.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "OpetationExplain.h"

void TitleExplain::Config()
{
	m_TextureName = "asset/texture/tab.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_RenderQueue = e2D;
}

void TitleExplain::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void TitleExplain::Uninit()
{
}


