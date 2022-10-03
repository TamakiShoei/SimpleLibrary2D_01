﻿/**
* @file Engine.h
* @brief エンジンクラスの宣言
*/
#ifndef ENGINE_H
#define	ENGINE_H

#include "CommonDifinition.h"
#include "InputDataDefinition.h"
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

	/**
	* @brief キーボード押下チェック関数
	* @details キーボードが押された瞬間を検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	static bool IsPushedKey(int key);
	
	/**
	* @brief キーボー押下チェック関数
	* @details キーボードが押され続けているかを検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	static bool IsHeldKey(int key);

	/**
	* @brief キーボー押下チェック関数
	* @details キーボードが押されなくなった瞬間を検知する
	* @param[in] key 調べたいキーの定数
	* @retval true 検知された
	* @retval false 検知されていない
	*/
	static bool IsReleasedKey(int key);

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

	/**
	* @brief 三角形描画関数
	* @details 引数で受け取った頂点をもとに三角形を描画する
	* @param[in] lower_left 左下頂点座標
	* @param[in] upper_left 左上頂点座標
	* @param[in] lower_right 右下頂点座標
	*/
	static void DrawTriangle(VECTOR lower_left, VECTOR upper_left, VECTOR lower_right);

	/**
	* @brief 矩形描画関数
	* @details 引数で受け取った頂点をもとに三角形を描画する
	* @param[in] lower_left 左下頂点座標
	* @param[in] upper_left 左上頂点座標
	* @param[in] upper_right 右上頂点座標
	* @param[in] lower_right 右下頂点座標
	*/
	static void DrawRect(VECTOR lower_left, VECTOR upper_left, VECTOR upper_right, VECTOR lower_right);

	/**
	* @brief 画像読み込み関数
	* @details 引数で受け取ったファイルパスをもとに画像を描画する際に必要なキーを返す
	* @param[in] file_path 読み込みたい画像のファイルパス
	* @retval キー値
	*/
	static int LoadTexture(const char* file_path);

	/**
	* @brief テクスチャ描画関数
	* @details 引数で受け取った頂点をもとにテクスチャを描画する
	* @param[in] pos_x 左上頂点のx座標
	* @param[in] pos_y 左上頂点のy座標
	*/
	static void DrawTexture(float pos_x, float pos_y, int key);

private:
	static Engine* instance;

	Window window;
	Input input;
	Graphics graphics;
};


#endif