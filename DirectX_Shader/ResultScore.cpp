#include "ResultScore.h"
#include "Score.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include <random>
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Manager/audio.h"

void ResultScore::Config()
{
	m_TextureName = "asset/texture/number.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;

	m_DramRoll = new Audio();
	m_Don = new Audio();
}

void ResultScore::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_Score = Score::GetScore();
	m_FrameCount = 0.0f;
	m_DramRoll->Load("asset/music/dram.wav");
	m_Don->Load("asset/music/don.wav");

	m_DramRoll->Play();

	m_IsDon = false;

}

void ResultScore::Uninit()
{
	m_DramRoll->StopAll();
	m_Don->StopAll();

	delete m_Don;
	delete m_DramRoll;
}

void ResultScore::Update()
{
	if (PRINT_SCORE_MAX_FRAME > m_FrameCount)m_FrameCount++;
}

void ResultScore::Draw()
{
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	
	int number;
	if (m_FrameCount < PRINT_SCORE_MAX_FRAME) {
		
		number = rand();
	}
	else {
		if (!m_IsDon) {
			m_DramRoll->StopAll();
			m_Don->Play();
			m_IsDon = true;
		}
		number = m_Score;
	}

	for (int i = 0; i < SCORE_DIGIT; i++) {

		// ����\�����錅�̐���
		float x = (float)(number % 10);

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = m_Position.x - m_Scale.x * i;	// �v���C���[�̕\���ʒuX
		float py = m_Position.y;			// �v���C���[�̕\���ʒuY
		float pw = m_Scale.x;				// �v���C���[�̕\����
		float ph = m_Scale.y;				// �v���C���[�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = x * tw;			// �e�N�X�`���̍���X���W
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W

		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), D3DXVECTOR3(px, py, 0.0f), D3DXVECTOR3(pw, ph, 0.0f), m_Color, D3DXVECTOR2(tx, ty), D3DXVECTOR2(tw, th));

		number /= 10;
	}
}
