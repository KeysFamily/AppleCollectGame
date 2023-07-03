//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"


//はみ出し防止移動処理（はみ出したらtrueを返す）
bool BChara::CharaMove(ML::Vec2 e_) {
	while (e_.x != 0.0f) {
		float preX = pos.x;
		if (e_.x >= +1.0f) { pos.x += 1.0f, e_.x -= 1.0f; }
		else if(e_.x <= -1.0f) { pos.x -= 1.0f; e_.x += 1.0f; }
		else { pos.x += e_.x; e_.x = 0.0f; }
		//当たり判定用の矩形を用意
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//当たったらゲームオーバー
			pos.x = preX;
			return true;
		}
	}

	while (e_.y != 0.0f) {
		float preY = pos.y;
		if (e_.y >= +1.0f) { pos.y += 1.0f, e_.y -= 1.0f; }
		else if (e_.y <= -1.0f) { pos.y -= 1.0f; e_.y += 1.0f; }
		else { pos.y += e_.y; e_.y = 0.0f; }
		//当たり判定用の矩形を用意
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//当たったらゲームオーバー
			pos.y = preY;
			return true;
		}
	}
	return false;
}

//画面外にでたらtrueを返す
bool BChara::CheckScreenOut(ML::Box2D hit_) {
	//学習用注釈
	//RectとBox2Dの変換について
	//ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };

	ML::Rect s{ 0,0,static_cast<int>(ge->screenWidth),static_cast<int>(ge->screenHeight) };
	ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };
	
	//どこかがはみ出していれば画面からはみ出している
	if(o.left < s.left){ return true; }
	if(o.top < s.top){ return true; }
	if (o.right > s.right) { return true; }
	if (o.bottom > s.bottom) { return true; }

	return false;
}

ML::Box2D BChara::OffsetBox() {
	return this->hitBase.OffsetCopy(this->pos);
}

//接触時の移動処理（あくまでダミー）
void BChara::Reverse(BChara* from_,int scoreFluctuation_) {
	ML::MsgBox("Received 実装されていません");
}
