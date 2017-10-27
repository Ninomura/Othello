
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>


#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include "ExternGV.h"

#include <random>


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//�E�B���h�E�̃n���h��
	UINT msg,	//���b�Z�[�W
	WPARAM wParam,
	LPARAM lParam
)
{
	//���b�Z�[�W�����̂��߂̊֐�
	//DispatchMessage�ɂ���ČĂяo�����

	//�E�B���h�E�N���X�̓o�^����
	//�e�E�B���h�E�ɐݒ肳���

	//�A�v���P�[�V���������b�Z�[�W��
	//�擾������Ăяo�����

	//���b�Z�[�W�̗�

	//WM_DESTROY �E�B���h�E���j�����ꂽ
	//			 �~�{�^���������ꂽ���Ȃ�
	//WM_MOVE    �E�B���h�E���ړ�
	//WM_SIZE    �E�B���h�E�T�C�Y�̕ύX
	//��

	//���ʂȏ�����K�v�Ƃ���ꍇ
	//�܂��͎����œƎ��̏������`�������ꍇ
	//�����Ń��b�Z�[�W�̎�ނɉ�����
	//�������s��

	//��肠���� WM_DESTROY����������
	//�I�������̂ݒ�`

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		//���b�Z�[�W�L���[��
		//�V����WM_QUIT���b�Z�[�W�𑗂�

		//�����ꃁ�b�Z�[�W�L���[����
		//���o����
		//���C�����[�v���I������
		//(���C�����[�v�̏I��������
		//���̂悤�ɍ��)
		break;
	}

	//�Ǝ��̏������s������
	//�f�t�H���g�̃E�B���h�E�v���V�[�W�������s

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//�E�B���h�E�ݒ�̖��O
static const TCHAR* WC_BASIC = _T("Basic");

//��肽���E�B���h�E�̐ݒ��
//�o�^����֐�
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//�E�B���h�E�̐ݒ��ۑ�����\����
	WNDCLASSEX wcex;

	//wcex�̐擪�A�h���X����
	//WNDCLASSEX�̒������̃�������
	//�S��0�Ŗ��߂�
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//�܂��wcex���g�p���Ă��郁�����̈��
	//�S��0�ŏ��������鏈��

	//�E�B���h�E�̐ݒ�

	//�\���̂̃T�C�Y
	wcex.cbSize = sizeof(WNDCLASSEX);

	//�E�B���h�E�̐ݒ���ɒǉ�����ߑ���t���Ȃ��̂�
	//0�ŌŒ�
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Window�̃X�^�C��
	//���ꂼ�ꂪ�r�b�g�P�ʂ̃t���O�ł��邽��
	//�I�A���Z�ō����������̂��X�^�C���̍ŏI�I�Ȑݒ�
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW ���T�C�Y���ς�����Ƃ��E�B���h�E�S�̂̍ĕ`��
	//   VREDRAW �c�T�C�Y���ς�����Ƃ��E�B���h�E�S�̂̍ĕ`��
	//   DBLCLKS �_�u���N���b�N�������������Ƃ��E�B���h�E�ɒʒm

	//WinMain�̃C���X�^���X�n���h��
	wcex.hInstance = hInstance;

	//�E�B���h�E�v���V�[�W���̃A�h���X
	//�֐��|�C���^�̐ݒ�
	//�E�B���h�E�v���V�[�W����OS���Ăяo����悤�ɂ���
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//�w�i�F
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//���j���[�@�Ȃ�
	wcex.lpszMenuName = NULL;

	//�E�B���h�E�N���X�̖��O�@���̃E�B���h�E�ݒ�̖��O
	wcex.lpszClassName = WC_BASIC;

	//�A�C�R���@�f�t�H���g�̂��g��
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//�J�[�\�� �f�t�H���g�̂��̂��g��
	wcex.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//�ݒ肵���E�B���h�E�N���X��o�^
	//�o�^�����E�B���h�E�ݒ�łȂ���
	//�E�B���h�E����邱�Ƃ��ł��Ȃ�
	return RegisterClassEx(&wcex);
	//HRESULT�^�̎��s���ʂ��߂����
}


