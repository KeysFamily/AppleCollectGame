//-------------------------------------------------------------------
// �Ń����S
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_PoisonedApple.h"

#include  "sound.h"

namespace  PoisonedApple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/apple_doku_ringo.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.8f;
		this->hitBase = Assist::BoxCentering(48, 48);// ML::Box2D(-24, -24, 48, 48);

		//���^�X�N�̐���

		//�I�[�f�B�I
		se::LoadFile("sound", "./data/sound/poison.wav");
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{

		//2�b��Ɏ��R����
		if (this->moveCnt++ > 60 * 2) {
			this->Kill();
		}

		//�v���C���[�Ƃ̏���
		if (ge->qa_player != nullptr) {//�v���C�������݂���΁A�Փ˔�����s��
			ML::Box2D you = ge->qa_player->hitBase.OffsetCopy(ge->qa_player->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				//�ł̎�
				se::Play("sound");

				//�v���C����i�s�����Ƃ͋t�����ɔ�΂�
				float speed = 10;
				if (ge->qa_player->angle4 == Player::Object::Angle4::Down) { ge->qa_player->moveVec = ML::Vec2(0, -speed); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Up) { ge->qa_player->moveVec = ML::Vec2(0, +speed); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Left) { ge->qa_player->moveVec = ML::Vec2(+speed, 0); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Right) { ge->qa_player->moveVec = ML::Vec2(-speed, 0); }

				//�X�R�A��25�����炷
				ge->score = ge->score * 3 / 4;
				this->Kill();
				ge->CreateEffect(10, this->pos);
			}
		}

		//�D�_�Ƃ̏���
		for (auto it = ge->qa_thief->begin(); it != ge->qa_thief->end(); ++it) {
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				se::Play("sound");

				this->Kill();	//�v���C���Ƃ̏���
				(*it)->Kill();	//�G�����ł�����

				ge->CreateEffect(10, this->pos);
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = OffsetBox();
		ML::Box2D src(0, 0, 400, 400);
		this->res->img->Draw(draw, src);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}