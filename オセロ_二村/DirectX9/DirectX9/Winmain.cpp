
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>


#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include "ExternGV.h"

#include <random>


//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		//メッセージキューに
		//新しくWM_QUITメッセージを送る

		//いずれメッセージキューから
		//取り出され
		//メインループが終了する
		//(メインループの終了条件を
		//そのように作る)
		break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;

	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex);
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
					//正しくウィンドウの作成ができたら
					//この変数に識別子を代入する
	int width = 675,	//クライアント領域の幅
	int height = 375)	//クライアント領域の高さ
						//クライアント領域はウィンドウ全体から
						//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("REVERSE"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

								//ウィンドウが正しく作れたかをチェック
								//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}


//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstance,	//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	int nCmdShow)		//ウィンドウの表示状態
{
	//メッセージボックス
	MessageBox(NULL,		//ウィンドウのハンドル 
		TEXT("テスト"),		//本文
		TEXT("テスト-タイトル"),//タイトル
		MB_OK);				//メッセージボックスのタイプ
							//MB_OK  okのボタンが表示

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウクラス設定失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	//正しくウィンドウが作成されれば
	//hWndにウィンドウの識別ハンドルが入る
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device作成成功"),
			TEXT("テスト-タイトル"),
			MB_OK);
	}

	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHATEST);

	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	int Xcount, Ycount;
	bool Mouseflag, Reverseflag;
	bool ReverseCount;

	//マウス座標
	int MousePosX, MousePosY,

	//駒の数
		blackCount, whiteCount, ChooseableCount;


	//盤面の状態
	PieceState PS[MapSize][MapSize];

	PieceState
	InversionVariable, //反転変数
	DecisionVariable;  //判断変数

	//駒を表す画像サイズ
	int PieceImageSize = BoardImageSize / MapSize;

	//盤面の画像を設定
	Sprite sprite_img_Board;
	Texture img_Board;

	sprite_img_Board.SetAlpha(1);
	sprite_img_Board.SetSize(BoardImageSize, BoardImageSize);
	sprite_img_Board.SetAngle(0);

	img_Board.Load(_T("Texture/Board.png"));
	img_Board.SetDivide(0, 0);
	img_Board.SetNum(0, 0);

	sprite_img_Board.SetPos(BoardImageSize / 2, BoardImageSize / 2);

	//駒の画像を設定
	Sprite sprite_img_Piece[MapSize][MapSize];
	Texture img_Piece[MapSize][MapSize];

	//選択可能領域の画像
	Sprite sprite_img_Chooseable[MapSize][MapSize];
	Texture img_Chooseable[MapSize][MapSize];

	for (int countY = 0; countY < MapSize; countY++)
	{
		for (int countX = 0; countX < MapSize; countX++)
		{
			//駒の画像を設定
			sprite_img_Piece[countY][countX].SetAlpha(1);
			sprite_img_Piece[countY][countX].SetSize(PieceImageSize, PieceImageSize);
			sprite_img_Piece[countY][countX].SetAngle(0);

			img_Piece[countY][countX].Load(_T("Texture/Piece.png"));
			img_Piece[countY][countX].SetDivide(8, 0);

			//選択可能領域の画像
			sprite_img_Chooseable[countY][countX].SetAlpha(1);
			sprite_img_Chooseable[countY][countX].SetSize(PieceImageSize+14, PieceImageSize+14);
			sprite_img_Chooseable[countY][countX].SetAngle(0);

			img_Chooseable[countY][countX].Load(_T("Texture/Chooseable.png"));
			img_Chooseable[countY][countX].SetDivide(0, 0);
			img_Chooseable[countY][countX].SetNum(0, 0);
		}
	}

	//カーソルの画像を設定
	Sprite sprite_img_cursor;
	Texture img_cursor;
	sprite_img_cursor.SetAlpha(1);
	sprite_img_cursor.SetSize(50.0, 50.0);
	sprite_img_cursor.SetAngle(0);

	img_cursor.Load(_T("Texture/Cursor.png"));
	img_cursor.SetDivide(0, 0);
	img_cursor.SetNum(0, 0);

	//スコアボードの画像を設定
	Sprite sprite_img_ScoreBoard;
	Texture img_ScoreBoard;
	sprite_img_ScoreBoard.SetAlpha(1);
	sprite_img_ScoreBoard.SetSize(250.0, 375.0);
	sprite_img_ScoreBoard.SetAngle(0);
	sprite_img_ScoreBoard.SetPos(550.0, 190.0);

	img_ScoreBoard.Load(_T("Texture/ScoreBoard.png"));
	img_ScoreBoard.SetDivide(0, 0);
	img_ScoreBoard.SetNum(0, 0);

	//スコアの画像を設定
	//白
	Sprite sprite_img_PieceScoreW;
	Texture img_PieceScoreW;
	sprite_img_PieceScoreW.SetAlpha(1);
	sprite_img_PieceScoreW.SetSize(PieceImageSize, PieceImageSize);
	sprite_img_PieceScoreW.SetAngle(0);
	sprite_img_PieceScoreW.SetPos(510.0, 200.0 + (float)PieceImageSize / 2.0 + 2.0);

	img_PieceScoreW.Load(_T("Texture/Piece.png"));
	img_PieceScoreW.SetDivide(8, 0);
	img_PieceScoreW.SetNum(0, 0);

	//黒
	Sprite sprite_img_PieceScoreB;
	Texture img_PieceScoreB;
	sprite_img_PieceScoreB.SetAlpha(1);
	sprite_img_PieceScoreB.SetSize(PieceImageSize, PieceImageSize);
	sprite_img_PieceScoreB.SetAngle(0);
	sprite_img_PieceScoreB.SetPos(510.0, 100.0 + (float)PieceImageSize / 2.0 + 2.0);

	img_PieceScoreB.Load(_T("Texture/Piece.png"));
	img_PieceScoreB.SetDivide(8, 0);
	img_PieceScoreB.SetNum(7, 0);

	//勝ち、負け、引き分けの画像設定
	Sprite sprite_img_WLD[2];
	Texture img_WLD[2];
	for (int count = 0; count < 2; count++)
	{
		sprite_img_WLD[count].SetAlpha(1);
		sprite_img_WLD[count].SetSize(90.0, 30.0);
		sprite_img_WLD[count].SetAngle(0);
		sprite_img_WLD[count].SetPos(490.0, count*100.0 + (float)PieceImageSize / 2.0 + 67.0);

		img_WLD[count].Load(_T("Texture/WLD.png"));
		img_WLD[count].SetDivide(3, 0);
	}
	
	//駒の数の画像設定
	//白
	Sprite sprite_img_NumberW[2];
	Texture img_NumberW[2];
	for (int count = 0; count < 2; count++)
	{
		sprite_img_NumberW[count].SetAlpha(1);
		sprite_img_NumberW[count].SetSize(30.0, 30.0);
		sprite_img_NumberW[count].SetAngle(0);
		sprite_img_NumberW[count].SetPos(count*20.0 + 560.0, 200.0 + (float)PieceImageSize / 2.0 + 2.0);

		img_NumberW[count].Load(_T("Texture/Number.png"));
		img_NumberW[count].SetDivide(10, 0);
	}

	Sprite sprite_img_NumberB[2];
	Texture img_NumberB[2];
	for (int count = 0; count < 2; count++)
	{
		sprite_img_NumberB[count].SetAlpha(1);
		sprite_img_NumberB[count].SetSize(30.0, 30.0);
		sprite_img_NumberB[count].SetAngle(0);
		sprite_img_NumberB[count].SetPos(count*20.0 + 560.0, 100.0 + (float)PieceImageSize / 2.0 + 2.0);

		img_NumberB[count].Load(_T("Texture/Number.png"));
		img_NumberB[count].SetDivide(10, 0);
	}

	Game_MODE Mode;
	Mode = GameStartProcessing; //初期モードを演算処理モードに設定

	Game_WB_MODE WB;

	MSG msg = {};

	//メインプログラム-------------------------------------------------
	//quitメッセージが出てくるまでループを繰り返す
	//quitメッセージは上記のウィンドウプロシージャから送信
	//送信の条件などはウィンドウプロシージャを確認
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し
			//それをメッセージキューにポストする
			TranslateMessage(&msg);

			//メッセージの割り当て
			//ウィンドウプロシージャでメッセージを処理
			DispatchMessage(&msg);
		}
		else
		{
			pDi->Update();//キー状態の更新

			switch (Mode)
			{

			//スタート画面
			case Game_MODE::StartScreenProcessing:

				break;

			//初期処理
			case Game_MODE::GameStartProcessing:

				//黒の順番に指定
				WB = Mode_black;

				//反転変数を白に
				InversionVariable = white;

				//判断変数を黒に
				DecisionVariable = black;

				blackCount = 0;
				whiteCount = 0;
				ChooseableCount = 0;
				ReverseCount = false;

				Mouseflag = false;
				Reverseflag = false;

				//盤面のリセット
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						PS[countY][countX] = null;
					}
				}

				//初期の盤面
				PS[3][3] = white; PS[4][4] = white;
				PS[3][4] = black; PS[4][3] = black;

				//選択可能処理モードに移動
				Mode = SelectableProcessing;

				break;

				//選択可能処理モード
			case Game_MODE::SelectableProcessing:

				//選択可能処理のリセット
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						if (PS[countY][countX] == Chooseable)
						{
							PS[countY][countX] = null;
						}
					}
				}

				//選択可能処理
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						//駒が白である場合。（順番が黒の場合）
						if (PS[countY][countX] == InversionVariable)
						{
							//→方向
							if (countX != 0 && countX + 1 < MapSize)
							{
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//黒だった場合
									if (PS[countY][Xcount] == DecisionVariable)
									{
										if (PS[countY][countX - 1] != InversionVariable && PS[countY][countX - 1] != DecisionVariable)
										{
											PS[countY][countX - 1] = Chooseable;
											break;
										}
									}
									else if (PS[countY][Xcount] == null || PS[countY][Xcount] == Chooseable)
									{
										break;
									}
								}

								//←方向
								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									//黒だった場合
									if (PS[countY][Xcount] == DecisionVariable)
									{
										if (PS[countY][countX + 1] != InversionVariable && PS[countY][countX + 1] != DecisionVariable)
										{
											PS[countY][countX + 1] = Chooseable;
											break;
										}
									}
									else if (PS[countY][Xcount] == null || PS[countY][Xcount] == Chooseable)
									{
										break;
									}
								}
							}
							//↓方向
							if (countY != 0 && countY + 1 < MapSize)
							{
								for (Ycount = countY + 1; Ycount < MapSize; Ycount++)
								{
									//黒だった場合
									if (PS[Ycount][countX] == DecisionVariable)
									{
										if (PS[countY - 1][countX] != InversionVariable && PS[countY - 1][countX] != DecisionVariable)
										{
											PS[countY - 1][countX] = Chooseable;
											break;
										}
									}
									else if (PS[Ycount][countX] == null || PS[Ycount][countX] == Chooseable)
									{
										break;
									}
								}
								//↑方向
								for (Ycount = countY - 1; Ycount >= 0; Ycount--)
								{
									//黒だった場合
									if (PS[Ycount][countX] == DecisionVariable)
									{
										if (PS[countY + 1][countX] != InversionVariable && PS[countY + 1][countX] != DecisionVariable)
										{
											PS[countY + 1][countX] = Chooseable;
											break;

										}
									}
									else if (PS[Ycount][countX] == null || PS[Ycount][countX] == Chooseable)
									{
										break;
									}
								}
							}

							if (countX != 0 && countX + 1 < MapSize && countY != 0 && countY + 1 < MapSize)
							{
								//右上
								Ycount = countY - 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//黒だった場合
									if (PS[Ycount][Xcount] == DecisionVariable)
									{
										if (PS[countY + 1][countX - 1] != InversionVariable && PS[countY + 1][countX - 1] != DecisionVariable)
										{
											PS[countY + 1][countX - 1] = Chooseable;
											break;
										}
									}
									else if (PS[Ycount][Xcount] == null || PS[Ycount][Xcount] == Chooseable)
									{
										break;
									}

									Ycount--;
								}

								//右下
								Ycount = countY + 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//黒だった場合
									if (PS[Ycount][Xcount] == DecisionVariable)
									{
										if (PS[countY - 1][countX - 1] != InversionVariable && PS[countY - 1][countX - 1] != DecisionVariable)
										{
											PS[countY - 1][countX - 1] = Chooseable;
											break;
										}
									}
									else if (PS[Ycount][Xcount] == null || PS[Ycount][Xcount] == Chooseable)
									{
										break;
									}

									Ycount++;
								}

								//左上
								Ycount = countY - 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//黒だった場合
									if (PS[Ycount][Xcount] == DecisionVariable)
									{
										if (PS[countY + 1][countX + 1] != InversionVariable && PS[countY + 1][countX + 1] != DecisionVariable)
										{
											PS[countY + 1][countX + 1] = Chooseable;
											break;
										}
									}
									else if (PS[Ycount][Xcount] == null || PS[Ycount][Xcount] == Chooseable)
									{
										break;
									}

									Ycount--;

								}
								//左下
								Ycount = countY + 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//黒だった場合
									if (PS[Ycount][Xcount] == DecisionVariable)
									{
										if (PS[countY - 1][countX + 1] != InversionVariable && PS[countY - 1][countX + 1] != DecisionVariable)
										{
											PS[countY - 1][countX + 1] = Chooseable;
											break;
										}
									}
									else if (PS[Ycount][Xcount] == null || PS[Ycount][Xcount] == Chooseable)
									{
										break;
									}

									Ycount++;
								}
							}
						}
					}
				}

				//置ける場所がない時はパス
				//黒、白の駒がいくつあるか

				ChooseableCount = 0;
				blackCount = 0;
				whiteCount = 0;

				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						if (PS[countY][countX] == Chooseable)
						{
							ChooseableCount++;
						}
						else if (PS[countY][countX] == black)
						{
							blackCount++;
						}
						else if (PS[countY][countX] == white)
						{
							whiteCount++;
						}
					}
				}

				if (ChooseableCount == 0)
				{
					if (ReverseCount == true)
					{
						Mode = GameEndProcessing;
						break;
					}
					else
					{
						switch (WB)
						{
							//白の手の場合
						case Mode_white:

							//黒の順番に変更
							WB = Mode_black;

							//反転変数を白に
							InversionVariable = white;

							//判断変数を黒に
							DecisionVariable = black;
							break;

							//黒の手の場合
						case Mode_black:

							//白の順番に変更
							WB = Mode_white;

							//反転変数を黒に
							InversionVariable = black;

							//判断変数を白に
							DecisionVariable = white;
							break;
						}

						ReverseCount = true;
					}
				}
				else
				{
					ReverseCount = false;
					//プレイヤー操作モードに移動
					Mode = PlayerProcessing;
				}

				break;

			//プレイヤー操作モード
			case Game_MODE::PlayerProcessing:

				if (Mouseflag == false && pDi->MouseButton(0))
				{
					Mouseflag = true;

					Vector2<int> mousePos = pDi->MousePosition();

					MousePosX = mousePos.X() / PieceImageSize;
					MousePosY = mousePos.Y() / PieceImageSize;

					if (MousePosX < MapSize&&MousePosX < MapSize)
					{
						if (PS[MousePosY][MousePosX] == Chooseable)
						{
							switch (WB)
							{
							case Mode_white:
								PS[MousePosY][MousePosX] = white;
								Reverseflag = true;
								break;
							case Mode_black:
								PS[MousePosY][MousePosX] = black;
								Reverseflag = true;
								break;

							}
						}
					}
				}
				else if (Reverseflag == true && Mouseflag == true && !(pDi->MouseButton(0)))
				{
					Reverseflag = false;
					Mouseflag = false;
					//反転処理モードに移動
					Mode = InversionProcessing;
				}
				else if (Mouseflag == true && !(pDi->MouseButton(0)))
				{
					Mouseflag = false;
				}

				break;

				//反転処理モード
			case Game_MODE::InversionProcessing:
			
				switch (WB)
				{
				//白の手の場合
				case Mode_white:

					//黒の順番に変更
					WB = Mode_black;

					//反転変数を白に
					InversionVariable = white;

					//判断変数を黒に
					DecisionVariable = black;
					break;

				//黒の手の場合
				case Mode_black:

					//白の順番に変更
					WB = Mode_white;

					//反転変数を黒に
					InversionVariable = black;

					//判断変数を白に
					DecisionVariable = white;
					break;
				}

				//選択可能処理モードに移動
				Mode = SelectableProcessing;

				//→方向
				if (MousePosX + 1 < MapSize)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						//判断変数だった場合
						if (PS[MousePosY][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[MousePosY][Xcount] == InversionVariable&&Xcount != MousePosX + 1)
						{
							for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
							{
								//判断変数だった場合
								if (PS[MousePosY][Xcount] == DecisionVariable)
								{
									PS[MousePosY][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}
							}
							break;
						}
						else
						{
							break;
						}
					}
				}

				//←方向
				if (MousePosX > 0)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						//判断変数だった場合
						if (PS[MousePosY][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[MousePosY][Xcount] == InversionVariable&& Xcount != MousePosX - 1)
						{
							for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
							{
								//判断変数だった場合
								if (PS[MousePosY][Xcount] == DecisionVariable)
								{
									PS[MousePosY][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}
							}
							break;
						}
						else
						{
							break;
						}

					}
				}

				//↓方向
				if (MousePosY + 1 < MapSize)
				{
					for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
					{
						//判断変数だった場合
						if (PS[Ycount][MousePosX] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][MousePosX] == InversionVariable && Ycount != MousePosY + 1)
						{
							for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
							{
								//判断変数だった場合
								if (PS[Ycount][MousePosX] == DecisionVariable)
								{
									PS[Ycount][MousePosX] = InversionVariable;
								}
								else
								{
									break;
								}
							}
							break;
						}
						else
						{
							break;
						}
					}
				}

				//↑方向
				if (MousePosY > 0)
				{
					for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
					{
						//判断変数だった場合
						if (PS[Ycount][MousePosX] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][MousePosX] == InversionVariable&&Ycount != MousePosY - 1)
						{
							for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
							{
								//判断変数だった場合
								if (PS[Ycount][MousePosX] == DecisionVariable)
								{
									PS[Ycount][MousePosX] = InversionVariable;
								}
								else
								{
									break;
								}
							}
							break;
						}
						else
						{
							break;
						}
					}
				}

				//右上
				Ycount = MousePosY - 1;

				if (MousePosX + 1 < MapSize&&Ycount >= 0)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						if (Ycount <= 0)
						{
							break;
						}

						//判断変数だった場合
						if (PS[Ycount][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][Xcount] == InversionVariable&&Xcount != MousePosX + 1)
						{
							Ycount = MousePosY - 1;

							for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
							{
								if (Ycount <= 0)
								{
									break;
								}

								//判断変数だった場合
								if (PS[Ycount][Xcount] == DecisionVariable)
								{
									PS[Ycount][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}

								Ycount--;
							}
							break;
						}
						else
						{
							break;
						}

						Ycount--;
					}
				}

				//右下
				Ycount = MousePosY + 1;

				if (MousePosX + 1 < MapSize&&Ycount < MapSize)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						if (Ycount >= MapSize)
						{
							break;
						}

						//判断変数だった場合
						if (PS[Ycount][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][Xcount] == InversionVariable&&Xcount != MousePosX + 1)
						{
							Ycount = MousePosY + 1;

							for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
							{
								if (Ycount >= MapSize)
								{
									break;
								}

								//判断変数だった場合
								if (PS[Ycount][Xcount] == DecisionVariable)
								{
									PS[Ycount][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}

								Ycount++;
							}
							break;
						}
						else
						{
							break;
						}

						Ycount++;
					}
				}

				//左上
				Ycount = MousePosY - 1;

				if (MousePosX - 1 >= 0 && Ycount >= 0)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						if (Ycount <= 0)
						{
							break;
						}

						//判断変数だった場合
						if (PS[Ycount][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][Xcount] == InversionVariable&&Xcount != MousePosX - 1)
						{
							Ycount = MousePosY - 1;

							for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
							{
								if (Ycount <= 0)
								{
									break;
								}

								//判断変数だった場合
								if (PS[Ycount][Xcount] == DecisionVariable)
								{
									PS[Ycount][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}

								Ycount--;
							}
							break;
						}
						else
						{
							break;
						}

						Ycount--;
					}
				}

				//左下
				Ycount = MousePosY + 1;

				if (MousePosX - 1 >= 0 && Ycount < MapSize)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						if (Ycount >= MapSize)
						{
							break;
						}

						//判断変数だった場合
						if (PS[Ycount][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][Xcount] == InversionVariable&&Xcount != MousePosX - 1)
						{
							Ycount = MousePosY + 1;

							for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
							{
								if (Ycount >= MapSize)
								{
									break;
								}

								//判断変数だった場合
								if (PS[Ycount][Xcount] == DecisionVariable)
								{
									PS[Ycount][Xcount] = InversionVariable;
								}
								else
								{
									break;
								}
								
								Ycount++;
							}
							break;
						}
						else
						{
							break;
						}

						Ycount++;
					}
				}

				break;

			//ゲーム終了モード
			case Game_MODE::GameEndProcessing:

				if (Mouseflag == false && pDi->MouseButton(0))
				{
					Mouseflag = true;

				}
				else if (Mouseflag == true && !(pDi->MouseButton(0)))
				{
					Mouseflag = false;
					//反転処理モードに移動
					Mode = GameStartProcessing;
				}

				break;

			}

			//描画処理
			//バックバッファのクリア
			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();

				//盤の描画
				sprite_img_Board.Draw(img_Board);

				//駒、選択可能領域の描画
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						switch (PS[countY][countX])
						{
						case black:       //黒
							img_Piece[countY][countX].SetNum(7, 0);
							sprite_img_Piece[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize /2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Piece[countY][countX].Draw(img_Piece[countY][countX]);
							break;
						case white:       //白
							img_Piece[countY][countX].SetNum(0, 0);
							sprite_img_Piece[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Piece[countY][countX].Draw(img_Piece[countY][countX]);
							break;
						case Chooseable:  //選択可能
							sprite_img_Chooseable[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Chooseable[countY][countX].Draw(img_Chooseable[countY][countX]);
							break;
						case null:         //無

							break;
						}
					}
				}

				//スコアボードの画像を設定
				sprite_img_ScoreBoard.Draw(img_ScoreBoard);

				if (Mode != GameEndProcessing)
				{
					//現在の順番の画像
					switch (WB)
					{
					case Mode_white:
						sprite_img_cursor.SetPos(470.0, 200.0 + (float)PieceImageSize / 2.0 + 2.0);
						break;
					case Mode_black:
						sprite_img_cursor.SetPos(470.0, 100.0 + (float)PieceImageSize / 2.0 + 2.0);
						break;
					}
					sprite_img_cursor.Draw(img_cursor);
				}
				else
				{
					if (blackCount > whiteCount)
					{
						img_WLD[0].SetNum(0, 0);
						img_WLD[1].SetNum(1, 0);

					}
					else if(blackCount < whiteCount)
					{
						img_WLD[0].SetNum(1, 0);
						img_WLD[1].SetNum(0, 0);
					}
					else
					{
						img_WLD[0].SetNum(2, 0);
						img_WLD[1].SetNum(2, 0);
					}

					sprite_img_WLD[0].Draw(img_WLD[0]);
					sprite_img_WLD[1].Draw(img_WLD[1]);
				}

				sprite_img_PieceScoreW.Draw(img_PieceScoreW);
				sprite_img_PieceScoreB.Draw(img_PieceScoreB);

				//駒の数の画像設定
				//白
				if (whiteCount >= 10)
				{
					img_NumberW[0].SetNum(whiteCount / 10, 0);
					sprite_img_NumberW[0].Draw(img_NumberW[0]);
				}

				img_NumberW[1].SetNum(whiteCount % 10, 0);
				sprite_img_NumberW[1].Draw(img_NumberW[1]);

				//黒
				if (blackCount >= 10)
				{
					img_NumberB[0].SetNum(blackCount / 10, 0);
					sprite_img_NumberB[0].Draw(img_NumberB[0]);
				}

				img_NumberB[1].SetNum(blackCount % 10, 0);
				sprite_img_NumberB[1].Draw(img_NumberB[1]);

				//描画終了の合図
				d3d.EndScene();
				//バックバッファをフロントへ反映
				d3d.Present();
			}
		}
	}

	return 0;

}