//�o�^�����ݒ����ɃE�B���h�E���쐬����
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMain�̎��ʎq
	HWND &refHWnd,	//�E�B���h�E�̎��ʎq
					//�������E�B���h�E�̍쐬���ł�����
					//���̕ϐ��Ɏ��ʎq��������
	int width = 675,	//�N���C�A���g�̈�̕�
	int height = 375)	//�N���C�A���g�̈�̍���
						//�N���C�A���g�̈�̓E�B���h�E�S�̂���
						//�O�g�⃁�j���[�̕��������������ƍ��͎v���Ă�����OK
{
	//�N���C�A���g�̈�̂������������߂�
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//�N���C�A���g�̈��ێ�����̂ɕK�v��
	//�E�B���h�E�̂����������v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//���������s������rect�̒l���X�V�����

	//WS_OVERLAPPEDWINDOW�̓E�B���h�E�̃X�^�C���̈��

	//�E�B���h�E�̃n���h��
	//�E�B���h�E�̎��ʂɎg�p����
	HWND hWnd;

	hWnd = CreateWindowEx
	(0,						//�E�B���h�E�g���X�^�C��
		WC_BASIC,				//��肽���E�B���h�E�N���X
								//���炩���ߓo�^���ꂽ����
		_T("REVERSE"),			//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,			//�ʒux���W �f�t�H���g�̒l
		CW_USEDEFAULT,			//�ʒuy���W �f�t�H���g�̒l
		rect.right - rect.left,	//�E�B���h�E�S�̂̕�
		rect.bottom - rect.top, //�E�B���h�E�S�̂̍���
		NULL,					//�e�E�B���h�E�̃n���h��
		NULL,					//���j���[�̃n���h��
		hInstance,				//WinMain�̃n���h��

		NULL);					//�E�B���h�E�쐬�f�[�^
								//�E�B���h�E�ɍD����32�r�b�g�̃f�[�^��
								//�t�������邱�Ƃ��ł���
								//�ǉ����\���̂ւ̃|�C���^�Ȃ�
								//��t����̂�����Ȏg������
								//����͎g��Ȃ�

								//�E�B���h�E����������ꂽ�����`�F�b�N
								//��������ꂽ��hWnd��NULL�ȊO������͂�
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT�^�@���s��\�����ʎq�̈��
	}

	//�E�B���h�E���A�N�e�B�u�ɂ��ĕ\��
	//hWnd�ŃE�B���h�E�̎��ʂ����Ă��邱�Ƃɒ���
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//�N���C�A���g�̈�̍X�V
	UpdateWindow(hWnd);

	//�Q�Ɠn���ɂȂ��Ă��������
	//�E�B���h�E�̎��ʃn���h�����R�s�[����
	//����ɂ���č�������E�B���h�E�̎��ʃn���h����
	//�Ăяo�����̃v���O�����œ��邱�Ƃ��ł���
	refHWnd = hWnd;

	//HRESULT�^�@������\�����ʎq�̈��
	return S_OK;
}


//�G���g���[�|�C���g
//�v���O�����̊J�n�֐�

