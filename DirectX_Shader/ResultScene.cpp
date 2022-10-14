#include "ResultScene.h"
#include "ResultBG.h"
#include "ResultSceneCamera.h"
#include "ResultScoreFrame.h"
#include "ResultScore.h"
#include "OpenBlackBG.h"
#include "CloseBlackBG.h"
#include "FrameWork/Resources/DirectionalLight.h"
#include "PressEnterKey.h"

void ResultScene::SetObject()
{
	m_CurrentMainCamera = AddGameObject<ResultSceneCamera>(eBackMost);

	ResultBG* bg = AddGameObject<ResultBG>(e2DObject);
	bg->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 0.0f));
	bg->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	ResultScoreFrame* frame = AddGameObject<ResultScoreFrame>(e2DObject);
	frame->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	frame->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	ResultScore* score = AddGameObject<ResultScore>(e2DObject);
	score->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 30.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	score->SetInitScale(D3DXVECTOR3(90.0f, 120.0f, 0.0f));

	PressLKey* enter = AddGameObject<PressLKey>(e2DObject);
	enter->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, SCREEN_HEIGHT - 150.0f, 0.0f));
	enter->SetInitScale(D3DXVECTOR3(300.0f, 100.0f, 0.0f));

	OpenBlackBG* open = AddGameObject<OpenBlackBG>(e2DObject);
	open->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	open->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	CloseBlackBG* close = AddGameObject<CloseBlackBG>(e2DObject);
	close->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f, 0.0f));
	close->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));


	//AddGameObject<SkyBox>(e3DObject)->SetInitScale(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f));
	AddGameObject<DirectionalLight>(e3DObject);
}
