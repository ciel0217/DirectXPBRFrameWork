#include "ManagerInput.h"
#include "../LowLevel/DirectX.h"

ManagerInput* ManagerInput::m_Instance;

void ManagerInput::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	if (!m_DInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_DInput, NULL);
	}
	InitKeyboard(hInstance, hWnd);
	InitPad(hInstance, hWnd);
	InitMouse(hInstance, hWnd);

}

void ManagerInput::UpdateInput()
{
	UpdateKeyboard();
	UpdatePad();
	UpdateMouse();
}

void ManagerInput::UninitInput()
{
	UninitKeyboard();
	UninitPad();
	UninitMouse();
}


void ManagerInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = m_DInput->CreateDevice(GUID_SysKeyboard, &m_DIDevKeyboard, NULL);
	if (FAILED(hr) || m_DIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_DIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_DIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return ;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_DIDevKeyboard->Acquire();
}


BOOL CALLBACK ManagerInput::SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	int* count = ManagerInput::GetInstance()->GetPadCount();
	result = ManagerInput::GetInstance()->GetDevice()->CreateDevice(lpddi->guidInstance, &ManagerInput::GetInstance()->GetGamePad()[*count++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���
}

void ManagerInput::InitPad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;

	m_PadCount = 0;

	m_DInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);

	for (int i = 0; i < m_PadCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = m_GamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
//		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//		if ( FAILED(result) )
//			return false; // ���[�h�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		m_GamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		m_GamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		m_GamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		m_GamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		m_GamePad[i]->Acquire();
	}
	return;
}

void ManagerInput::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = m_DInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	if (FAILED(result) || m_Mouse == NULL)
	{
		MessageBox(hWnd, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = m_Mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = m_Mouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = m_Mouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWnd, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return;
	}

	// �A�N�Z�X���𓾂�
	m_Mouse->Acquire();
	return;
}



void ManagerInput::UpdateKeyboard()
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, m_KeyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = m_DIDevKeyboard->GetDeviceState(sizeof(m_KeyState), m_KeyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			m_KeyStateTrigger[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & m_KeyState[cnt];
			m_KeyStateRelease[cnt] = (keyStateOld[cnt] ^ m_KeyState[cnt]) & ~m_KeyState[cnt];
			m_KeyStateRepeat[cnt] = m_KeyStateTrigger[cnt];

			if (m_KeyState[cnt])
			{
				m_KeyStateRepeatCnt[cnt]++;
				if (m_KeyStateRepeatCnt[cnt] >= 20)
				{
					m_KeyStateRepeat[cnt] = m_KeyState[cnt];
				}
			}
			else
			{
				m_KeyStateRepeatCnt[cnt] = 0;
				m_KeyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		m_DIDevKeyboard->Acquire();
	}

}

void ManagerInput::UpdatePad()
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < m_PadCount; i++)
	{
		DWORD lastPadState;
		lastPadState = m_PadState[i];
		m_PadState[i] = 0x00000000l;	// ������

		result = m_GamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = m_GamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_GamePad[i]->Acquire();
		}

		result = m_GamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = m_GamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = m_GamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (dijs.lY < 0)					m_PadState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					m_PadState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					m_PadState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					m_PadState[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	m_PadState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	m_PadState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	m_PadState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	m_PadState[i] |= BUTTON_X;
		//* �x�{�^��
		if (dijs.rgbButtons[4] & 0x80)	m_PadState[i] |= BUTTON_Y;
		//* �y�{�^��
		if (dijs.rgbButtons[5] & 0x80)	m_PadState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	m_PadState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	m_PadState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[8] & 0x80)	m_PadState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[9] & 0x80)	m_PadState[i] |= BUTTON_M;

		// Trigger�ݒ�
		m_PadTrigger[i] = ((lastPadState ^ m_PadState[i])	// �O��ƈ���Ă���
			& m_PadState[i]);					// ��������ON�̂��

	}
}

void ManagerInput::UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = m_MouseState;
	// �f�[�^�擾
	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
	if (SUCCEEDED(result))
	{
		m_MouseTrigger.lX = m_MouseState.lX;
		m_MouseTrigger.lY = m_MouseState.lY;
		m_MouseTrigger.lZ = m_MouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i < 8; i++)
		{
			m_MouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				m_MouseState.rgbButtons[i]) & m_MouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = m_Mouse->Acquire();
	}
	return;
}

void ManagerInput::UninitKeyboard()
{
	if (m_DIDevKeyboard)
	{
		m_DIDevKeyboard->Release();
		m_DIDevKeyboard = NULL;
	}
}

void ManagerInput::UninitPad()
{
	for (int i = 0; i < GAME_PAD_MAX; i++) {
		if (m_GamePad[i])
		{
			m_GamePad[i]->Unacquire();
			m_GamePad[i]->Release();
		}
	}
}

void ManagerInput::UninitMouse()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = NULL;
	}
}


