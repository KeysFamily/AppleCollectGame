//-------------------------------------------------------------------
//エンディング
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include "randomLib.h"

#include "sound.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/grass_template2_0.png");
		this->imgNext = DG::Image::Create("./data/image/StartKey.png");
		this->fontLabel = DG::Font::Create("ＭＳ ゴシック", 15, 30);
		this->fontScore = DG::Font::Create("ＭＳ ゴシック", 80, 100);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		this->imgNext.reset();
		this->fontLabel.reset();
		this->fontScore.reset();
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

		this->effectCnt = 0;
		this->flashCnt = 0;

		//サウンド系
		se::LoadFile("poku", "./data/sound/poku.wav");
		se::LoadFile("ok", "./data/sound/ok.wav");

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
			auto  nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		//もし、スタートボタンが押されたときの処理
		if (inp.ST.down && ge->getCounterFlag("test") != ge->ACTIVE) {
			se::Play("ok");
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(99, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}

		//表示用の処理
		if (this->effectCnt <= 90) {
			
			++this->effectCnt;
			if (this->effectCnt <= 60 && this->effectCnt % 30 == 0) {
				se::Play("poku");
			}
		}
		else {
			if (++this->flashCnt > 60) {
				this->flashCnt = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景描画
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 1280, 720);
		this->res->imgBG->Draw(draw, src);

		if (this->effectCnt >= 30) {
			//ラベル表示
			ML::Box2D drawLabel(100, 200, 400, 30);
			string labelText = "あなたの集めたリンゴは";
			this->res->fontLabel->Draw(drawLabel, labelText);
		}
		if (this->effectCnt >= 60) {
			//スコア表示
			ML::Box2D drawScore(150, 250, 600, 100);

			ostringstream oss;
			oss << right << setw(4) << ge->score;
			oss << "個";
			this->res->fontScore->Draw(drawScore, oss.str());
		}
		if (this->effectCnt >= 90) {
			if (this->flashCnt / 30 == 0) {
				//ロゴ描画
				ML::Box2D drawNext = Assist::BoxCentering(
					static_cast<int>(ge->screenWidth) / 2,
					static_cast<int>(ge->screenHeight) * 2 / 3,
					596 * 2 / 3, 91 * 2 / 3);
				ML::Box2D srcNext = ML::Box2D(0, 0, 596, 91);
				this->res->imgNext->Draw(drawNext, srcNext);
			}
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