//-------------------------------------------------------------------
//情報描画
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_DrawInfo.h"

namespace  DrawInfo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imgFont = DG::Image::Create("./data/image/font_text.png");
		this->imgOver = DG::Image::Create("./data/image/gameOver.png");
		this->imgNext = DG::Image::Create("./data/image/StartKey.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->imgFont.reset();
		this->imgOver.reset();
		this->imgNext.reset();
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
		this->render2D_Priority[1] = 0.1f;
		this->flashCnt = 0;

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
		if (ge->GameOverFlag) {

			if (++this->flashCnt >= 60) {
				this->flashCnt = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ostringstream oss;
		oss << "Apple:";
		oss << setw(5) << right << ge->score;
		this->RenderString(static_cast<int>(ge->screenWidth) - 32 * 11, 0, 32, 32, oss.str());

		//もしゲームオーバーであれば
		if (ge->GameOverFlag) {
			//タイトル描画
			ML::Box2D drawTitle = Assist::BoxCentering(
				static_cast<int>(ge->screenWidth) / 2,
				static_cast<int>(ge->screenHeight) / 3,
				600, 100);

			ML::Box2D srcTitle = ML::Box2D(0, 0, 418, 74);
			this->res->imgOver->Draw(drawTitle, srcTitle);

			if (this->flashCnt / 30 == 0) {
				//ロゴ描画
				ML::Box2D drawStart = Assist::BoxCentering(
					static_cast<int>(ge->screenWidth) / 2,
					static_cast<int>(ge->screenHeight) * 2 / 3,
					596 * 2 / 3, 91 * 2 / 3);
				ML::Box2D srcStart = ML::Box2D(0, 0, 596, 91);
				this->res->imgNext->Draw(drawStart, srcStart);
			}
		}
	}

	//-------------------------------------------------------------------
	//文字列表示関数
	void Object::RenderString(int x_, int y_, int w_, int h_, string str_) {
		for (int i = 0; i < str_.length(); ++i) {
			char t = str_[i];//一文字取得
			//int n_ = t - ' ';//番号の取得
			int n_ = t;
			ML::Point FontSize = { 32,32 };
			ML::Box2D draw(x_ + i * w_, y_, w_, h_);
			ML::Box2D src((n_ % 16) * FontSize.x, ((n_ - (n_ % 16)) / 16) * FontSize.y, FontSize.x, FontSize.y);

			this->res->imgFont->Draw(draw, src);
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