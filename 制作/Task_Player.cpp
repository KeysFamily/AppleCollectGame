//-------------------------------------------------------------------
//　プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/chara.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化

		this->render2D_Priority[1] = 0.5f;

		this->angle4 = Angle4::Down;
		this->moveVec = ML::Vec2(0, +1);
		this->hitBase = Assist::BoxCentering(64, 64);	//ML::Box2D(-32, -32, 64, 64);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//入力取得
		auto inp = ge->in1->GetState();

		this->animCnt = (this->animCnt + 1) % 60;
		
		//速度設定
		float speed = 1.0f + min(ge->score, 7);

		if (inp.LStick.BU.down) { this->moveVec = ML::Vec2(0, -speed); this->angle4 = Angle4::Up; }
		if (inp.LStick.BD.down) { this->moveVec = ML::Vec2(0, +speed); this->angle4 = Angle4::Down; }
		if (inp.LStick.BL.down) { this->moveVec = ML::Vec2(-speed, 0); this->angle4 = Angle4::Left; }
		if (inp.LStick.BR.down) { this->moveVec = ML::Vec2(+speed, 0); this->angle4 = Angle4::Right; }
		
		//はみ出さない処理(はみ出しを検出する)
		ge->GameOverFlag = CharaMove(this->moveVec);

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		int anim[4] = { 1,0,1,2 };
		ML::Box2D draw = this->OffsetBox();
		ML::Box2D src(64 * anim[this->animCnt / 15], 64 * static_cast<int>(this->angle4), 64, 64);
		this->res->img->Draw(draw, src);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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