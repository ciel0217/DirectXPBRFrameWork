#include "CommonProcess.h"

//�Q�[�����ɍ폜����Ƃ�
bool CommonProcess::Destroy()
{
	if (m_IsDestroy) {
		Uninit();
		//delete this;
		return true;
	}

	return false;
}

//Uninit�̎��ɌĂԁB���true
bool CommonProcess::Release()
{
	delete this;
	return true;
}
