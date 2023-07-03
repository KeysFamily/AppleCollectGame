//-------------------------------------------------------------------
//���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Apple.h"

#include  "sound.h"

namespace  Apple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/fruit_ringo.png");
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
		se::LoadFile("poku", "./data/sound/poku.wav");
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
		//�v���C���ƓD�_�ŏ��ł���B

		//�v���C���Ƃ̏���
		if (ge->qa_player != nullptr) {//�v���C�������݂���΁A�Փ˔�����s��
			ML::Box2D you = ge->qa_player->hitBase.OffsetCopy(ge->qa_player->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				++ge->score;
				se::Play("poku");
				ge->CreateEffect(1, this->pos);
				this->Kill();
			}
		}
		
		//�D�_�Ƃ̏���
		for (auto it = ge->qa_thief->begin(); it != ge->qa_thief->end(); ++it) {
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				//++ge->score;
				//se::Play("poku");
				ge->CreateEffect(1, this->pos);
				this->Kill();
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