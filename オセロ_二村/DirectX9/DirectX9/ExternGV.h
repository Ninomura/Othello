#pragma once //�C���N���[�h�K�[�h

#define Window_Size_X 640  //�E�B���h�E��X��
#define Window_Size_Y 480  //�E�B���h�E��Y��
#define BoardImageSize 372 //�Ֆʂ�\���摜�T�C�Y
#define MapSize 8          //�Ֆʂ̃T�C�Y��

//�摜�n���h��
extern int
img_Board,       //�Ֆʂ̉摜
img_Piece,       //��̉摜
img_Chooseable,  //�I���\�̉摜
img_StartScreen, //�X�^�[�g��ʂ̉摜
img_GameEnd;     //�I����ʂ̉摜

//�Q�[���̃��[�h
enum Game_MODE
{
	StartScreenProcessing, //�X�^�[�g��ʂ̐ݒ�
	GameStartProcessing,   //��������
	PlayerProcessing,      //�v���C���[���쏈��
	SelectableProcessing,  //�I���\����
	InversionProcessing,   //���]����
	GameEndProcessing,     //�ŏI����
};

extern Game_MODE Mode;

//���Ԃ̏��
enum Game_WB_MODE
{
	Mode_white,
	Mode_black
};

extern Game_WB_MODE WB;

//�Ֆʂ̏��
enum PieceState
{
	black,       //��
	white,       //��
	Chooseable,  //�I���\
	null         //��
};

extern PieceState PS[MapSize*MapSize];
