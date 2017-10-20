
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
	int width = 800,	//�N���C�A���g�̈�̕�
	int height = 600)	//�N���C�A���g�̈�̍���
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
		_T("�^�C�g��"),			//�E�B���h�E�̃^�C�g��
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
	bool Mouseflag = false, Reverseflag = false;
	bool Reverseflag8[8];

	for (int count = 0; count < 8; count++)
	{
		Reverseflag8[count] = false;
	}

	//�}�E�X���W
	int MousePosX, MousePosY;

	PieceState PS[MapSize][MapSize];

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
			sprite_img_Chooseable[countY][countX].SetSize(PieceImageSize, PieceImageSize);
			sprite_img_Chooseable[countY][countX].SetAngle(0);

			img_Chooseable[countY][countX].Load(_T("Texture/Chooseable.png"));
			img_Chooseable[countY][countX].SetDivide(0, 0);
			img_Chooseable[countY][countX].SetNum(0, 0);
		}
	}

	Game_MODE Mode;
	Mode = GameStartProcessing; //�������[�h�����Z�������[�h�ɐݒ�

	Game_WB_MODE WB;
	WB = Mode_black;

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

			if (pDi->KeyJustPressed(DIK_A))
			{
				MessageBox(NULL,
					TEXT("�L�[���͊m�F"),
					TEXT("�e�X�g-�^�C�g��"),
					MB_OK);
			}

			switch (Mode)
			{

				//�X�^�[�g���
			case Game_MODE::StartScreenProcessing:

				break;

				//��������
			case Game_MODE::GameStartProcessing:

				//�Ֆʂ̃��Z�b�g
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						PS[countY][countX] = null;
					}
				}

				//�����̔Ֆ�
				PS[3][3] = white, PS[4][4] = white;
				PS[3][4] = black, PS[4][3] = black;

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

				//���]�����p�i8�����j�t���O�̃��Z�b�g
				for (int count = 0; count < 8; count++)
				{
					Reverseflag8[count] = false;
				}

				//�I���\����
				for (int countY = 0; countY < MapSize; countY++)
				{
					for (int countX = 0; countX < MapSize; countX++)
					{
						//����ł���ꍇ�B�i���Ԃ����̏ꍇ�j
						if (WB == Mode_black && PS[countY][countX] == white)
						{
							//������
							if (countX != 0 && countX + 1 < MapSize)
							{
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//���������ꍇ
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

								//������
								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									//���������ꍇ
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
							//������
							if (countY != 0 && countY + 1 < MapSize)
							{
								for (Ycount = countY + 1; Ycount < MapSize; Ycount++)
								{
									//���������ꍇ
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
								//������
								for (Ycount = countY - 1; Ycount >= 0; Ycount--)
								{
									//���������ꍇ
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
								//�E��
								Ycount = countY - 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//���������ꍇ
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
								//�E��
								Ycount = countY + 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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
								//����

								Ycount = countY - 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//���������ꍇ
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
								//����
								Ycount = countY + 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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
						//����ł����āA�O���łȂ��ꍇ�B�i���Ԃ����̏ꍇ�j
						else if (WB == Mode_white&&PS[countY][countX] == black)
						{

							if (countX != 0 && countX + 1 < MapSize)
							{
								//������
								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									//���������ꍇ
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
								//������
								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									//���������ꍇ
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
								//������
								for (Ycount = countY + 1; Ycount < MapSize; Ycount++)
								{
									//���������ꍇ
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
								//������
								for (Ycount = countY - 1; Ycount >= 0; Ycount--)
								{
									//���������ꍇ
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
								//�E��
								Ycount = countY - 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount <= 0)
									{
										break;
									}

									//���������ꍇ
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

								//�E��
								Ycount = countY + 1;

								for (Xcount = countX + 1; Xcount < MapSize; Xcount++)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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

								//����

								Ycount = countY - 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount <= 0)
									{
										break;
									}


									//���������ꍇ
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

								//����
								Ycount = countY + 1;

								for (Xcount = countX - 1; Xcount >= 0; Xcount--)
								{
									if (Ycount >= MapSize)
									{
										break;
									}

									//���������ꍇ
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

				//�v���C���[���샂�[�h�Ɉړ�
				Mode = PlayerProcessing;

				break;

				//�v���C���[���샂�[�h
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
				case Mode_white:

					//������
					if (Reverseflag8[0] == true)
					{
						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							//���������ꍇ
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

					//������
					if (Reverseflag8[1] == true)
					{
						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							//���������ꍇ
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

					//������
					if (Reverseflag8[2] == true)
					{
						for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
						{
							//���������ꍇ

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

					//������
					if (Reverseflag8[3] == true)
					{
						for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
						{
							//���������ꍇ
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

					//�E��
					if (Reverseflag8[4] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//���������ꍇ
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

					//�E��
					if (Reverseflag8[5] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//���������ꍇ
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

					//����
					if (Reverseflag8[6] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//���������ꍇ
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

					//����
					if (Reverseflag8[7] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//���������ꍇ
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

					//���̏��ԂɕύX
					WB = Mode_black;
					//�I���\�������[�h�Ɉړ�
					Mode = SelectableProcessing;
					break;
				case Mode_black:

					//������
					if (Reverseflag8[0] == true)
					{
						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							//���������ꍇ
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

					//������
					if (Reverseflag8[1] == true)
					{
						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							//���������ꍇ
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

					//������
					if (Reverseflag8[2] == true)
					{
						for (Ycount = MousePosY + 1; Ycount < MapSize; Ycount++)
						{
							//���������ꍇ

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

					//������
					if (Reverseflag8[3] == true)
					{
						for (Ycount = MousePosY - 1; Ycount >= 0; Ycount--)
						{
							//���������ꍇ
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

					//�E��
					if (Reverseflag8[4] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//���������ꍇ
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

					//�E��
					if (Reverseflag8[5] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX + 1; Xcount < MapSize; Xcount++)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//���������ꍇ
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

					//����
					if (Reverseflag8[6] == true)
					{
						Ycount = MousePosY - 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount <= 0)
							{
								break;
							}

							//���������ꍇ
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

					//����
					if (Reverseflag8[7] == true)
					{
						Ycount = MousePosY + 1;

						for (Xcount = MousePosX - 1; Xcount >= 0; Xcount--)
						{
							if (Ycount >= MapSize)
							{
								break;
							}

							//���������ꍇ
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

					//���̏��ԂɕύX
					WB = Mode_white;
					//�I���\�������[�h�Ɉړ�
					Mode = SelectableProcessing;
					break;
				}

				break;

				//�Q�[���I�����[�h
			case Game_MODE::GameEndProcessing:

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

				//�`��I���̍��}
				d3d.EndScene();
				//�o�b�N�o�b�t�@���t�����g�֔��f
				d3d.Present();
			}
		}
	}

	return 0;

}