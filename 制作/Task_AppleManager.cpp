//-------------------------------------------------------------------
//�����S�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "randomLib.h"
#include  "Task_AppleManager.h"

#include "Task_Apple.h"
#include "Task_PoisonedApple.h"

namespace  AppleManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G(Apple::defGroupName);
		ge->KillAll_G(PoisonedApple::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto apples = ge->GetTasks<BChara>("���");
		if (ge->qa_player == nullptr) { return; }

		int appearSheet[5] = { 0,0,1,0,0 };//�o���\ 0:�ʏ탊���S 1:�Ń����S
		int init = GetRandom<int>(0, 4);
		//�����A�����S�I�u�W�F�N�g���Ȃ���΁A��������B
		if (apples->empty()) {
			//�X�R�A�ɂ���Đ��������ς��
			for (int i = 0; i <= ge->score / 10; ++i) {
				BChara::SP apple;//��񂲁E�Ń����S��������悤�ɂ���

				ML::Box2D you;//�v���C���̓����蔻��

				//�����S�̑����ɂ���ē����蔻���ς���B
				//�����ăv���C���[�̓����蔻����傫�����āA�v���C���̋߂��ɐ������邱�Ƃ��h��
				if (ge->score >= 10 && appearSheet[(init + i) % 5] == 1) {
					apple = PoisonedApple::Object::Create(true);
					you = Assist::BoxCentering(ge->qa_player->pos, 128, 128);
				}
				else {
					apple = Apple::Object::Create(true);
					you = Assist::BoxCentering(ge->qa_player->pos, 256, 256);
				}
				ML::Vec2 posA;
				ML::Box2D me;
				do {
					//�v���C���Əd�Ȃ�Ȃ��悤�ɂ���B

					//���W�𗐐��Ō���
					posA.x = GetRandom<float>(apple->hitBase.w / 2.0f, ge->screenWidth - apple->hitBase.w / 2.0f);
					posA.y = GetRandom<float>(apple->hitBase.h / 2.0f, ge->screenHeight - apple->hitBase.h / 2.0f);

					me = apple->hitBase.OffsetCopy(posA);
				} while (you.Hit(me));//�������Ă�����J��Ԃ�

				apple->pos = posA;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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