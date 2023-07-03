//-------------------------------------------------------------------
//泥棒マネージャ
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
		ge->KillAll_G(Thief::defGroupName);
		ge->KillAll_G(MasterThief::defGroupName);

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//もし泥棒がいなければ
		int appearSheet[5] = { 0,0,0,1,0 };
		int init = GetRandom<int>(0, 4);
		if (ge->qa_thief->empty()) {
			for (int i = 0; i < ge->score / 30; ++i) {

				if (appearSheet[(init + i) % 5] == 1) {
					auto master = MasterThief::Object::Create(true);

					float height = ge->qa_player->pos.y;
					ML::Vec2 posA;//出現座標

					//向きを決定する
					if (ge->qa_player->angle4 == Player::Object::Angle4::Left) {
						//左向きの時
						master->angle2 = MasterThief::Object::Angle2::Right;
					}
					else if (ge->qa_player->angle4 == Player::Object::Angle4::Right) {
						//右向きの時
						master->angle2 = MasterThief::Object::Angle2::Left;
					}
					else {
						//上下向きの時
						master->angle2 = (GetRandom<int>(0, 9) % 2 == 0 ? MasterThief::Object::Angle2::Left : MasterThief::Object::Angle2::Right);
					}

					//出現位置を向きの応じて決める
					if (master->angle2 == MasterThief::Object::Angle2::Left) {
						//左向き--
						posA = ML::Vec2(ge->screenWidth + master->hitBase.w / 2.0f, height);
					}
					else {
						//右向き++
						posA = ML::Vec2(-master->hitBase.w / 2.0f, height);
					}
					master->pos = posA;
				}
				else {
					//泥棒生成
					auto thief = Thief::Object::Create(true);

					ML::Vec2 posA;//出現座標

					//高さ確定
					float height = GetRandom<float>(thief->hitBase.w / 2.0f, ge->screenHeight - thief->hitBase.w / 2.0f);

					if (GetRandom<int>(0, 9) % 2 == 0) {
						//左向き（--x）
						thief->angle2 = Thief::Object::Angle2::Left;
						posA = ML::Vec2(ge->screenWidth + thief->hitBase.w / 2.0f, height);

					}
					else {
						//右向き(++x)
						thief->angle2 = Thief::Object::Angle2::Right;
						posA = ML::Vec2(-thief->hitBase.w / 2.0f, height);
					}
					thief->pos = posA;

				}
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