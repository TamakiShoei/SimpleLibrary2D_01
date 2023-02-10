#ifndef	FACTORY_H
#define FACTORY_H

#include "../../CommonDifinition.h"

class Factory
{
public:
	Factory();
	~Factory();

	bool Initialize(UINT factory_flags);
	void Finalize();

	inline ComPtr<IDXGIFactory4> Get()
	{
		return this->factory;
	}

private:
	ComPtr<IDXGIFactory4> factory;
};

#endif