#ifndef DESCRIPTOR_HEAP_H
#define	DESCRIPTOR_HEAP_H

#include <map>
#include "../../CommonDifinition.h"

class BasicDescHeap
{
public:
	BasicDescHeap();
	~BasicDescHeap();

	/**
	* @brief �f�B�X�N���v�^�q�[�v�̏������֐�
	* @param[in] device �f�o�C�X
	* @retval true ����������
	* @retval false ���������s
	*/
	bool Initialize(ID3D12Device* device);

	/**
	* @brief �f�B�X�N���v�^�q�[�v��CBV��o�^����֐�
	* @param[in] cbv_desc CBV�̐ݒ�
	* @param[in] key �摜�̃n���h��
	* @param[in] device �f�o�C�X
	*/
	void RegisterCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc, int key, ID3D12Device* device);

	/**
	* @brief �f�B�X�N���v�^�q�[�v��SRV��o�^����֐�
	* @param[in] buff �e�N�X�`���o�b�t�@
	* @param[in] srv_desc SRV�̐ݒ�
	* @param[in] key �摜�̃n���h��
	* @param[in] device �f�o�C�X
	*/
	void RegisterSRV(ID3D12Resource* buff, D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc, int key, ID3D12Device* device);
	
	/**
	* @brief CBV�̃n���h�����擾����֐�
	* @param[in] key �摜�̃n���h��
	* @param[in] device �f�o�C�X
	* @retval CBV�̃n���h��
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandle(int key, ID3D12Device* device);

	/**
	* @brief SRV�̃n���h�����擾����֐�
	* @param[in] key �摜�̃n���h��
	* @param[in] device �f�o�C�X
	* @retval SRV�̃n���h��
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(int key, ID3D12Device* device);

	/**
	* @brief �f�B�X�N���v�^�q�[�v�̐擪�A�h���X���擾����֐�
	* @retval �f�B�X�N���v�^�q�[�v�̐擪�A�h���X
	*/
	ID3D12DescriptorHeap* Get();

	/**
	* @brief �J�E���^�[�̃��Z�b�g�֐�
	* @details <pre>
	* SRV��CBV�̓o�^�̍ۂɃJ�E���^�[���g���ăA�h���X�̈ʒu�����߂Ă��邽��
	* ���t���[�����Z�b�g������K�v������
	* </pre>
	*/
	void ResetCounter();

	/**
	* @brief �f�B�X�N���v�^�q�[�v�̉���֐�
	*/
	void Finalize();

private:
	ComPtr<ID3D12DescriptorHeap> instance;
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	std::map<int, int> cbvDictionaly;
	std::map<int, int> srvDictionaly;
	int registerCounter;
};

#endif