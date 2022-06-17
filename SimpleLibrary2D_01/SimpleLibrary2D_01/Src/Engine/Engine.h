/**
* @file Engine.h
* @brief エンジンクラスの宣言
*/
#ifndef ENGINE_H
#define	ENGINE_H

#include "Graphics.h"
#include "Window.h"

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

	/**
	* @brief ウィンドウサイズ設定関数
	* @引数をもとにウィンドウのサイズを設定する
	* @param[in] width_size ウィンドウの横幅
	* @param[in] height_size ウィンドウの縦幅
	*/
	static void SetWindowSize(int width_size, int height_size);

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

	static void DrawTriangle(VECTOR lower_left, VECTOR upper_left, VECTOR lower_right);

private:
	static Engine* instance;

	Window window;
	Graphics graphics;
};


#endif