//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"


//�͂ݏo���h�~�ړ������i�͂ݏo������true��Ԃ��j
bool BChara::CharaMove(ML::Vec2 e_) {
	while (e_.x != 0.0f) {
		float preX = pos.x;
		if (e_.x >= +1.0f) { pos.x += 1.0f, e_.x -= 1.0f; }
		else if(e_.x <= -1.0f) { pos.x -= 1.0f; e_.x += 1.0f; }
		else { pos.x += e_.x; e_.x = 0.0f; }
		//�����蔻��p�̋�`��p��
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//����������Q�[���I�[�o�[
			pos.x = preX;
			return true;
		}
	}

	while (e_.y != 0.0f) {
		float preY = pos.y;
		if (e_.y >= +1.0f) { pos.y += 1.0f, e_.y -= 1.0f; }
		else if (e_.y <= -1.0f) { pos.y -= 1.0f; e_.y += 1.0f; }
		else { pos.y += e_.y; e_.y = 0.0f; }
		//�����蔻��p�̋�`��p��
		ML::Box2D hit = hitBase.OffsetCopy(pos);
		if (this->CheckScreenOut(hit)) {
			//����������Q�[���I�[�o�[
			pos.y = preY;
			return true;
		}
	}
	return false;
}

//��ʊO�ɂł���true��Ԃ�
bool BChara::CheckScreenOut(ML::Box2D hit_) {
	//�w�K�p����
	//Rect��Box2D�̕ϊ��ɂ���
	//ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };

	ML::Rect s{ 0,0,static_cast<int>(ge->screenWidth),static_cast<int>(ge->screenHeight) };
	ML::Rect o{ hit_.x,hit_.y ,hit_.x + hit_.w,hit_.y + hit_.h };
	
	//�ǂ������͂ݏo���Ă���Ή�ʂ���͂ݏo���Ă���
	if(o.left < s.left){ return true; }
	if(o.top < s.top){ return true; }
	if (o.right > s.right) { return true; }
	if (o.bottom > s.bottom) { return true; }

	return false;
}

ML::Box2D BChara::OffsetBox() {
	return this->hitBase.OffsetCopy(this->pos);
}

//�ڐG���̈ړ������i�����܂Ń_�~�[�j
void BChara::Reverse(BChara* from_,int scoreFluctuation_) {
	ML::MsgBox("Received ��������Ă��܂���");
}
