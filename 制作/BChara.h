#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^

	

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
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

	//�ڐG���̏���(�󂯐g�̏���)
	//��j�u���Q�ҁv���u��Q�ҁv�ɍU��
	//�u���Q�ҁv	HitCheck�Ȃ�
	//�u��Q�ҁv	Reverse
	//�I�u�W�F�N�g�ƃX�R�A�ϓ����i�����ʁj
	virtual void Reverse(BChara* from_,int scoreFluctuation_);
};
