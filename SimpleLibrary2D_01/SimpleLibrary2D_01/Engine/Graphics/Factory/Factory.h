#ifndef	FACTORY_H
#define FACTORY_H

#include "../../CommonDifinition.h"

class Factory
{
public:
	Factory();
	~Factory();

	/**
	* @brief �t�@�N�g���[�̏������֐�
	* @param[in] factory_flag �t�@�N�g���[�t���O
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(UINT factory_flags);

	/**
	* @brief �t�@�N�g���[�̉���֐�
	*/
	void Finalize();

	/**
	* @brief �t�@�N�g���[�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ComPtr<IDXGIFactory4> Get()
	{
		return this->instance;
	}

private:
	ComPtr<IDXGIFactory4> instance;
};

#endif