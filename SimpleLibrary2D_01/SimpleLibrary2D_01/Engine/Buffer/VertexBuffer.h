#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	/**
	* @brief ���_�o�b�t�@�̍쐬
	* @param[in] �f�o�C�X
	* @retval true ��������
	* @retval false �������s
	*/
	void Create(ID3D12Device* device);

	/**
	* @brief ���_�o�b�t�@�̎擾�֐�
	* @retval ���_�o�b�t�@�f�[�^
	*/
	ID3D12Resource* Get(ID3D12Device* device);

	/**
	* @brief ���_�o�b�t�@����֐�
	*/
	void Finalize();

	/**
	* @brief UseCounter�̃��Z�b�g�֐�
	* @details ClearScreen�ŕK�����t���[���Ă�
	*/
	void ResetUseCounter();

private:
	std::vector<ID3D12Resource*> buffers;
	int useCounter;
};

#endif