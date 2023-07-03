//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include "randomLib.h"

#include "sound.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/grass_template2_0.png");
		this->imgNext = DG::Image::Create("./data/image/StartKey.png");
		this->fontLabel = DG::Font::Create("�l�r �S�V�b�N", 15, 30);
		this->fontScore = DG::Font::Create("�l�r �S�V�b�N", 80, 100);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgNext.reset();
		this->fontLabel.reset();
		this->fontScore.reset();
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

		this->effectCnt = 0;
		this->flashCnt = 0;

		//�T�E���h�n
		se::LoadFile("poku", "./data/sound/poku.wav");
		se::LoadFile("ok", "./data/sound/ok.wav");

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
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//�����A�X�^�[�g�{�^���������ꂽ�Ƃ��̏���
		if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		//�\���p�̏���
		if (this->effectCnt <= 90) {
			
			++this->effectCnt;
			if (this->effectCnt <= 60 && this->effectCnt % 30 == 0) {
				se::Play("poku");
			}
		}
		else {
			if (++this->flashCnt > 60) {
				this->flashCnt = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�`��
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
		this->res->imgBG->Draw(draw, src);

		if (this->effectCnt >= 30) {
			//���x���\��
			ML::Box2D drawLabel(100, 200, 400, 30);
			string labelText = "���Ȃ��̏W�߂������S��";
			this->res->fontLabel->Draw(drawLabel, labelText);
		}
		if (this->effectCnt >= 60) {
			//�X�R�A�\��
			ML::Box2D drawScore(150, 250, 600, 100);

			ostringstream oss;
			oss << right << setw(4) << ge->score;
			oss << "��";
			this->res->fontScore->Draw(drawScore, oss.str());
		}
		if (this->effectCnt >= 90) {
			if (this->flashCnt / 30 == 0) {
				//���S�`��
				ML::Box2D drawNext = Assist::BoxCentering(
					static_cast<int>(ge->screenWidth) / 2,
					static_cast<int>(ge->screenHeight) * 2 / 3,
					596 * 2 / 3, 91 * 2 / 3);
				ML::Box2D srcNext = ML::Box2D(0, 0, 596, 91);
				this->res->imgNext->Draw(drawNext, srcNext);
			}
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