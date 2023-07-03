//-------------------------------------------------------------------
//�D�_�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_ThiefManager.h"
#include  "randomLib.h"

#include  "Task_Thief.h"
#include  "Task_MasterThief.h"
#include  "Task_Player.h"

namespace  ThiefManager
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
		ge->KillAll_G(Thief::defGroupName);
		ge->KillAll_G(MasterThief::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�����D�_�����Ȃ����
		int appearSheet[5] = { 0,0,0,1,0 };
		int init = GetRandom<int>(0, 4);
		if (ge->qa_thief->empty()) {
			for (int i = 0; i < ge->score / 30; ++i) {

				if (appearSheet[(init + i) % 5] == 1) {
					auto master = MasterThief::Object::Create(true);

					float height = ge->qa_player->pos.y;
					ML::Vec2 posA;//�o�����W

					//���������肷��
					if (ge->qa_player->angle4 == Player::Object::Angle4::Left) {
						//�������̎�
						master->angle2 = MasterThief::Object::Angle2::Right;
					}
					else if (ge->qa_player->angle4 == Player::Object::Angle4::Right) {
						//�E�����̎�
						master->angle2 = MasterThief::Object::Angle2::Left;
					}
					else {
						//�㉺�����̎�
						master->angle2 = (GetRandom<int>(0, 9) % 2 == 0 ? MasterThief::Object::Angle2::Left : MasterThief::Object::Angle2::Right);
					}

					//�o���ʒu�������̉����Č��߂�
					if (master->angle2 == MasterThief::Object::Angle2::Left) {
						//������--
						posA = ML::Vec2(ge->screenWidth + master->hitBase.w / 2.0f, height);
					}
					else {
						//�E����++
						posA = ML::Vec2(-master->hitBase.w / 2.0f, height);
					}
					master->pos = posA;
				}
				else {
					//�D�_����
					auto thief = Thief::Object::Create(true);

					ML::Vec2 posA;//�o�����W

					//�����m��
					float height = GetRandom<float>(thief->hitBase.w / 2.0f, ge->screenHeight - thief->hitBase.w / 2.0f);

					if (GetRandom<int>(0, 9) % 2 == 0) {
						//�������i--x�j
						thief->angle2 = Thief::Object::Angle2::Left;
						posA = ML::Vec2(ge->screenWidth + thief->hitBase.w / 2.0f, height);

					}
					else {
						//�E����(++x)
						thief->angle2 = Thief::Object::Angle2::Right;
						posA = ML::Vec2(-thief->hitBase.w / 2.0f, height);
					}
					thief->pos = posA;

				}
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