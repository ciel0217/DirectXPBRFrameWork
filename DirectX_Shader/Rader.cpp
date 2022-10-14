#include "Rader.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "RaderPoint.h"
#include "Player.h"
#include "EnemyManager.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "quaternion.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"

void Rader::Config()
{
	m_TextureName = "asset/texture/radar.png";
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	ManagerTexture::LoadTexture(m_TextureName);
	m_RenderQueue = e2D;

	m_RaderPoint = new RaderPoint();
	m_RaderPoint->Config();

}

void Rader::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

	m_Wh = D3DXVECTOR2(170.0f, 170.0f);

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
	m_EnemyManager = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<EnemyManager>(e3DObject);
}

void Rader::Uninit()
{
	delete m_RaderPoint;
}

void Rader::Update()
{
	std::list<CGameObject3D*> enemy_list = m_EnemyManager->GetEnemyList();
	D3DXVECTOR3 player_pos = m_Player->GetPosition();


	for (auto enemy : enemy_list){
		if (!enemy)continue;

		D3DXVECTOR3 enemy_pos = enemy->GetPosition();
		if (((player_pos.x - SEARCH_RADIUS < enemy_pos.x) && (player_pos.x + SEARCH_RADIUS > enemy_pos.x))
			&& ((player_pos.z - SEARCH_RADIUS < enemy_pos.z) && (player_pos.z + SEARCH_RADIUS > enemy_pos.z)))
		{
			enemy_pos.y = 0.0f;
			player_pos.y = 0.0f;
			D3DXVECTOR3 vec = enemy_pos - player_pos;
			float distance = D3DXVec3Length(&vec);

			D3DXVECTOR3 axisz = Transform(UnitZ, m_Player->GetRotation());
			D3DXVECTOR3 axisx = D3DXVECTOR3(cosf(D3DX_PI / 2.0f)*axisz.x + sinf(D3DX_PI / 2.0f)*axisz.z, 0.0f, -sinf(D3DX_PI / 2.0f)*axisz.x + cosf(D3DX_PI / 2.0f)*axisz.z);

			//yé≤Çñ≥éãÇµÇΩì‡êœäOêœí∑Ç≥
			float inner = vec.x * axisx.x + vec.z * axisx.z;
			float inner1 = axisz.x * axisx.x + axisz.z * axisx.z;
			float closs = axisx.x * vec.z - axisx.z * vec.x;
			float length1 = sqrtf(vec.x * vec.x + vec.z * vec.z);
			float length2 = sqrtf(axisx.x * axisx.x + axisx.z * axisx.z);

			float radian = acosf(inner / (length1 * length2));
			if (closs < 0.0f) {
				radian = D3DX_PI * 2.0f - radian;
			}

			float hiritu = distance / SEARCH_RADIUS;
			//_RPTN(_CRT_WARN," %lf \n", player->rot.y);
			float y = sinf(radian) * (m_Wh.y * 0.5f) * hiritu;
			float x = cosf(radian) * (m_Wh.x * 0.5f)* hiritu;

			m_EnemyPoint.push_back(D3DXVECTOR3(m_Position.x + x, (m_Position.y + -1.0f *  y), 0.0f));
		}
	}

}

void Rader::Draw()
{
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	
	CDxRenderer::GetRenderer()->SetBlendState(BLEND_MODE_ADD);
	for (auto point : m_EnemyPoint) {
		m_RaderPoint->SetPosition(point);
		m_RaderPoint->SetScale(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
		m_RaderPoint->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));

		m_RaderPoint->Draw();
	}

	CDxRenderer::GetRenderer()->SetBlendState(BLEND_MODE_ALPHABLEND);
	m_EnemyPoint.clear();
}
