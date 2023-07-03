//-------------------------------------------------------------------
//�@�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara.png");
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

		this->render2D_Priority[1] = 0.5f;

		this->angle4 = Angle4::Down;
		this->moveVec = ML::Vec2(0, +1);
		this->hitBase = Assist::BoxCentering(64, 64);	//ML::Box2D(-32, -32, 64, 64);
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
		//���͎擾
		auto inp = ge->in1->GetState();

		this->animCnt = (this->animCnt + 1) % 60;
		
		//���x�ݒ�
		float speed = 1.0f + min(ge->score, 7);

		if (inp.LStick.BU.down) { this->moveVec = ML::Vec2(0, -speed); this->angle4 = Angle4::Up; }
		if (inp.LStick.BD.down) { this->moveVec = ML::Vec2(0, +speed); this->angle4 = Angle4::Down; }
		if (inp.LStick.BL.down) { this->moveVec = ML::Vec2(-speed, 0); this->angle4 = Angle4::Left; }
		if (inp.LStick.BR.down) { this->moveVec = ML::Vec2(+speed, 0); this->angle4 = Angle4::Right; }
		
		//�͂ݏo���Ȃ�����(�͂ݏo�������o����)
		ge->GameOverFlag = CharaMove(this->moveVec);

	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		int anim[4] = { 1,0,1,2 };
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(64 * anim[this->animCnt / 15], 64 * static_cast<int>(this->angle4), 64, 64);
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