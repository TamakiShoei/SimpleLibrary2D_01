#ifndef TEX_BUFFER
#define TEX_BUFFER

#include <map>
#include "../CommonDifinition.h"

class TexBuffer
{
public:
	TexBuffer();
	~TexBuffer();

	bool CreateBuffer(int key, ID3D12Device* device);

	ID3D12Resource* GetBuffer(int key);

private:
	static std::map<int, ID3D12Resource*> Buffers;
};

#endif