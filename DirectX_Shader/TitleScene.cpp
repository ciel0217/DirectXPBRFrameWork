#include "TitleScene.h"
#include "Ocean.h"
#include "TitlePlayer.h"
#include "FrameWork/Resources/SkyBox.h"
#include "FrameWork/Resources/DirectionalLight.h"
#include "TitleSceneCamera.h"
#include "TitleLogo.h"
#include "OperationPic.h"
#include "OpetationExplain.h"
#include "CloseBlackBG.h"
#include "OperationWeapon.h"
#include "Operation.h"
#include "Sousasetumei.h"
#include "PressEnterKey.h"
#include "Test3D.h"
#include  "Enemy.h"

void TitleScene::SetObject()
{
	m_CurrentMainCamera = AddGameObject<TitleSceneCamera>(eBackMost);
	m_CurrentMainCamera->SetInitPosition(D3DXVECTOR3(0.0f, -0.5f, 0.0f));

	/*Enemy* enemy = AddGameObject<Enemy>(e3DObject);
	enemy->SetInitPosition(D3DXVECTOR3(0.0f, 95.0f, 0.0f));*/
	//for (int i = 0; i < 7; i++) {
	//	for (int j = 0; j < 7; j++) {
	//		Test3D* test = AddGameObject<Test3D>(e3DObject);
	//		test->Load();
	//		test->SetInitPosition(D3DXVECTOR3(-50.0f + 110.0f * ((float)j/7.0f), 50.0f + 110.0f * ((float)i /7.0f), 0.0f));
	//		test->SetInitScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));
	//		test->Settt((float)j/7.0f, (float)i/7.0f);
	//		//test->Settt(.1f,.1f);
	//	}
	//}

	Ocean* ocean = AddGameObject<Ocean>(e3DObject);
	ocean->SetInitPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	ocean->SetInitScale(D3DXVECTOR3(100000.0f, 1.0f, 100000.0f));
	
	TitlePlayer* test = AddGameObject<TitlePlayer>(e3DObject);
	test->SetInitPosition(D3DXVECTOR3(-100.0f, 0.0f, -100.0f));
	test->SetInitScale(D3DXVECTOR3(5.0f, 5.0f, 5.0f));

	TitleLogo* logo = AddGameObject<TitleLogo>(e2DObject);
	logo->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/4.0f,0.0f));
	logo->SetInitScale(D3DXVECTOR3(600.0f, 300.0f,0.0f));

	Sousasetumei* so = AddGameObject<Sousasetumei>(e2DObject);
	so->SetInitPosition(D3DXVECTOR3(150.0f, 50.0f, 0.0f));
	so->SetInitScale(D3DXVECTOR3(270.0f, 100.0f, 0.0f));

	PressLKey* press = AddGameObject<PressLKey>(e2DObject);
	press->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f + 100.0f, 0.0f));
	press->SetInitScale(D3DXVECTOR3(300.0f, 100.0f, 0.0f));
	
	Operation* ope = AddGameObject<Operation>(e2DObject);
	ope->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	ope->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	ope->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
	
	OperationExplain* exp = AddGameObject<OperationExplain>(e2DObject);
	exp->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	exp->SetInitScale(D3DXVECTOR3(1100.0f, 800.0f, 0.0f));

	OperationPic* pic = AddGameObject<OperationPic>(e2DObject);
	pic->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.7f, 0.0f));
	pic->SetInitScale(D3DXVECTOR3(450.0f, 350.0f, 0.0f));

	

	OperationWeapon* wepon = AddGameObject<OperationWeapon>(e2DObject);
	wepon->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
	wepon->SetInitScale(D3DXVECTOR3(1100.0f, 800.0f, 0.0f));

	CloseBlackBG* black = AddGameObject<CloseBlackBG>(e2DObject);
	black->SetInitPosition(D3DXVECTOR3(SCREEN_WIDTH/2.0f + SCREEN_WIDTH, SCREEN_HEIGHT/2.0f, 0.0f));
	black->SetInitScale(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	AddGameObject<SkyBox>(e3DObject)->SetInitScale(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f));
	AddGameObject<DirectionalLight>(e3DObject);


}
