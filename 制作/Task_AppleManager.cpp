//-------------------------------------------------------------------
//リンゴマネージャ
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G(Apple::defGroupName);
		ge->KillAll_G(PoisonedApple::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto apples = ge->GetTasks<BChara>("りんご");
		if (ge->qa_player == nullptr) { return; }

		int appearSheet[5] = { 0,0,1,0,0 };//出現表 0:通常リンゴ 1:毒リンゴ
		int init = GetRandom<int>(0, 4);
		//もし、リンゴオブジェクトがなければ、生成する。
		if (apples->empty()) {
			//スコアによって生成個数が変わる
			for (int i = 0; i <= ge->score / 10; ++i) {
				BChara::SP apple;//りんご・毒リンゴを扱えるようにする

				ML::Box2D you;//プレイヤの当たり判定

				//リンゴの属性によって当たり判定を変える。
				//あえてプレイヤーの当たり判定より大きくして、プレイヤの近くに生成することも防ぐ
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
					//プレイヤと重ならないようにする。

					//座標を乱数で決定
					posA.x = GetRandom<float>(apple->hitBase.w / 2.0f, ge->screenWidth - apple->hitBase.w / 2.0f);
					posA.y = GetRandom<float>(apple->hitBase.h / 2.0f, ge->screenHeight - apple->hitBase.h / 2.0f);

					me = apple->hitBase.OffsetCopy(posA);
				} while (you.Hit(me));//当たっていたら繰り返す

				apple->pos = posA;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
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