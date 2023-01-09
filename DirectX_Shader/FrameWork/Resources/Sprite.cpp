#include "Sprite.h"
#include "../LowLevel/CDxRenderer.h"
#include "CBuffer.h"
#include "../Manager/ManagerScene.h"


Microsoft::WRL::ComPtr<ID3D11Buffer> Sprite2D::m_VertexBuffer;
CBuffer* Sprite2D::m_MatrixCBuffer;
CBuffer* Sprite2D::m_MaterialCBuffer;

void Sprite2D::InitSprite()
{
	m_MatrixCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	m_MaterialCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(MATERIAL_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	VERTEX_3D *vertex = new VERTEX_3D;
	D3D11_BUFFER_DESC dsc{};
	dsc.Usage = D3D11_USAGE_DEFAULT;
	dsc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dsc.ByteWidth = sizeof(VERTEX_3D) * 4;
	dsc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	m_VertexBuffer.Attach(CDxRenderer::GetRenderer()->CreateVertexBuffer(4, nullptr));
}

void Sprite2D::Release()
{
	delete m_MaterialCBuffer;
	delete m_MatrixCBuffer;
}

void Sprite2D::Draw2D(ID3D11ShaderResourceView * texture, D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color, D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	
	HRESULT ht = CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = wh.x * 0.5f;
	hh = wh.y * 0.5f;

	// ���_�O�ԁi����̒��_�j
	vertex[0].Position = D3DXVECTOR3(pos.x - hw, pos.y - hh, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);

	// ���_�P�ԁi�E��̒��_�j
	vertex[1].Position = D3DXVECTOR3(pos.x + hw, pos.y - hh, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y);

	// ���_�Q�ԁi�����̒��_�j
	vertex[2].Position = D3DXVECTOR3(pos.x - hw, pos.y + hh, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + uv_wh.y);

	// ���_�R�ԁi�E���̒��_�j
	vertex[3].Position = D3DXVECTOR3(pos.x + hw, pos.y + hh, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y + uv_wh.y);

	CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_VertexBuffer.Get(), 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (texture)
	{
		// �e�N�X�`���ݒ�
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);

	D3DXMATRIX worldViewProjection;
	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);
	m_MatrixCBuffer->UpdateBuffer(&worldViewProjection);
	m_MatrixCBuffer->VSSetCBuffer(2);

	// 2D�`��Ȃ̂Ő[�x����
	CDxRenderer::GetRenderer()->SetDepthEnable(false);


	// �|���S���`��
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);
}

void Sprite2D::Draw2D(D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color, D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh)
{
	D3D11_MAPPED_SUBRESOURCE msr;

	HRESULT ht = CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D *vertex = (VERTEX_3D*)msr.pData;

	float hw, hh;
	hw = wh.x * 0.5f;
	hh = wh.y * 0.5f;

	// ���_�O�ԁi����̒��_�j
	vertex[0].Position = D3DXVECTOR3(pos.x - hw, pos.y - hh, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);

	// ���_�P�ԁi�E��̒��_�j
	vertex[1].Position = D3DXVECTOR3(pos.x + hw, pos.y - hh, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y);

	// ���_�Q�ԁi�����̒��_�j
	vertex[2].Position = D3DXVECTOR3(pos.x - hw, pos.y + hh, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + uv_wh.y);

	// ���_�R�ԁi�E���̒��_�j
	vertex[3].Position = D3DXVECTOR3(pos.x + hw, pos.y + hh, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y + uv_wh.y);

	CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_VertexBuffer.Get(), 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);

	D3DXMATRIX worldViewProjection;
	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);
	m_MatrixCBuffer->UpdateBuffer(&worldViewProjection);
	m_MatrixCBuffer->VSSetCBuffer(2);

	//// 2D�`��Ȃ̂Ő[�x����
	CDxRenderer::GetRenderer()->SetDepthEnable(false);


	// �|���S���`��
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);
	

}

void Sprite2D::DrawBillboard(ID3D11ShaderResourceView * texture, D3DXVECTOR3 pos, D3DXVECTOR3 wh, D3DXCOLOR color,
	D3DXVECTOR2 uv, D3DXVECTOR2 uv_wh, MATERIAL_CBUFFER MATERIAL_CBUFFER)
{
	//CDxRenderer::GetRenderer()->SetAlphaTestEnable(true);
	Camera* camera = ManagerScene::GetInstance()->GetCurrentSceneCamera();
	
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CDxRenderer::GetRenderer()->GetDeviceContext()->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		float width = wh.x;
		float height = wh.y;

		D3DXVECTOR3 dir = camera->GetPosition() - pos;
		D3DXVec3Normalize(&dir, &dir);

		// ���_���W�̐ݒ�
		vertex[0].Position = D3DXVECTOR3(-width / 2.0f, height / 2.0f, 0.0f);
		vertex[1].Position = D3DXVECTOR3(width / 2.0f, height / 2.0f, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-width / 2.0f, -height / 2.0f, 0.0f);
		vertex[3].Position = D3DXVECTOR3(width / 2.0f, -height / 2.0f, 0.0f);

		//�@���̐ݒ�
		vertex[0].Normal = dir;
		vertex[1].Normal = dir;
		vertex[2].Normal = dir;
		vertex[3].Normal = dir;

		// ���_�J���[�̐ݒ�
		vertex[0].Diffuse = color;
		vertex[1].Diffuse = color;
		vertex[2].Diffuse = color;
		vertex[3].Diffuse = color;

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y + uv_wh.y);
		vertex[1].TexCoord = D3DXVECTOR2(uv.x, uv.y);
		vertex[2].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y + uv_wh.y);
		vertex[3].TexCoord = D3DXVECTOR2(uv.x + uv_wh.x, uv.y);

		CDxRenderer::GetRenderer()->GetDeviceContext()->Unmap(m_VertexBuffer.Get(), 0);
	}

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3DXMATRIX  mtxScale, mtxTranslate, mtxWorld;
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	D3DXMATRIX mtxView, mtxInverseView;
	D3DXMatrixLookAtLH(&mtxView, &camera->GetPosition(), &camera->GetCameraLookAtPoint(), &camera->GetCameraUp());
	//D3DXMatrixTranspose(&mtxView, &mtxView);
	//D3DXMatrixInverse(&mtxInverseView, NULL, &mtxView);
	
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	mtxWorld._21 = mtxView._12;
	mtxWorld._22 = mtxView._22;
	mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
	D3DXMatrixTranspose(&mtxWorld, &mtxWorld);

	m_MatrixCBuffer->UpdateBuffer(&mtxWorld);
	m_MatrixCBuffer->VSSetCBuffer(0);

	m_MaterialCBuffer->UpdateBuffer(&MATERIAL_CBUFFER);
	m_MaterialCBuffer->PSSetCBuffer(3);

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);

	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &texture);

	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);

	//�J�����O�����ɖ߂�
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);

	//CDxRenderer::GetRenderer()->SetAlphaTestEnable(false);
}
