/**
* @file Engine.h
* @brief エンジンクラスの宣言
*/
#ifndef ENGINE_H
#define	ENGINE_H

#include "Graphics.h"
#include "Window.h"

/** @brief エンジンクラス */
class Engine
{
public:
	//エンジン関係の関数
	/**
	* @brief 初期化関数
	* @details <pre>
	* エンジン全体(Windowクラス、Graphicsクラスの初期化)
	* この関数はゲームループ開始前に一度だけ実行する
	* </pre>
	* @retval true 初期化成功
	* @retval false 初期化成功
	*/
	static bool Initialize();

	/**
	* @brief 解放関数
	* @details <pre>
	* エンジンの解放を行う
	* この関数はゲームループ終了時に一度だけ実行する
	* </pre>
	*/
	static void Finalize();

	/**
	* @brief 更新関数
	* @details <pre>
	* エンジンの更新処理を行う
	* この関数はゲームループ内で毎フレーム実行する
	* </pre>
	*/
	static void Update();

	//ウィンドウ関係の関数
	/**
	* @brief ウィンドウ終了チェック関数
	* @details ウィンドウが終了しているかどうかを返す
	* @retval true 閉じている
	* @retval false 閉じていない
	*/
	static bool IsClosedWindow();

	//描画関係の関数
	/**
	* @brief 描画開始関数
	* @details <pre>
	* バックバッファのクリアを行う
	* この関数は必ず描画処理前に実行する必要がある
	* また、実行後にClearScreen関数を実行する必要がある
	* </pre>
	*/
	static void ClearScreen();

	/**
	* @brief 描画終了関数
	* @details <pre>
	* バックバッファとフロントバッファを入れ替える
	* この関数は必ずClearScreenの後に実行する必要がある
	* </pre>
	*/
	static void ScreenFlip();

private:
	
	/**
	* @brief GPU待ち関数
	* @details CommandListの完了を検知する
	*/
	static void WaitForPreviousFrame();

	/**
	* @brief WindowインスタンスのGetter関数
	* @retval windowインスタンス
	*/
	inline Window* GetWindows()
	{
		return &window;
	}

	/**
	* @brief WindowインスタンスのGetter関数
	* @retval windowインスタンス
	*/
	inline Graphics* GetGraphics()
	{
		return &graphics;
	}

private:
	static Engine* instance;

	Window window;
	Graphics graphics;
};


#endif