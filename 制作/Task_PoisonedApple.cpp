//-------------------------------------------------------------------
// 毒リンゴ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_PoisonedApple.h"

#include  "sound.h"

namespace  PoisonedApple
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/apple_doku_ringo.png");
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
		se::LoadFile("sound", "./data/sound/poison.wav");
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

		//2秒後に自然消滅
		if (this->moveCnt++ > 60 * 2) {
			this->Kill();
		}

		//プレイヤーとの処理
		if (ge->qa_player != nullptr) {//プレイヤが存在すれば、衝突判定を行う
			ML::Box2D you = ge->qa_player->hitBase.OffsetCopy(ge->qa_player->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				//毒の時
				se::Play("sound");

				//プレイヤを進行方向とは逆方向に飛ばす
				float speed = 10;
				if (ge->qa_player->angle4 == Player::Object::Angle4::Down) { ge->qa_player->moveVec = ML::Vec2(0, -speed); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Up) { ge->qa_player->moveVec = ML::Vec2(0, +speed); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Left) { ge->qa_player->moveVec = ML::Vec2(+speed, 0); }
				else if (ge->qa_player->angle4 == Player::Object::Angle4::Right) { ge->qa_player->moveVec = ML::Vec2(-speed, 0); }

				//スコアを25％減らす
				ge->score = ge->score * 3 / 4;
				this->Kill();
				ge->CreateEffect(10, this->pos);
			}
		}

		//泥棒との処理
		for (auto it = ge->qa_thief->begin(); it != ge->qa_thief->end(); ++it) {
			ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
			ML::Box2D me = this->OffsetBox();
			if (you.Hit(me)) {
				se::Play("sound");

				this->Kill();	//プレイヤとの処理
				(*it)->Kill();	//敵を消滅させる

				ge->CreateEffect(10, this->pos);
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