int _stdcall WinMain
(HINSTANCE hInstance,	//�v���O���������ʂ���n���h��
	HINSTANCE hPrevInstance,//�A�v���P�[�V�����̑O�̃C���X�^���X�n���h��
							//win32���Ə��NULL
	LPSTR lpCmdLine,		//�R�}���h���C������
	int nCmdShow)		//�E�B���h�E�̕\�����
{
	//���b�Z�[�W�{�b�N�X
	MessageBox(NULL,		//�E�B���h�E�̃n���h�� 
		TEXT("�e�X�g"),		//�{��
		TEXT("�e�X�g-�^�C�g��"),//�^�C�g��
		MB_OK);				//���b�Z�[�W�{�b�N�X�̃^�C�v
							//MB_OK  ok�̃{�^�����\��

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("�E�B���h�E�N���X�ݒ莸�s"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	//�������E�B���h�E���쐬������
	//hWnd�ɃE�B���h�E�̎��ʃn���h��������
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("�E�B���h�E�쐬���s"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
		return 0;
	}

	//Direct3D���Ǘ�����N���X(�V���O���g��)�ւ̎Q�Ƃ��擾
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDevice�̍쐬�����݂�
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device�쐬����"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
	}

	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHATEST);

	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	int Xcount, Ycount;
	bool Mouseflag, Reverseflag;
	bool ReverseCount;

	//�}�E�X���W
	int MousePosX, MousePosY,

	//��̐�
		blackCount, whiteCount, ChooseableCount;


	//�Ֆʂ̏��
	PieceState PS[MapSize][MapSize];

	PieceState
	InversionVariable, //���]�ϐ�
	DecisionVariable;  //���f�ϐ�

	//���\���摜�T�C�Y
	int PieceImageSize = BoardImageSize / MapSize;

	//�Ֆʂ̉摜��ݒ�
	Sprite sprite_img_Board;
	Texture img_Board;

	sprite_img_Board.SetAlpha(1);
	sprite_img_Board.SetSize(BoardImageSize, BoardImageSize);
	sprite_img_Board.SetAngle(0);

	img_Board.Load(_T("Texture/Board.png"));
	img_Board.SetDivide(0, 0);
	img_Board.SetNum(0, 0);

	sprite_img_Board.SetPos(BoardImageSize / 2, BoardImageSize / 2);

	//��̉摜��ݒ�
	Sprite sprite_img_Piece[MapSize][MapSize];
	Texture img_Piece[MapSize][MapSize];

	//�I���\�̈�̉摜
	Sprite sprite_img_Chooseable[MapSize][MapSize];
	Texture img_Chooseable[MapSize][MapSize];

	for (int countY = 0; countY < MapSize; countY++)
	{
		for (int countX = 0; countX < MapSize; countX++)
		{
			//��̉摜��ݒ�
			sprite_img_Piece[countY][countX].SetAlpha(1);
			sprite_img_Piece[countY][countX].SetSize(PieceImageSize, PieceImageSize);
			sprite_img_Piece[countY][countX].SetAngle(0);

			img_Piece[countY][countX].Load(_T("Texture/Piece.png"));
			img_Piece[countY][countX].SetDivide(8, 0);

			//�I���\�̈�̉摜
			sprite_img_Chooseable[countY][countX].SetAlpha(1);
			sprite_img_Chooseable[countY][countX].SetSize(PieceImageSize+14, PieceImageSize+14);
			sprite_img_Chooseable[countY][countX].SetAngle(0);

			img_Chooseable[countY][countX].Load(_T("Texture/Chooseable.png"));
			img_Chooseable[countY][countX].SetDivide(0, 0);
			img_Chooseable[countY][countX].SetNum(0, 0);
		}
	}

	//�J�[�\���̉摜��ݒ�
	Sprite sprite_img_cursor;
	Texture img_cursor;
	sprite_img_cursor.SetAlpha(1);
	sprite_img_cursor.SetSize(50.0, 50.0);
	sprite_img_cursor.SetAngle(0);

	img_cursor.Load(_T("Texture/Cursor.png"));
	img_cursor.SetDivide(0, 0);
	img_cursor.SetNum(0, 0);

	//�X�R�A�{�[�h�̉摜��ݒ�
	Sprite sprite_img_ScoreBoard;
	Texture img_ScoreBoard;
	sprite_img_ScoreBoard.SetAlpha(1);
	sprite_img_ScoreBoard.SetSize(250.0, 375.0);
	sprite_img_ScoreBoard.SetAngle(0);
	sprite_img_ScoreBoard.SetPos(550.0, 190.0);

	img_ScoreBoard.Load(_T("Texture/ScoreBoard.png"));
	img_ScoreBoard.SetDivide(0, 0);
	img_ScoreBoard.SetNum(0, 0);

	//�X�R�A�̉摜��ݒ�
	//��
	Sprite sprite_img_PieceScoreW;
	Texture img_PieceScoreW;
	sprite_img_PieceScoreW.SetAlpha(1);
	sprite_img_PieceScoreW.SetSize(PieceImageSize, PieceImageSize);
	sprite_img_PieceScoreW.SetAngle(0);
	sprite_img_PieceScoreW.SetPos(510.0, 200.0 + (float)PieceImageSize / 2.0 + 2.0);

	img_PieceScoreW.Load(_T("Texture/Piece.png"));
	img_PieceScoreW.SetDivide(8, 0);
	img_PieceScoreW.SetNum(0, 0);

	//��
	Sprite sprite_img_PieceScoreB;
	Texture img_PieceScoreB;
	sprite_img_PieceScoreB.SetAlpha(1);
	sprite_img_PieceScoreB.SetSize(PieceImageSize, PieceImageSize);
	sprite_img_PieceScoreB.SetAngle(0);
	sprite_img_PieceScoreB.SetPos(510.0, 100.0 + (float)PieceImageSize / 2.0 + 2.0);

	img_PieceScoreB.Load(_T("Texture/Piece.png"));
	img_PieceScoreB.SetDivide(8, 0);
	img_PieceScoreB.SetNum(7, 0);

	//�����A�����A���������̉摜�ݒ�
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
	
	//��̐��̉摜�ݒ�
	//��
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
	Mode = GameStartProcessing; //�������[�h�����Z�������[�h�ɐݒ�

	Game_WB_MODE WB;

	MSG msg = {};

	//���C���v���O����-------------------------------------------------
	//quit���b�Z�[�W���o�Ă���܂Ń��[�v���J��Ԃ�
	//quit���b�Z�[�W�͏�L�̃E�B���h�E�v���V�[�W�����瑗�M
	//���M�̏����Ȃǂ̓E�B���h�E�v���V�[�W�����m�F
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ���
			//��������b�Z�[�W�L���[�Ƀ|�X�g����
			TranslateMessage(&msg);

			//���b�Z�[�W�̊��蓖��
			//�E�B���h�E�v���V�[�W���Ń��b�Z�[�W������
			DispatchMessage(&msg);
		}
		else
		{
			pDi->Update();//�L�[��Ԃ̍X�V

			switch (Mode)
			{

			//�X�^�[�g���
			case Game_MODE::StartScreenProcessing:

				break;

			//��������
			case Game_MODE::GameStartProcessing:

				//���̏��ԂɎw��
				WB = Mode_black;

				//���]�ϐ��𔒂�
				InversionVariable = white;

				//���f�ϐ�������
				DecisionVariable = black;

				blackCount = 0;
				whiteCount = 0;
				ChooseableCount = 0;
				ReverseCount = false;

				Mouseflag = false;
				Reverseflag = false;

				//�Ֆʂ̃��Z�b�g
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						PS[countY][countX] = null;
					}
				}

				//�����̔Ֆ�
				PS[3][3] = white; PS[4][4] = white;
				PS[3][4] = black; PS[4][3] = black;

				//�I���\�������[�h�Ɉړ�
				Mode = SelectableProcessing;

				break;

				//�I���\�������[�h
			case Game_MODE::SelectableProcessing:

				//�I���\�����̃��Z�b�g
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

				//�I���\����
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						//����ł���ꍇ�B�i���Ԃ����̏ꍇ�j
						if (PS[countY][countX] == InversionVariable)
						{
							//������
							if (countX != 0 && countX + 1 < MapSize)
							{
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//���������ꍇ
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

								//������
								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									//���������ꍇ
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
							//������
							if (countY != 0 && countY + 1 < MapSize)
							{
								for (Ycount = countY + 1; Ycount < MapSize; Ycount++)
								{
									//���������ꍇ
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
								//������
								for (Ycount = countY - 1; Ycount >= 0; Ycount--)
								{
									//���������ꍇ
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
								//�E��
								Ycount = countY - 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//���������ꍇ
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

								//�E��
								Ycount = countY + 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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

								//����
								Ycount = countY - 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//���������ꍇ
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
								//����
								Ycount = countY + 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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

				//�u����ꏊ���Ȃ����̓p�X
				//���A���̋�������邩

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
							//���̎�̏ꍇ
						case Mode_white:

							//���̏��ԂɕύX
							WB = Mode_black;

							//���]�ϐ��𔒂�
							InversionVariable = white;

							//���f�ϐ�������
							DecisionVariable = black;
							break;

							//���̎�̏ꍇ
						case Mode_black:

							//���̏��ԂɕύX
							WB = Mode_white;

							//���]�ϐ�������
							InversionVariable = black;

							//���f�ϐ��𔒂�
							DecisionVariable = white;
							break;
						}

						ReverseCount = true;
					}
				}
				else
				{
					ReverseCount = false;
					//�v���C���[���샂�[�h�Ɉړ�
					Mode = PlayerProcessing;
				}

				break;

			//�v���C���[���샂�[�h
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
					//���]�������[�h�Ɉړ�
					Mode = InversionProcessing;
				}
				else if (Mouseflag == true && !(pDi->MouseButton(0)))
				{
					Mouseflag = false;
				}

				break;

				//���]�������[�h
			case Game_MODE::InversionProcessing:
			
				switch (WB)
				{
				//���̎�̏ꍇ
				case Mode_white:

					//���̏��ԂɕύX
					WB = Mode_black;

					//���]�ϐ��𔒂�
					InversionVariable = white;

					//���f�ϐ�������
					DecisionVariable = black;
					break;

				//���̎�̏ꍇ
				case Mode_black:

					//���̏��ԂɕύX
					WB = Mode_white;

					//���]�ϐ�������
					InversionVariable = black;

					//���f�ϐ��𔒂�
					DecisionVariable = white;
					break;
				}

				//�I���\�������[�h�Ɉړ�
				Mode = SelectableProcessing;

				//������
				if (MousePosX + 1 < MapSize)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						//���f�ϐ��������ꍇ
						if (PS[MousePosY][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[MousePosY][Xcount] == InversionVariable&&Xcount != MousePosX + 1)
						{
							for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
							{
								//���f�ϐ��������ꍇ
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

				//������
				if (MousePosX > 0)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						//���f�ϐ��������ꍇ
						if (PS[MousePosY][Xcount] == DecisionVariable)
						{
							;
						}
						else if (PS[MousePosY][Xcount] == InversionVariable&& Xcount != MousePosX - 1)
						{
							for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
							{
								//���f�ϐ��������ꍇ
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

				//������
				if (MousePosY + 1 < MapSize)
				{
					for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
					{
						//���f�ϐ��������ꍇ
						if (PS[Ycount][MousePosX] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][MousePosX] == InversionVariable && Ycount != MousePosY + 1)
						{
							for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
							{
								//���f�ϐ��������ꍇ
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

				//������
				if (MousePosY > 0)
				{
					for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
					{
						//���f�ϐ��������ꍇ
						if (PS[Ycount][MousePosX] == DecisionVariable)
						{
							;
						}
						else if (PS[Ycount][MousePosX] == InversionVariable&&Ycount != MousePosY - 1)
						{
							for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
							{
								//���f�ϐ��������ꍇ
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

				//�E��
				Ycount = MousePosY - 1;

				if (MousePosX + 1 < MapSize&&Ycount >= 0)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						if (Ycount <= 0)
						{
							break;
						}

						//���f�ϐ��������ꍇ
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

								//���f�ϐ��������ꍇ
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

				//�E��
				Ycount = MousePosY + 1;

				if (MousePosX + 1 < MapSize&&Ycount < MapSize)
				{
					for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
					{
						if (Ycount >= MapSize)
						{
							break;
						}

						//���f�ϐ��������ꍇ
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

								//���f�ϐ��������ꍇ
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

				//����
				Ycount = MousePosY - 1;

				if (MousePosX - 1 >= 0 && Ycount >= 0)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						if (Ycount <= 0)
						{
							break;
						}

						//���f�ϐ��������ꍇ
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

								//���f�ϐ��������ꍇ
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

				//����
				Ycount = MousePosY + 1;

				if (MousePosX - 1 >= 0 && Ycount < MapSize)
				{
					for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
					{
						if (Ycount >= MapSize)
						{
							break;
						}

						//���f�ϐ��������ꍇ
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

								//���f�ϐ��������ꍇ
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

			//�Q�[���I�����[�h
			case Game_MODE::GameEndProcessing:

				if (Mouseflag == false && pDi->MouseButton(0))
				{
					Mouseflag = true;

				}
				else if (Mouseflag == true && !(pDi->MouseButton(0)))
				{
					Mouseflag = false;
					//���]�������[�h�Ɉړ�
					Mode = GameStartProcessing;
				}

				break;

			}

			//�`�揈��
			//�o�b�N�o�b�t�@�̃N���A
			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();

				//�Ղ̕`��
				sprite_img_Board.Draw(img_Board);

				//��A�I���\�̈�̕`��
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						switch (PS[countY][countX])
						{
						case black:       //��
							img_Piece[countY][countX].SetNum(7, 0);
							sprite_img_Piece[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize /2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Piece[countY][countX].Draw(img_Piece[countY][countX]);
							break;
						case white:       //��
							img_Piece[countY][countX].SetNum(0, 0);
							sprite_img_Piece[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Piece[countY][countX].Draw(img_Piece[countY][countX]);
							break;
						case Chooseable:  //�I���\
							sprite_img_Chooseable[countY][countX].SetPos
							((float)countX*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0,
								(float)countY*(float)PieceImageSize + (float)PieceImageSize / 2.0 + 2.0);
							sprite_img_Chooseable[countY][countX].Draw(img_Chooseable[countY][countX]);
							break;
						case null:         //��

							break;
						}
					}
				}

				//�X�R�A�{�[�h�̉摜��ݒ�
				sprite_img_ScoreBoard.Draw(img_ScoreBoard);

				if (Mode != GameEndProcessing)
				{
					//���݂̏��Ԃ̉摜
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

				//��̐��̉摜�ݒ�
				//��
				if (whiteCount >= 10)
				{
					img_NumberW[0].SetNum(whiteCount / 10, 0);
					sprite_img_NumberW[0].Draw(img_NumberW[0]);
				}

				img_NumberW[1].SetNum(whiteCount % 10, 0);
				sprite_img_NumberW[1].Draw(img_NumberW[1]);

				//��
				if (blackCount >= 10)
				{
					img_NumberB[0].SetNum(blackCount / 10, 0);
					sprite_img_NumberB[0].Draw(img_NumberB[0]);
				}

				img_NumberB[1].SetNum(blackCount % 10, 0);
				sprite_img_NumberB[1].Draw(img_NumberB[1]);

				//�`��I���̍��}
				d3d.EndScene();
				//�o�b�N�o�b�t�@���t�����g�֔��f
				d3d.Present();
			}
		}
	}

	return 0;

}