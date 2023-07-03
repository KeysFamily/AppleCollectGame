//-------------------------------------------------------------------
//�@�D�_
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Thief.h"

namespace  Thief
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/thief.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = Assist::BoxCentering(53, 64);
		this->hitPlayer = false;
		
		//���^�X�N�̐���

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
		float speed = this->hitPlayer ? 6.0f : 3.0f;
		
		if (this->angle2 == Angle2::Left) {
			//--
			this->moveVec = ML::Vec2(-speed, 0);
		}
		else {
			//++
			this->moveVec = ML::Vec2(speed, 0);
		}
		this->pos += this->moveVec;

		//��ʊO�ɏo�����_�ŏ��ł���
		if ((this->angle2 == Angle2::Left && this->pos.x < -this->hitBase.w / 2.0f) ||
			(this->angle2 == Angle2::Right && this->pos.x > ge->screenWidth + this->hitBase.w / 2.0f)) {
			this->Kill();
		}

		//�v���C���Ƃ̐ڐG����i�v���C���[�ƈ�x�����Փ˂�����j
		if (ge->qa_player != nullptr && !this->hitPlayer) {
			ML::Box2D you = ge->qa_player->hitBase.OffsetCopy(ge->qa_player->pos);
			ML::Box2D me = this->OffsetBox();

			if (you.Hit(me)) {
				this->hitPlayer = true;
				ge->score = ge->score * 4 / 5;
			}

		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->OffsetBox();
		
		ML::Box2D src/*(0, 0, 362, 400)*/;
		if (this->angle2 == Angle2::Left) {
			src = ML::Box2D(0, 0, 362, 400);
		}
		else {
			src = ML::Box2D(362, 0, -362, 400);
		}


		//�����A�v���C���[�ƈ�x�ł��Փ˂��Ă���Δ������ɂ���
		this->res->img->Draw(draw, src, ML::Color(this->hitPlayer ? 0.5f : 1.0f, 1.0f, 1.0f, 1.0f));
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