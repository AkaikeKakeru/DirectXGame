﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();


	//ファイル名を指定して読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンス・ツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);


	for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
	//X,Y,Z方向のスケーリングを設定
	worldTransform_[i].scale_ = { 1.0f, 1.0f, 1.0f };
	//X,Y,Z軸周りの回転角を設定
	worldTransform_[i].rotation_ = {rotDist(engine),rotDist(engine), rotDist(engine)};
	//X,Y,Z方向の平行移動を設定
	worldTransform_[i].translation_ = { posDist(engine), posDist(engine), posDist(engine) };

	//ワールドトランスフォームの初期化
	worldTransform_[i].Initialize();

	}

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,0,-10 };
	//カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	/*debugText_->SetPos(50, 50);
	debugText_->Printf("transform:(%f,%f,%f)",
		worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("rotation:(%f,%f,%f)",
		worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf("scale:(%f,%f,%f)",
		worldTransform_.scale_.x, worldTransform_.scale_.y, worldTransform_.scale_.z);*/

	//視点移動処理
	{
		//視点の移動ベクトル
		XMFLOAT3 move = { 0,0,0 };

		//視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move = { 0,0,kEyeSpeed };
		} else if (input_->PushKey(DIK_S)) {
			move = { 0,0,-kEyeSpeed };
		}
		//視点移動(ベクトルの加算)
		viewProjection_.eye.x += move.x;
		viewProjection_.eye.y += move.y;
		viewProjection_.eye.z += move.z;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 50);
		debugText_->Printf("eye:(%f,%f,%f)",
			viewProjection_.eye.x,viewProjection_.eye.y,viewProjection_.eye.z);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
	model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
