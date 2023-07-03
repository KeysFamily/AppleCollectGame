//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "sound.h"

//���̃^�X�N�̓ǂݍ���
#include  "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/grass_template2_0.png");
		this->TitleLogo = DG::Image::Create("./data/image/title_text.png");
		this->StartLogo = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo.reset();
		this->StartLogo.reset();
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
		this->render2D_Priority[1] = 0.5f;
		this->controller = ge->in1;
		this->flashCnt = 0;

		//���y�t�@�C���̓ǂݍ���
		se::LoadFile("se", "./data/sound/ok.wav");

		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
			se::Play("se");
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		//�_�ŃJ�E���g����
		if (++this->flashCnt >= 60) {
			this->flashCnt = 0;
		}

		return;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
		this->res->img->Draw(draw, src);

		//�^�C�g���`��
		ML::Box2D drawTitle = Assist::BoxCentering(
			static_cast<int>(ge->screenWidth) / 2, 
			static_cast<int>(ge->screenHeight) / 3, 
			676, 88);

		ML::Box2D srcTitle = ML::Box2D(0, 0, 676, 88);
		this->res->TitleLogo->Draw(drawTitle, srcTitle);

		if (this->flashCnt / 30 == 0) {
			//���S�`��
			ML::Box2D drawStart = Assist::BoxCentering(
				static_cast<int>(ge->screenWidth) / 2,
				static_cast<int>(ge->screenHeight) * 2 / 3,
				596 * 2 / 3, 91 * 2 / 3);
			ML::Box2D srcStart = ML::Box2D(0, 0, 596, 91);
			this->res->StartLogo->Draw(drawStart, srcStart);
		}
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