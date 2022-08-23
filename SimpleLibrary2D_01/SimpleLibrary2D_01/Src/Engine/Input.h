#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>

#define MAX_KEY 256

struct KeyState
{
	// �L�[��� => �����ꂽ or ������Ă��Ȃ�
	bool IsPrevPushed;		// 1�t���[���O�̃L�[���
	bool IsCurrentPushed;	// ���̃t���[���̃L�[���
};

class Input
{
public:
	Input();
	~Input();

	/**
	* @brief Input�֌W�̍X�V�֐�
	* @details Input���𖈃t���[���󂯎��
	*/
	void Update();

	/**
	* @brief �L�[�{�[�h�����`�F�b�N�֐�
	* @details �L�[�{�[�h�������ꂽ�u�Ԃ����m����
	* @param[in] key ���ׂ����L�[�̒萔
	* @retval true ���m���ꂽ
	* @retval false ���m����Ă��Ȃ�
	*/
	bool IsPushed(int key);

	/**
	* @brief �L�[�{�[�����`�F�b�N�֐�
	* @details �L�[�{�[�h�������ꑱ���Ă��邩�����m����
	* @param[in] key ���ׂ����L�[�̒萔
	* @retval true ���m���ꂽ
	* @retval false ���m����Ă��Ȃ�
	*/
	bool IsHeld(int key);
	
	/**
	* @brief �L�[�{�[�����`�F�b�N�֐�
	* @details �L�[�{�[�h��������Ȃ��Ȃ����u�Ԃ����m����
	* @param[in] key ���ׂ����L�[�̒萔
	* @retval true ���m���ꂽ
	* @retval false ���m����Ă��Ȃ�
	*/
	bool IsReleased(int key);

private:
	KeyState keyStates[MAX_KEY];
};

#endif