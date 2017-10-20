
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
	int width = 800,	//クライアント領域の幅
	int height = 600)	//クライアント領域の高さ
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
		_T("タイトル"),			//ウィンドウのタイトル
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
	bool Mouseflag = false, Reverseflag = false;
	bool Reverseflag8[8];

	for (int count = 0; count < 8; count++)
	{
		Reverseflag8[count] = false;
	}

	//マウス座標
	int MousePosX, MousePosY;

	PieceState PS[MapSize][MapSize];

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
			sprite_img_Chooseable[countY][countX].SetSize(PieceImageSize, PieceImageSize);
			sprite_img_Chooseable[countY][countX].SetAngle(0);

			img_Chooseable[countY][countX].Load(_T("Texture/Chooseable.png"));
			img_Chooseable[countY][countX].SetDivide(0, 0);
			img_Chooseable[countY][countX].SetNum(0, 0);
		}
	}

	Game_MODE Mode;
	Mode = GameStartProcessing; //初期モードを演算処理モードに設定

	Game_WB_MODE WB;
	WB = Mode_black;

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

			if (pDi->KeyJustPressed(DIK_A))
			{
				MessageBox(NULL,
					TEXT("キー入力確認"),
					TEXT("テスト-タイトル"),
					MB_OK);
			}

			switch (Mode)
			{

				//スタート画面
			case Game_MODE::StartScreenProcessing:

				break;

				//初期処理
			case Game_MODE::GameStartProcessing:

				//盤面のリセット
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						PS[countY][countX] = null;
					}
				}

				//初期の盤面
				PS[3][3] = white, PS[4][4] = white;
				PS[3][4] = black, PS[4][3] = black;

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

				//反転処理用（8方向）フラグのリセット
				for (int count = 0; count < 8; count++)
				{
					Reverseflag8[count] = false;
				}

				//選択可能処理
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						//駒が白である場合。（順番が黒の場合）
						if (WB == Mode_black && PS[countY][countX] == white)
						{
							//→方向
							if (countX != 0 && countX + 1 < MapSize)
							{
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//黒だった場合
									if (PS[countY][Xcount] == black)
									{
										if (PS[countY][countX - 1] == null)
										{
											PS[countY][countX - 1] = Chooseable;
											Reverseflag8[0] = true;
											break;
										}
									}
									else if (PS[countY][Xcount] == null|| PS[countY][Xcount] == Chooseable)
									{
										break;
									}
								}

								//←方向
								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									//黒だった場合
									if (PS[countY][Xcount] == black)
									{
										if (PS[countY][countX + 1] != white && PS[countY][countX + 1] != black)
										{
											PS[countY][countX + 1] = Chooseable;
											Reverseflag8[1] = true;
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
									if (PS[Ycount][countX] == black)
									{
										if (PS[countY - 1][countX] != white && PS[countY - 1][countX] != black)
										{
											PS[countY - 1][countX] = Chooseable;
											Reverseflag8[2] = true;
											break;
										}
									}
									else if (PS[Ycount][countX] == null)
									{
										break;
									}
								}
								//↑方向
								for (Ycount = countY - 1; Ycount >= 0; Ycount--)
								{
									//黒だった場合
									if (PS[Ycount][countX] == black)
									{
										if (PS[countY + 1][countX] != white && PS[countY + 1][countX] != black)
										{
											PS[countY + 1][countX] = Chooseable;
											Reverseflag8[3] = true;
											break;

										}
									}
									else if (PS[Ycount][countX] == null || PS[Ycount][countX] == Chooseable)
									{
										break;
									}
								}
							}

							if (countX != 0 && countX + 1 < MapSize || countY != 0 && countY + 1 < MapSize)
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
									if (PS[Ycount][Xcount] == black)
									{
										if (PS[countY + 1][countX - 1] != white && PS[countY + 1][countX - 1] != black)
										{
											PS[countY + 1][countX - 1] = Chooseable;
											Reverseflag8[4] = true;
											break;
										}
									}
									else if (PS[Ycount][Xcount] == null || PS[Ycount][countX] == Chooseable)
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
									if (PS[Ycount][Xcount] == black)
									{
										if (PS[countY - 1][countX - 1] != white && PS[countY - 1][countX - 1] != black)
										{
											PS[countY - 1][countX - 1] = Chooseable;
											Reverseflag8[5] = true;
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
									if (PS[Ycount][Xcount] == black)
									{
										if (PS[countY + 1][countX + 1] != white && PS[countY + 1][countX + 1] != black)
										{
											PS[countY + 1][countX + 1] = Chooseable;
											Reverseflag8[6] = true;
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
									if (PS[Ycount][Xcount] == black)
									{
										if (PS[countY - 1][countX + 1] != white && PS[countY - 1][countX + 1] != black)
										{
											PS[countY - 1][countX + 1] = Chooseable;
											Reverseflag8[7] = true;
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
						//駒が黒であって、外側でない場合。（順番が白の場合）
						else if (WB == Mode_white&&PS[countY][countX] == black)
						{

							if (countX != 0 && countX + 1 < MapSize)
							{
								//→方向
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//白だった場合
									if (PS[countY][Xcount] == white)
									{
										if (PS[countY][countX - 1] != white && PS[countY][countX - 1] != black)
										{
											PS[countY][countX - 1] = Chooseable;
											Reverseflag8[0] = true;
											break;
										}
										else
										{
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
									//白だった場合
									if (PS[countY][Xcount] == white)
									{
										if (PS[countY][countX + 1] != white && PS[countY][countX + 1] != black)
										{
											PS[countY][countX + 1] = Chooseable;
											Reverseflag8[1] = true;
											break;
										}
										else
										{
											break;
										}
									}
									else if (PS[countY][Xcount] == null || PS[countY][Xcount] == Chooseable)
									{
										break;
									}
								}
							}

							if (countY != 0 && countY + 1 < MapSize)
							{
								//↓方向
								for (Ycount = countY + 1; Ycount < MapSize; Ycount++)
								{
									//白だった場合
									if (PS[Ycount][countX] == white)
									{
										if (PS[countY - 1][countX] != white && PS[countY - 1][countX] != black)
										{
											PS[countY - 1][countX] = Chooseable;
											Reverseflag8[2] = true;
											break;
										}
										else
										{
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
									//白だった場合
									if (PS[Ycount][countX] == white)
									{
										if (PS[countY + 1][countX] != white && PS[countY + 1][countX] != black)
										{
											PS[countY + 1][countX] = Chooseable;
											Reverseflag8[3] = true;
											break;
										}
										else
										{
											break;
										}
									}
									else if (PS[Ycount][countX] == null || PS[Ycount][countX] == Chooseable)
									{
										break;
									}
								}
							}


							if (countX != 0 && countX + 1 < MapSize || countY != 0 && countY + 1 < MapSize)
							{
								//右上
								Ycount = countY - 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//白だった場合
									if (PS[Ycount][Xcount] == white)
									{
										if (PS[countY + 1][countX - 1] != white && PS[countY + 1][countX - 1] != black)
										{
											PS[countY + 1][countX - 1] = Chooseable;
											Reverseflag8[4] = true;
											break;
										}
										else
										{
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

									//白だった場合
									if (PS[Ycount][Xcount] == white)
									{
										if (PS[countY - 1][countX - 1] != white && PS[countY - 1][countX - 1] != black)
										{
											PS[countY - 1][countX - 1] = Chooseable;
											Reverseflag8[5] = true;
											break;
										}
										else
										{
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


									//白だった場合
									if (PS[Ycount][Xcount] == white)
									{
										if (PS[countY + 1][countX + 1] != white && PS[countY + 1][countX + 1] != black)
										{
											PS[countY + 1][countX + 1] = Chooseable;
											Reverseflag8[6] = true;
											break;
										}
										else
										{
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

									//白だった場合
									if (PS[Ycount][Xcount] == white)
									{
										if (PS[countY - 1][countX + 1] != white && PS[countY - 1][countX + 1] != black)
										{
											PS[countY - 1][countX + 1] = Chooseable;
											Reverseflag8[7] = true;
											break;
										}
										else
										{
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

				//プレイヤー操作モードに移動
				Mode = PlayerProcessing;

				break;

				//プレイヤー操作モード
			case Game_MODE::PlayerProcessing:

				if (Mouseflag == false&&pDi->MouseButton(0))
				{
					Mouseflag = true;

					Vector2<int> mousePos = pDi->MousePosition();

					MousePosX = mousePos.X() / PieceImageSize;
					MousePosY = mousePos.Y() / PieceImageSize;

					for (int countY = 0; countY < MapSize; countY++)
					{
						for (int countX = 0; countX < MapSize; countX++)
						{
							if (MousePosX == countX && MousePosY == countY&&PS[countY][countX] == Chooseable)
							{
								switch (WB)
								{
								case Mode_white:
									PS[countY][countX] = white;
									Reverseflag = true;
									break;
								case Mode_black:
									PS[countY][countX] = black;
									Reverseflag = true;
									break;

								}
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
				case Mode_white:

					//→方向
					if (Reverseflag8[0] == true)
					{
						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							//黒だった場合
							if (PS[MousePosY][Xcount] == black)
							{
								PS[MousePosY][Xcount] = white;
							}
							else
							{
								break;
							}
						}
					}

					//←方向
					if (Reverseflag8[1] == true)
					{
						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							//黒だった場合
							if (PS[MousePosY][Xcount] == black)
							{
								PS[MousePosY][Xcount] = white;
							}
							else
							{
								break;
							}
						}
					}

					//↓方向
					if (Reverseflag8[2] == true)
					{
						for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
						{
							//黒だった場合

							if (PS[Ycount][MousePosX] == black)
							{
								PS[Ycount][MousePosX] = white;
							}
							else
							{
								break;
							}
						}
					}

					//↑方向
					if (Reverseflag8[3] == true)
					{
						for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
						{
							//黒だった場合
							if (PS[Ycount][MousePosX] == black)
							{
								PS[Ycount][MousePosX] = white;
							}
							else
							{
								break;
							}
						}
					}

					//右上
					if (Reverseflag8[4] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//黒だった場合
							if (PS[Ycount][Xcount] == black)
							{
								PS[Ycount][Xcount] = white;
							}
							else
							{
								break;
							}

							Ycount--;
						}
					}

					//右下
					if (Reverseflag8[5] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//黒だった場合
							if (PS[Ycount][Xcount] == black)
							{
								PS[Ycount][Xcount] = white;
							}
							else
							{
								break;
							}

							Ycount++;
						}
					}

					//左上
					if (Reverseflag8[6] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//黒だった場合
							if (PS[Ycount][Xcount] == black)
							{
								PS[Ycount][Xcount] = white;
							}
							else
							{
								break;
							}

							Ycount--;

						}
					}

					//左下
					if (Reverseflag8[7] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//黒だった場合
							if (PS[Ycount][Xcount] == black)
							{
								PS[Ycount][Xcount] = white;
							}
							else
							{
								break;
							}

							Ycount++;
						}
					}

					//黒の順番に変更
					WB = Mode_black;
					//選択可能処理モードに移動
					Mode = SelectableProcessing;
					break;
				case Mode_black:

					//→方向
					if (Reverseflag8[0] == true)
					{
						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							//白だった場合
							if (PS[MousePosY][Xcount] == white)
							{
								PS[MousePosY][Xcount] = black;
							}
							else
							{
								break;
							}
						}
					}

					//←方向
					if (Reverseflag8[1] == true)
					{
						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							//白だった場合
							if (PS[MousePosY][Xcount] == white)
							{
								PS[MousePosY][Xcount] = black;
							}
							else
							{
								break;
							}
						}
					}

					//↓方向
					if (Reverseflag8[2] == true)
					{
						for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
						{
							//白だった場合

							if (PS[Ycount][MousePosX] == white)
							{
								PS[Ycount][MousePosX] = black;
							}
							else
							{
								break;
							}
						}
					}

					//↑方向
					if (Reverseflag8[3] == true)
					{
						for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
						{
							//白だった場合
							if (PS[Ycount][MousePosX] == white)
							{
								PS[Ycount][MousePosX] = black;
							}
							else
							{
								break;
							}
						}
					}

					//右上
					if (Reverseflag8[4] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//白だった場合
							if (PS[Ycount][Xcount] == white)
							{
								PS[Ycount][Xcount] = black;
							}
							else
							{
								break;
							}

							Ycount--;
						}
					}

					//右下
					if (Reverseflag8[5] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//白だった場合
							if (PS[Ycount][Xcount] == white)
							{
								PS[Ycount][Xcount] = black;
							}
							else
							{
								break;
							}

							Ycount++;
						}
					}

					//左上
					if (Reverseflag8[6] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//白だった場合
							if (PS[Ycount][Xcount] == white)
							{
								PS[Ycount][Xcount] = black;
							}
							else
							{
								break;
							}

							Ycount--;

						}
					}

					//左下
					if (Reverseflag8[7] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//白だった場合
							if (PS[Ycount][Xcount] == white)
							{
								PS[Ycount][Xcount] = black;
							}
							else
							{
								break;
							}

							Ycount++;
						}
					}

					//白の順番に変更
					WB = Mode_white;
					//選択可能処理モードに移動
					Mode = SelectableProcessing;
					break;
				}

				break;

				//ゲーム終了モード
			case Game_MODE::GameEndProcessing:

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

				//描画終了の合図
				d3d.EndScene();
				//バックバッファをフロントへ反映
				d3d.Present();
			}
		}
	}

	return 0;

}