#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>

#define MAX_KEY 256

struct KeyState
{
	// キー情報 => 押された or 押されていない
	bool IsPrevPushed;		// 1フレーム前のキー情報
	bool IsCurrentPushed;	// 今のフレームのキー情報
};

class Input
{
public:
	Input();
	~Input();

	/**
	* @brief Input関係の更新関数
	* @details Input情報を毎フレーム受け取る
	*/
	void Update();

	/**
	* @brief キーボード押下チェック関数
	* @details キーボードが押された瞬間を検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	bool IsPushed(int key);

	/**
	* @brief キーボー押下チェック関数
	* @details キーボードが押され続けているかを検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	bool IsHeld(int key);
	
	/**
	* @brief キーボー押下チェック関数
	* @details キーボードが押されなくなった瞬間を検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	bool IsReleased(int key);

private:
	KeyState keyStates[MAX_KEY];
};

#endif