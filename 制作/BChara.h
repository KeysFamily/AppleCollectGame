#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2    pos;		//キャラクタ位置
	ML::Box2D   hitBase;	//あたり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ

	

	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
	{
	}
	virtual  ~BChara() {}

protected:
	bool CharaMove(ML::Vec2 e_);
	bool CheckScreenOut(ML::Box2D hit_);
	ML::Box2D OffsetBox();

	//接触時の処理(受け身の処理)
	//例）「加害者」が「被害者」に攻撃
	//「加害者」	HitCheckなど
	//「被害者」	Reverse
	//オブジェクトとスコア変動数（増減量）
	virtual void Reverse(BChara* from_,int scoreFluctuation_);
};
