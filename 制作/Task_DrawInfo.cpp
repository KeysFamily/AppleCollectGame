//-------------------------------------------------------------------
//���`��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DrawInfo.h"

namespace  DrawInfo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imgFont = DG::Image::Create("./data/image/font_text.png");
		this->imgOver = DG::Image::Create("./data/image/gameOver.png");
		this->imgNext = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->imgFont.reset();
		this->imgOver.reset();
		this->imgNext.reset();
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
		this->render2D_Priority[1] = 0.1f;
		this->flashCnt = 0;

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
		if (ge->GameOverFlag) {

			if (++this->flashCnt >= 60) {
				this->flashCnt = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ostringstream oss;
		oss << "Apple:";
		oss << setw(5) << right << ge->score;
		this->RenderString(static_cast<int>(ge->screenWidth) - 32 * 11, 0, 32, 32, oss.str());

		//�����Q�[���I�[�o�[�ł����
		if (ge->GameOverFlag) {
			//�^�C�g���`��
			ML::Box2D drawTitle = Assist::BoxCentering(
				static_cast<int>(ge->screenWidth) / 2,
				static_cast<int>(ge->screenHeight) / 3,
				600, 100);

			ML::Box2D srcTitle = ML::Box2D(0, 0, 418, 74);
			this->res->imgOver->Draw(drawTitle, srcTitle);

			if (this->flashCnt / 30 == 0) {
				//���S�`��
				ML::Box2D drawStart = Assist::BoxCentering(
					static_cast<int>(ge->screenWidth) / 2,
					static_cast<int>(ge->screenHeight) * 2 / 3,
					596 * 2 / 3, 91 * 2 / 3);
				ML::Box2D srcStart = ML::Box2D(0, 0, 596, 91);
				this->res->imgNext->Draw(drawStart, srcStart);
			}
		}
	}

	//-------------------------------------------------------------------
	//������\���֐�
	void Object::RenderString(int x_, int y_, int w_, int h_, string str_) {
		for (int i = 0; i < str_.length(); ++i) {
			char t = str_[i];//�ꕶ���擾
			//int n_ = t - ' ';//�ԍ��̎擾
			int n_ = t;
			ML::Point FontSize = { 32,32 };
			ML::Box2D draw(x_ + i * w_, y_, w_, h_);
			ML::Box2D src((n_ % 16) * FontSize.x, ((n_ - (n_ % 16)) / 16) * FontSize.y, FontSize.x, FontSize.y);

			this->res->imgFont->Draw(draw, src);
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