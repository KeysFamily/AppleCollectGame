//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "sound.h"

//次のタスクの読み込み
#include  "Task_Game.h"

namespace  Title
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/grass_template2_0.png");
		this->TitleLogo = DG::Image::Create("./data/image/title_text.png");
		this->StartLogo = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		this->TitleLogo.reset();
		this->StartLogo.reset();
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
		this->controller = ge->in1;
		this->flashCnt = 0;

		//音楽ファイルの読み込み
		se::LoadFile("se", "./data/sound/ok.wav");

		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = this->controller->GetState();

		if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
			se::Play("se");
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		//点滅カウント処理
		if (++this->flashCnt >= 60) {
			this->flashCnt = 0;
		}

		return;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景描画
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
		this->res->img->Draw(draw, src);

		//タイトル描画
		ML::Box2D drawTitle = Assist::BoxCentering(
			static_cast<int>(ge->screenWidth) / 2, 
			static_cast<int>(ge->screenHeight) / 3, 
			676, 88);

		ML::Box2D srcTitle = ML::Box2D(0, 0, 676, 88);
		this->res->TitleLogo->Draw(drawTitle, srcTitle);

		if (this->flashCnt / 30 == 0) {
			//ロゴ描画
			ML::Box2D drawStart = Assist::BoxCentering(
				static_cast<int>(ge->screenWidth) / 2,
				static_cast<int>(ge->screenHeight) * 2 / 3,
				596 * 2 / 3, 91 * 2 / 3);
			ML::Box2D srcStart = ML::Box2D(0, 0, 596, 91);
			this->res->StartLogo->Draw(drawStart, srcStart);
		}
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