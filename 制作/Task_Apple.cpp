//-------------------------------------------------------------------
//りんご
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Apple.h"

#include  "sound.h"

namespace  Apple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/fruit_ringo.png");
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
		this->render2D_Priority[1] = 0.8f;
		this->hitBase = Assist::BoxCentering(48, 48);// ML::Box2D(-24, -24, 48, 48);

		//★タスクの生成

		//オーディオ
		se::LoadFile("poku", "./data/sound/poku.wav");
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
		//プレイヤと泥棒で消滅する。

		//プレイヤとの処理
		if (ge->qa_player != nullptr) {//プレイヤが存在すれば、衝突判定を行う
			ML::Box2D you = ge->qa_player->hitBase.OffsetCopy(ge->qa_player->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				++ge->score;
				se::Play("poku");
				ge->CreateEffect(1, this->pos);
				this->Kill();
			}
		}
		
		//泥棒との処理
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
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = OffsetBox();
		ML::Box2D src(0, 0, 400, 400);
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