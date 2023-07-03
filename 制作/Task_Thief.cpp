//-------------------------------------------------------------------
//　泥棒
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Thief.h"

namespace  Thief
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/thief.png");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = Assist::BoxCentering(53, 64);
		this->hitPlayer = false;
		
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

		//画面外に出た時点で消滅する
		if ((this->angle2 == Angle2::Left && this->pos.x < -this->hitBase.w / 2.0f) ||
			(this->angle2 == Angle2::Right && this->pos.x > ge->screenWidth + this->hitBase.w / 2.0f)) {
			this->Kill();
		}

		//プレイヤとの接触判定（プレイヤーと一度だけ衝突させる）
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
	//「２Ｄ描画」１フレーム毎に行う処理
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


		//もし、プレイヤーと一度でも衝突していれば半透明にする
		this->res->img->Draw(draw, src, ML::Color(this->hitPlayer ? 0.5f : 1.0f, 1.0f, 1.0f, 1.0f));
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