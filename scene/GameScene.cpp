#include "GameScene.h"
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
		////X,Y,Z方向のスケーリングを設定
		//worldTransform_[i].scale_ = { 1.0f, 1.0f, 1.0f };
		////X,Y,Z軸周りの回転角を設定
		//worldTransform_[i].rotation_ = { rotDist(engine),rotDist(engine), rotDist(engine) };
		////X,Y,Z方向の平行移動を設定
		//worldTransform_[i].translation_ = { posDist(engine), posDist(engine), posDist(engine) };

		////ワールドトランスフォームの初期化
		//worldTransform_[i].Initialize();

		//大元
		worldTransform_[PartId::Root].Initialize();

		//脊椎
		worldTransform_[PartId::Spine].translation_ = { 0,4.5,0 };
		worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
		worldTransform_[PartId::Spine].Initialize();

		//胸
		worldTransform_[PartId::Chest].translation_ = { 0,1.5f,0 };
		worldTransform_[PartId::Chest].parent_ = &worldTransform_[PartId::Spine];
		worldTransform_[PartId::Chest].Initialize();

		//頭
		worldTransform_[PartId::Head].translation_ = { 0,3.0f,0 };
		worldTransform_[PartId::Head].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::Head].Initialize();

		//左腕
		worldTransform_[PartId::ArmL].translation_ = { -3.0f,0,0 };
		worldTransform_[PartId::ArmL].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::ArmL].Initialize();

		//右腕
		worldTransform_[PartId::ArmR].translation_ = { 3.0f,0,0 };
		worldTransform_[PartId::ArmR].parent_ = &worldTransform_[PartId::Chest];
		worldTransform_[PartId::ArmR].Initialize();

		//尻
		worldTransform_[PartId::Hip].translation_ = { 0,-1.5f,0 };
		worldTransform_[PartId::Hip].parent_ = &worldTransform_[PartId::Spine];
		worldTransform_[PartId::Hip].Initialize();

		//左足
		worldTransform_[PartId::LegL].translation_ = { -3.0f,-3.0f,0 };
		worldTransform_[PartId::LegL].parent_ = &worldTransform_[PartId::Hip];
		worldTransform_[PartId::LegL].Initialize();

		//右足
		worldTransform_[PartId::LegR].translation_ = { 3.0f,-3.0f,0 };
		worldTransform_[PartId::LegR].parent_ = &worldTransform_[PartId::Hip];
		worldTransform_[PartId::LegR].Initialize();
	}

	//カメラ視点座標を設定
	viewProjection_.eye = { 0,0,-50 };
	//カメラ注視点座標を設定
	viewProjection_.target = { 0,0,0 };
	//カメラ上方向ベクトルを設定 (右上45度指定)
	viewProjection_.up = { 0.0f,1.0f,0.0f };

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(45.0f);
	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//カメラ連続処理
	{
		////視点移動処理
		{
			//	//視点の移動ベクトル
			//	XMFLOAT3 move = { 0,0,0 };

			//	//視点の移動速さ
			//	const float kEyeSpeed = 0.2f;

			//	//押した方向で移動ベクトルを変更
			//	if (input_->PushKey(DIK_W)) {
			//		move = { 0,0,kEyeSpeed };
			//	} else if (input_->PushKey(DIK_S)) {
			//		move = { 0,0,-kEyeSpeed };
			//	}
			//	//視点移動(ベクトルの加算)
			//	viewProjection_.eye.x += move.x;
			//	viewProjection_.eye.y += move.y;
			//	viewProjection_.eye.z += move.z;

				//行列の再計算
			viewProjection_.UpdateMatrix();

			//デバッグ用表示
			debugText_->SetPos(50, 50);
			debugText_->Printf("eye:(%f,%f,%f)",
				viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
		}

		////注視点移動処理
		{
			//	//注視点の移動ベクトル
			//	XMFLOAT3 move = { 0,0,0 };

			//	//注視点の移動速さ
			//	const float kTargetSpeed = 0.2f;

			//	//押した方向で移動ベクトルを変更
			//	if (input_->PushKey(DIK_LEFT)) {
			//		move = { kTargetSpeed,0,0 };
			//	} else if (input_->PushKey(DIK_RIGHT)) {
			//		move = { -kTargetSpeed,0,0 };
			//	}
			//	//注視点移動(ベクトルの加算)
			//	viewProjection_.target.x += move.x;
			//	viewProjection_.target.y += move.y;
			//	viewProjection_.target.z += move.z;

				//行列の再計算
			viewProjection_.UpdateMatrix();

			//デバッグ用表示
			debugText_->SetPos(50, 70);
			debugText_->Printf("target:(%f,%f,%f)",
				viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
		}

		////上方向回転処理
		{
			//	//上方向の回転速さ[ラジアン/frame]
			//	const float kUpRotSpeed = 0.05f;

			//	//押した方向で移動ベクトルを変更
			//	if (input_->PushKey(DIK_SPACE)) {
			//		viewAngle +=  kUpRotSpeed;
			//		//2πを超えたら0に戻す
			//		viewAngle =  fmodf(viewAngle,XM_2PI);
			//	}
			//	//上方向ベクトルを計算(半径1の円周上の座標)
			//	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

				//行列の再計算
			viewProjection_.UpdateMatrix();

			//デバッグ用表示
			debugText_->SetPos(50, 90);
			debugText_->Printf("up:(%f,%f,%f)",
				viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
		}

		//FoV変更処理
		{
			////上のキーで視野角が広がる
			//if (input_->PushKey(DIK_W)) {
			//	viewProjection_.fovAngleY += 0.01f;
			//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, XM_PI);
			//	//下キーで視野角が狭まる
			//}
			//else if (input_->PushKey(DIK_S)) {
			//	viewProjection_.fovAngleY -= 0.01f;
			//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
			//}

			//行列の再計算
			viewProjection_.UpdateMatrix();

			//デバッグ用表示
			debugText_->SetPos(50, 110);
			debugText_->Printf("fovAngleY(Degree):%f",
				XMConvertToDegrees(viewProjection_.fovAngleY));
		}

		//クリップ距離変更処理
		{
			////上下キーでニアクリップ距離を増減
			//if (input_->PushKey(DIK_UP)) {
			//	viewProjection_.nearZ += 0.1f;	
			//} else if (input_->PushKey(DIK_DOWN)) {
			//	viewProjection_.nearZ -= 0.1f;	;
			//}

			//行列の再計算
			viewProjection_.UpdateMatrix();

			//デバッグ用表示
			debugText_->SetPos(50, 130);
			debugText_->Printf("nearZ:%f",
				XMConvertToDegrees(viewProjection_.nearZ));
		}
	}

	//キャラクターの移動処理
	{
		//キャラクターの移動ベクトル
		XMFLOAT3 move = { 0,0,0 };

		//キャラクターの移動速さ
		const float kCharacterSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed,0,0 };
		}

		//注視点移動(ベクトルの加算)
		worldTransform_[PartId::Root].translation_.x += move.x;
		worldTransform_[PartId::Root].translation_.y += move.y;
		worldTransform_[PartId::Root].translation_.z += move.z;

		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf("Root:(%f,%f,%f)",
			worldTransform_[PartId::Root].translation_.x, worldTransform_[PartId::Root].translation_.y, worldTransform_[PartId::Root].translation_.z);
	}

	//上半身回転処理
	{
		//上半身回転の速さ[ラジアン/frame]
		const float kChestSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransform_[PartId::Chest].rotation_.y -= kChestSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransform_[PartId::Chest].rotation_.y += kChestSpeed;
		}
	}

	//下半身回転処理
	{
		//下半身回転の速さ[ラジアン/frame]
		const float kHipSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransform_[PartId::Hip].rotation_.y -= kHipSpeed;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransform_[PartId::Hip].rotation_.y += kHipSpeed;
		}
	}

	worldTransform_[Root].UpdateMatrix();
	worldTransform_[Spine].UpdateMatrix();
	worldTransform_[Head].UpdateMatrix();
	worldTransform_[Chest].UpdateMatrix();
	worldTransform_[ArmL].UpdateMatrix();
	worldTransform_[ArmR].UpdateMatrix();
	worldTransform_[Hip].UpdateMatrix();
	worldTransform_[LegL].UpdateMatrix();
	worldTransform_[LegR].UpdateMatrix();
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
	/*for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}*/

	//model_->Draw(worldTransform_[Root], viewProjection_, textureHandle_);
	//model_->Draw(worldTransform_[Spine], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransform_[LegR], viewProjection_, textureHandle_);

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