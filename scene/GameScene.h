#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
		/// <summary>
		/// コンストクラタ
		/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//パーツID
	enum PartId {
		Root,	//大元
		Spine,	//脊椎
		Chest,	//胸
		Head,	//頭
		ArmL,	//左腕
		ArmR,	//右腕
		Hip,	//尻
		LegL,	//左足
		LegR,	//右足
	};

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//テクスチャ読み込み
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_[100];

	//ビュープロジェクション
	ViewProjection viewProjection_;

	
 
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};