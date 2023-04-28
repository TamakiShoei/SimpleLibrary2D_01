#ifndef	FACTORY_H
#define FACTORY_H

#include "../../CommonDifinition.h"

class Factory
{
public:
	Factory();
	~Factory();

	/**
	* @brief ファクトリーの初期化関数
	* @param[in] factory_flag ファクトリーフラグ
	* @retval true 初期化成功
	* @retval false 初期化失敗
	*/
	bool Initialize(UINT factory_flags);

	/**
	* @brief ファクトリーの解放関数
	*/
	void Finalize();

	/**
	* @brief ファクトリーの取得関数
	* @retval インスタンス
	*/
	inline ComPtr<IDXGIFactory4> Get()
	{
		return this->instance;
	}

private:
	ComPtr<IDXGIFactory4> instance;
};

#endif