//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"

#include "sound.h"

#include  "Task_Ending.h"
//シーン内で使用するタスク
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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/grass_template2_0.png");
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
		this->render2D_Priority[1] = 0.9f;
		
		this->maxBGMVolume = 100;
		//ゲームエンジン共通変数
		ge->GameOverFlag = false;
		ge->score = 0;
		

		//オーディオ系
		se::LoadFile("ok", "./data/sound/ok.wav");
		bgm::LoadFile("bgm", "./data/sound/bgm1.mp3");
		bgm::VolumeControl("bgm", this->maxBGMVolume);
		bgm::Play("bgm");

		//リソース常駐
		this->residentResource.push_back(Apple::Resource::Create());
		this->residentResource.push_back(PoisonedApple::Resource::Create());
		this->residentResource.push_back(Thief::Resource::Create());
		this->residentResource.push_back(MasterThief::Resource::Create());
		

		//★タスクの生成
		auto pl = Player::Object::Create(true);
		pl->pos.x = ge->screenWidth / 2.0f;
		pl->pos.y = ge->screenHeight / 2.0f;

		AppleManager::Object::Create(true);
		ThiefManager::Object::Create(true);
		DrawInfo::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("本編");
		ge->KillAll_G(Player::defGroupName);
		ge->KillAll_G(AppleManager::defGroupName);
		ge->KillAll_G(ThiefManager::defGroupName);
		ge->KillAll_G(DrawInfo::defGroupName);

		//BGMの停止
		bgm::Stop("bgm");

		//リソース常駐の解除
		this->residentResource.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			auto next = Ending::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState( );

		ge->qa_player = ge->GetTask<Player::Object>(Player::defGroupName, Player::defName);
		ge->qa_thief = ge->GetTasks<BChara>("泥棒");
		
		//もし、ゲームオーバーであれば、以下のタスクを一時停止する
		ge->StopAll_G(Player::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(Apple::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(PoisonedApple::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(AppleManager::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(Thief::defGroupName, ge->GameOverFlag);
		ge->StopAll_G(ThiefManager::defGroupName, ge->GameOverFlag);

		if (ge->GameOverFlag) {
			//ボタンが押されたときの処理
			if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
				ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
				ge->CreateEffect(99, ML::Vec2(0, 0));
				this->decVolumeCnt = 45;
				se::Play("ok");
			}
			//45フレーム後の処理
			if (ge->getCounterFlag("test") == ge->LIMIT) {
				this->Kill();
			}
			//フェードアウトの処理
			else if (ge->getCounterFlag("test") == ge->ACTIVE) {
				--this->decVolumeCnt;
				float nowVol = this->maxBGMVolume * this->decVolumeCnt / 45.0f;
				bgm::VolumeControl("bgm", static_cast<int>(nowVol));
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
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