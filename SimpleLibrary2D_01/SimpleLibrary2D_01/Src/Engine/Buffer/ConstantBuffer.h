#ifndef CONSTANT_BUFFER
#define CONSTANT_BUFFER

#include <map>
#include "../CommonDifinition.h"

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool CreateBuffer(int key, ID3D12Device* device);

	ID3D12Resource* GetBuffer(int key);

private:
	static std::map<int, ID3D12Resource*> Buffers;
};

#endif