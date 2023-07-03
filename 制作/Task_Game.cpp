//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"

#include "sound.h"

#include  "Task_Ending.h"
//�V�[�����Ŏg�p����^�X�N
#include  "Task_Player.h"
#include  "Task_Apple.h"
#include  "Task_PoisonedApple.h"
#include  "Task_AppleManager.h"
#include  "Task_Thief.h"
#include  "Task_MasterThief.h"
#include  "Task_ThiefManager.h"
#include  "Task_DrawInfo.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/grass_template2_0.png");
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
		this->render2D_Priority[1] = 0.9f;
		
		this->maxBGMVolume = 100;
		//�Q�[���G���W�����ʕϐ�
		ge->GameOverFlag = false;
		ge->score = 0;
		

		//�I�[�f�B�I�n
		se::LoadFile("ok", "./data/sound/ok.wav");
		bgm::LoadFile("bgm", "./data/sound/bgm1.mp3");
		bgm::VolumeControl("bgm", this->maxBGMVolume);
		bgm::Play("bgm");

		//���\�[�X�풓
		this->residentResource.push_back(Apple::Resource::Create());
		this->residentResource.push_back(PoisonedApple::Resource::Create());
		this->residentResource.push_back(Thief::Resource::Create());
		this->residentResource.push_back(MasterThief::Resource::Create());
		

		//���^�X�N�̐���
		auto pl = Player::Object::Create(true);
		pl->pos.x = ge->screenWidth / 2.0f;
		pl->pos.y = ge->screenHeight / 2.0f;

		AppleManager::Object::Create(true);
		ThiefManager::Object::Create(true);
		DrawInfo::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�{��");
		ge->KillAll_G(Player::defGroupName);
		ge->KillAll_G(AppleManager::defGroupName);
		ge->KillAll_G(ThiefManager::defGroupName);
		ge->KillAll_G(DrawInfo::defGroupName);

		//BGM�̒�~
		bgm::Stop("bgm");

		//���\�[�X�풓�̉���
		this->residentResource.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );

		ge->qa_player = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		ge->qa_thief = ge->GetTasks<BChara>("�D�_");
		
		//�����A�Q�[���I�[�o�[�ł���΁A�ȉ��̃^�X�N���ꎞ��~����
		ge->StopAll_G(Player::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(Apple::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(PoisonedApple::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(AppleManager::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(Thief::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(ThiefManager::defGroupName, ge->GameOverFlag);

		if (ge->GameOverFlag) {
			//�{�^���������ꂽ�Ƃ��̏���
			if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
				ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
				ge->CreateEffect(99, ML::Vec2(0, 0));
				this->decVolumeCnt = 45;
				se::Play("ok");
			}
			//45�t���[����̏���
			if (ge->getCounterFlag("test") == ge->LIMIT) {
				this->Kill();
			}
			//�t�F�[�h�A�E�g�̏���
			else if (ge->getCounterFlag("test") == ge->ACTIVE) {
				--this->decVolumeCnt;
				float nowVol = this->maxBGMVolume * this->decVolumeCnt / 45.0f;
				bgm::VolumeControl("bgm", static_cast<int>(nowVol));
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
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