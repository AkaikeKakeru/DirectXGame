﻿#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"

#include <DirectXMath.h>
using namespace DirectX;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow("LE2B_01_アカイケ_カケル_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	//頂点データ
	XMFLOAT3 vertices[] =
	{
		{-0.5f,-0.5f,0.0f},//左下
		{-0.5f,+0.5f,0.0f},//左上
		{+0.5f,-0.5f,0.0f},//右下
	};
	//全体のサイズ = 一つ分のサイズ * 要素数
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	//頂点バッファ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		gameScene->Update();
		// 軸表示の更新
		axisIndicator->Update();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		gameScene->Draw();
		// 軸表示の描画
		axisIndicator->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}