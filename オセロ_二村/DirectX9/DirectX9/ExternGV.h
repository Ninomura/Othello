#pragma once //インクルードガード

#define Window_Size_X 640  //ウィンドウのX幅
#define Window_Size_Y 480  //ウィンドウのY幅
#define BoardImageSize 372 //盤面を表す画像サイズ
#define MapSize 8          //盤面のサイズ比

//画像ハンドル
extern int
img_Board,       //盤面の画像
img_Piece,       //駒の画像
img_Chooseable,  //選択可能の画像
img_StartScreen, //スタート画面の画像
img_GameEnd;     //終了画面の画像

//ゲームのモード
enum Game_MODE
{
	StartScreenProcessing, //スタート画面の設定
	GameStartProcessing,   //初期処理
	PlayerProcessing,      //プレイヤー操作処理
	SelectableProcessing,  //選択可能処理
	InversionProcessing,   //反転処理
	GameEndProcessing,     //最終処理
};

extern Game_MODE Mode;

//順番の状態
enum Game_WB_MODE
{
	Mode_white,
	Mode_black
};

extern Game_WB_MODE WB;

//盤面の状態
enum PieceState
{
	black,       //黒
	white,       //白
	Chooseable,  //選択可能
	null         //無
};

extern PieceState PS[MapSize*MapSize];
