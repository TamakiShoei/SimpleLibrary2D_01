/**
* @file Window.h
* @brief <pre>
* ウィンドウクラスの宣言
* Engineクラスでインスタンスを作成するため、使用者が宣言する必要はない
* </pre>
*/

#ifndef WINDOW_H
#define WINDOW_H

#include "CommonDifinition.h"

class Window
{
public:
	Window() : isClosed(false), isRecievedMessage(false)
	{
	}

	/**
	* @brief ウィンドウプロシージャ関数
	* @details Windowsのメッセージ処理を行う
	* @param[in] hwnd 対象のウィンドウハンドル
	* @param[in] msg メッセージ
	* @param[in] w_param メッセージデータ
	* @param[in] l_param メッセージデータ
	*/
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT n_msg, WPARAM w_param, LPARAM l_param);

	/**
	* @brief ウィンドウ初期化関数
	* @details ウィンドウ情報を設定し、ウィンドウを作成する
	* @retval true 生成成功
	* @retval false 生成失敗
	*/
	bool Initialize();

	/**
	* @brief 更新関数
	* @details Windowsのメッセージ処理の更新を行う
	*/
	void Update();

	/**
	* @brief ウィンドウの閉鎖チェック関数
	* @details ウィンドウが閉じられているかどうかを返す
	* @retval true 閉じている
	* @retval false 閉じていない
	*/
	bool IsClosed()
	{
		return isClosed;
	}

private:
	bool isClosed;
	bool isRecievedMessage;
};

#endif