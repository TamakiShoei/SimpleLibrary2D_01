#ifndef SWAP_CHAIN
#define	SWAP_CHAIN

#include "../../CommonDifinition.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	/**
	* @brief �X���b�v�`�F�C���̏������֐�
	* @param[in] factory �t�@�N�g���[
	* @param[in] command_queue �R�}���h�L���[
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ComPtr<IDXGIFactory4> factory, ComPtr<ID3D12CommandQueue> command_queue);

	/**
	* @brief �`���̃o�b�t�@���o�b�N�o�b�t�@�ɐݒ�
	*/
	void UpdateFrameIndex();

	/**
	* @brief �X���b�v�`�F�C���̉���֐�
	*/
	void Finalize();

	/**
	* @brief �X���b�v�`�F�C���̎擾�֐�
	* @retval �C���X�^���X
	*/
	inline IDXGISwapChain4* Get()
	{
		return this->instance.Get();
	}

	/**
	* @brief ���݂̃t���[���̃C���f�b�N�X���擾����֐�
	*/
	inline UINT GetFrameIndex()
	{
		return this->frameIndex;
	}

private:
	ComPtr<IDXGISwapChain4> instance;
	UINT frameIndex;
};

#endif