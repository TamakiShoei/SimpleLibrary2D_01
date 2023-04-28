#ifndef	DEVICE_H
#define DEVICE_H

#include "../../CommonDifinition.h"

class Device
{
public:
	Device();
	~Device();

	/**
	* @brief �f�o�C�X�̏������֐�
	* @param[in] factory �t�@�N�g���[
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<IDXGIFactory4> factory);

	/**
	* @brief �f�o�C�X�̉���֐�
	*/
	void Finalize();

	/**
	* @brief �f�o�C�X�̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline ID3D12Device* Get()
	{
		return this->instance.Get();
	}

private:
	ComPtr<ID3D12Device> instance;
	std::vector<D3D_FEATURE_LEVEL> levels;
};

#endif	