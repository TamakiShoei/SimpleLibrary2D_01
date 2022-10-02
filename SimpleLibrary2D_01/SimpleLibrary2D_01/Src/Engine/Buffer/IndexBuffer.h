#ifndef INDEX_BUFFER
#define INDEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool CreateBuffer(int key, ID3D12Device* device);

	ID3D12Resource* GetBuffer(int key);

private:
	static std::map<int, ID3D12Resource*> Buffers;
};

#endif