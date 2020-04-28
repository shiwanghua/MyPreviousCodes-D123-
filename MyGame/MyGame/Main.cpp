//                                      �غ���С��Ϸ����ʿ������
//                                      2017/8

//   ����ͷ�ļ�
#include<Windows.h>
#include<tchar.h>   //   ʹ��swprintf_s���������ͷ�ļ�
#include<time.h>    //   ʹ��time()������ȡϵͳʱ��

//   �������ļ�
#pragma comment(lib,"winmm.lib")     //   ����PlaySound����
#pragma comment(lib,"Msimg32.lib")   //   ����TransparentBlt����

//   ���帨����
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"[��������Ϩ�����Ϸ����֮��]GDI��Ϸ����"   //   ���ڱ���
#define PARTICLE_NUMBER 50                                     //   ѩ����������

//                                      ����ȫ�ֽṹ��
struct CHARACTER
{
	int NowHp;          //   Ŀǰ����ֵ
	int MaxHp;          //   �������ֵ
	int NowMp;          //   Ŀǰħ��ֵ
	int MaxMp;          //   ���ħ��ֵ
	int Level;          //   ��ɫ�ĵȼ�
	int Intelligence;   //   ��ɫ����ֵ   
	int Strength;       //   ��ɫ����ֵ
	int Agility;        //   ��ɫ����ֵ
};

struct SNOW
{
	int x;
	int y;
	BOOL exist;
};

//   ����ȫ�ֶ���ö����
enum ActionTypes
{
	ACTION_TYPE_NORMAL = 0,     //��ͨ���
	ACTION_TYPE_CRITICAL = 1,   //��������
	ACTION_TYPE_MAGIC = 2,        //ħ������
	ACTION_TYPE_MISS = 3,         //ħ�������������
	ACTION_TYPE_RECOVER = 4,      //�ظ�ħ��
};

//                                      ����ȫ�ֱ���:���,��,����,�ṹ���;
HDC g_hdc = NULL, g_mdc = NULL, g_bufdc = NULL;
HBITMAP g_hBackGround, g_hGameOver, g_hVictory, g_hSnow;
HBITMAP g_hMonsterBitmap, g_hHeroBitmap, g_hRecoverSkill;
HBITMAP g_hSkillButton1, g_hSkillButton2, g_hSkillButton3, g_hSkillButton4;   //   �ĸ����ܰ�ťλͼ���
HBITMAP g_hHeroSkill1, g_hHeroSkill2, g_hHeroSkill3;
HBITMAP g_hBossSkill1, g_hBossSkill2, g_hBossSkill3;

DWORD g_tPre = 0, g_tNow = 0;
int InitPaintTimes = 0;
int g_SnowNum = 0;                //   �������ѩ����
int g_iFrameNum, g_iTxtNum;       //   ֡������������
int g_Mx, g_My;                   //   �����������

wchar_t text[8][50];              //   �����������
SNOW SnowFlowers[PARTICLE_NUMBER];

CHARACTER Hero, Boss;
ActionTypes HeroActionType, BossActionType;
RECT g_rect;                      //   rect�ṹ�壬�洢�ڲ���������
BOOL g_bCanAttack, g_bGameOver;   //   ��ɫ�Ƿ���Թ���/��Ϸ�Ƿ���Խ���

//                                      ����ȫ�ֺ���
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL Game_Init(HWND);
VOID Game_InitPaint(HWND);
VOID Game_Main(HWND);       //   ��ͼ������ԭpaint��
VOID Message_Insert(wchar_t* str);
VOID Die_Check(int NowHp, bool isHero,HWND hwnd);
VOID HeroAction_Logic();
VOID HeroAction_Paint();
VOID HeroAction_Paint1();
VOID HeroAction_Paint2();
VOID HeroAction_Paint3();
VOID BossAction_Logic();
VOID BossAction_Paint(HWND hwnd);
VOID Snow_Paint();
BOOL Game_ShutDown(HWND);   //   ��Դ��������ԭclean up��

//                                      WindowsӦ�ó������ں���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lPCmdLine, int nShowCmd)
{
	//���ڴ���:��1����ƴ����ࣨ��ʼ��������
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);                          //   ��ƽṹ���ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;                                       //   ������ĸ����ڴ�
	wndClass.cbWndExtra = 0;                                       //   ������ĸ����ڴ�
	wndClass.hInstance = hInstance;                                //   ָ���������ڹ��̺����ĳ����ʵ�����
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                //   ������Ĺ����                                  *
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);   //   ��ɫ��ˢ���
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForMyDreamOfGameDevelop";           //   �����������

																   //���ڴ���:��2��ע�ᴰ����
	if (!RegisterClassEx(&wndClass))   //   �ú�����BOOL���͵�
	{
		return -1;
	}

	//���ڴ���:��3����ʽ��������
	HWND hwnd = CreateWindow(L"ForMyDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL,
		NULL, hInstance, NULL);

	//���ڴ���:��4�����ڵ��ƶ���ʾ�����
	MoveWindow(hwnd, 550, 250, WINDOW_WIDTH, WINDOW_HEIGHT, true);   //   ��������Ļ��λ��
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);                                             //   ���´���

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ���ˡ������������Game_Init()�������ߴ��ڵĴ�������", L"��Ϣ����", 0);
		return FALSE;
	}
	PlaySound(L"GameMedia\\�λ�����ԭ��-ս��1-ɭ��.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);   //   ѭ�����ű�������             *

																							//��Ϣѭ������
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);      //   ���������Ϣת��Ϊ�ַ���Ϣ, �����ɴ���MSG�ṹ����
			DispatchMessage(&msg);       //   �ַ���dispatch��һ����Ϣ�����ڳ���
		}
		else                             //   û�յ���Ϣ�ͽ��л�ͼ����
		{
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre >= 60&&InitPaintTimes==100)   //   �˴�ѭ���������ϴλ�ͼʱ�����0.06sʱ�ٽ����ػ����
				Game_Main(hwnd);
		}
	}

	//�������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);

	return 0;
}

//                                      ȫ�ֺ����Ķ���

//                                      ����ص�����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		if (InitPaintTimes < 100)
			Game_InitPaint(hwnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);                                                                 //           *
			PostQuitMessage(0);
			break;

		case VK_NUMPAD1:
			g_bCanAttack = TRUE;
			HeroActionType = ACTION_TYPE_NORMAL;
			break;

		case VK_NUMPAD2:
			g_bCanAttack = TRUE;
			HeroActionType = ACTION_TYPE_MAGIC;
			break;

		case VK_NUMPAD3:
			g_bCanAttack = TRUE;
			HeroActionType = ACTION_TYPE_RECOVER;
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_Mx = LOWORD(lParam);
		g_My = HIWORD(lParam);
		break;

	case WM_LBUTTONDOWN:
		if (!g_bCanAttack&&!g_bGameOver)
		{
			if (g_Mx >= 555 && g_Mx <= 605 && g_My >= 345 && g_My <= 395)
			{
				g_bCanAttack = TRUE;
				HeroActionType = ACTION_TYPE_NORMAL;
			}
			else if (g_Mx >= 605 && g_Mx <= 655 && g_My >= 395 && g_My <= 445)
			{
				g_bCanAttack = TRUE;
				HeroActionType = ACTION_TYPE_MAGIC;
			}
			else if (g_Mx >= 655 && g_Mx <= 705 && g_My >= 445 && g_My <= 495)
			{
				g_bCanAttack = TRUE;
				HeroActionType = ACTION_TYPE_RECOVER;
			}
			else 
				MessageBox(NULL, L"�һ����ط��ˣ�û�����ֲ�����", L"HINT", 0);
		}
		else if (g_bGameOver)   //   ���¿�ʼ����
		{
			if (Boss.NowHp > 0)
			{
				if (g_Mx > 253 && g_Mx < 542 && g_My>379 && g_My < 433)
				{
					if (!Game_Init(hwnd))
					{
						MessageBox(hwnd, L"��Դ��ʼ��ʧ���ˡ������������Game_Init()�������ߴ��ڵĴ�������", L"��Ϣ����", 0);
						return FALSE;
					}
				}
			}
			else   //   ���Ӯ��
			{
				MessageBox(hwnd, L"��������һ��Ӣ�۾������Ǿ����¹��������ٱ�ץһ�ΰɣ�", L"��Ϣ����", MB_YESNO);
				if (g_Mx > 395 && g_Mx < 478 && g_My>324 && g_My < 349)   //   �����и�bug��Ҫ�ڳ���Box֮ǰ��OK������ܣ���һ�ε�OK����NO������ܽ����if
				{                                                         //   ǧ���¶�ڣ�
					if (!Game_Init(hwnd))
					{
						MessageBox(hwnd, L"��Դ��ʼ��ʧ���ˡ������������Game_Init()�������ߴ��ڵĴ�������", L"��Ϣ����", 0);
						return FALSE;
					}
				}
				else if (g_Mx < 495 && g_Mx>575 && g_My > 325 && g_My < 348)
				{
					message = WM_DESTROY;
					DestroyWindow(hwnd);                                                        
					PostQuitMessage(0);
				}
			}
		}
		break;

	case WM_DESTROY:
		Game_ShutDown(hwnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

//                                      �����ʼ������
BOOL Game_Init(HWND hwnd)
{
	srand((unsigned)time(NULL));

	//������������ϵ
	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);
	HBITMAP bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);   //   ����һ���ʹ��ڼ��ݵĿյ�λͼ����

	SelectObject(g_mdc, bmp);

	g_hGameOver = (HBITMAP)LoadImage(NULL, L"GameMedia\\gameover.bmp", IMAGE_BITMAP, 1086, 396, LR_LOADFROMFILE);   //   1086������ͼ�Ŀ��
	g_hVictory = (HBITMAP)LoadImage(NULL, L"GameMedia\\victory.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);

	g_hBackGround = (HBITMAP)LoadImage(NULL, L"GameMedia\\bg.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
	g_hSnow = (HBITMAP)LoadImage(NULL, L"GameMedia\\snow.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_hRecoverSkill = (HBITMAP)LoadImage(NULL, L"GameMedia\\recover.bmp", IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);

	g_hMonsterBitmap = (HBITMAP)LoadImage(NULL, L"GameMedia\\monster.bmp", IMAGE_BITMAP, 360, 360, LR_LOADFROMFILE);
	g_hBossSkill1 = (HBITMAP)LoadImage(NULL, L"GameMedia\\monsterslash.bmp", IMAGE_BITMAP, 234, 188, LR_LOADFROMFILE);
	g_hBossSkill2 = (HBITMAP)LoadImage(NULL, L"GameMedia\\monstermagic.bmp", IMAGE_BITMAP, 387, 254, LR_LOADFROMFILE);
	g_hBossSkill3 = (HBITMAP)LoadImage(NULL, L"GameMedia\\monstercritical.bmp", IMAGE_BITMAP, 574, 306, LR_LOADFROMFILE);

	g_hHeroBitmap = (HBITMAP)LoadImage(NULL, L"GameMedia\\hero.bmp", IMAGE_BITMAP, 360, 360, LR_LOADFROMFILE);
	g_hHeroSkill1 = (HBITMAP)LoadImage(NULL, L"GameMedia\\heroslash.bmp", IMAGE_BITMAP, 364, 140, LR_LOADFROMFILE);
	g_hHeroSkill2 = (HBITMAP)LoadImage(NULL, L"GameMedia\\heromagic.bmp", IMAGE_BITMAP, 374, 288, LR_LOADFROMFILE);
	g_hHeroSkill3 = (HBITMAP)LoadImage(NULL, L"GameMedia\\herocritical.bmp", IMAGE_BITMAP, 574, 306, LR_LOADFROMFILE);
	g_hSkillButton1 = (HBITMAP)LoadImage(NULL, L"GameMedia\\skillbutton1.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
	g_hSkillButton2 = (HBITMAP)LoadImage(NULL, L"GameMedia\\skillbutton2.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
	g_hSkillButton3 = (HBITMAP)LoadImage(NULL, L"GameMedia\\skillbutton3.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
	g_hSkillButton4 = (HBITMAP)LoadImage(NULL, L"GameMedia\\skillbutton4.bmp", IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);   //   ��������:���ͽ���

	GetClientRect(hwnd, &g_rect);    //   ��ȡ�ڲ����ڴ�С���浽g_rect��

    //�趨��ҵĸ�������ֵ
	Hero.NowHp = Hero.MaxHp = 1000;
	Hero.NowMp = Hero.MaxMp = 60;
	Hero.Level = 6;
	Hero.Strength = 10;
	Hero.Agility = 20;
	Hero.Intelligence = 18;                                                                            //                         *

	//�趨����ĸ�������ֵ
	Boss.NowHp = Boss.MaxHp = 2000;
	Boss.NowMp = Boss.MaxMp = 0;
	Boss.Level = 12;
	Boss.Strength = 10;
	Boss.Agility = 10;
	Boss.Intelligence = 9;

	g_bGameOver = 0;
	g_bCanAttack = 0;
	g_iTxtNum = 0;                   //   ��ʼ��������ʾ��Ϣ������

	HFONT hFont;
	hFont = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);   //�������ֱ���Ϊ͸��

	InitPaintTimes = 0;
	SetTimer(hwnd, 1, 1, NULL);

	return TRUE;
}

//                                      �����ʼPaint����
VOID Game_InitPaint(HWND hwnd)
{
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	Snow_Paint();

	SelectObject(g_bufdc, g_hMonsterBitmap);
	TransparentBlt(g_mdc, 0.1*InitPaintTimes, 0.7*InitPaintTimes, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));

	SelectObject(g_bufdc, g_hHeroBitmap);
	TransparentBlt(g_mdc, 550-InitPaintTimes, 550-5.5*InitPaintTimes, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	InitPaintTimes++;
	if (InitPaintTimes == 100)
	{
		KillTimer(hwnd, 1);
		Game_Main(hwnd);
	}
}

//                                      ������ƺ���                                   
VOID Game_Main(HWND hwnd)
{
	wchar_t str[100];

	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	//�����Ϸδ������ģ����ѩ����,����ʾ��ս��Ϣ
	if (!g_bGameOver)
	{
		Snow_Paint();  

		//�����ľ��úڱ�
		if (g_iTxtNum <= 4)
		{
			SetTextColor(g_mdc, RGB(0, 0, 0));
			for (int i = 0; i < g_iTxtNum; i++)
				TextOut(g_mdc, 20, 410 + i * 18, text[i], wcslen(text[i]));
		}
		//�����ľ��ðױ�
		if (g_iTxtNum > 4)             
		{
			SetTextColor(g_mdc, RGB(0, 0, 0));
			for (int i = 0; i < 4; i++)
				TextOut(g_mdc, 20, 410 + i * 18, text[i], wcslen(text[i]));

			SetTextColor(g_mdc, RGB(255, 255, 255));
			for (int i = 4; i < g_iTxtNum; i++)
				TextOut(g_mdc, 90, 410 + i * 18, text[i], wcslen(text[i]));
		}
	}

	//���Ϲ���ͼ����ʾ����Ѫ��
	if (Boss.NowHp > 0)   //   ��ʾ��ǰ������ֵ��Ϊ����
	{
		SelectObject(g_bufdc, g_hMonsterBitmap);
		TransparentBlt(g_mdc, 10, 70, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));

		swprintf_s(str, L"Hp: %d/%d", Boss.NowHp, Boss.MaxHp);
		SetTextColor(g_mdc, RGB(255, 10, 10));
		TextOut(g_mdc, 70, 395, str, wcslen(str));                                           //                                     *
	
		swprintf_s(str, L"/��ħ��ֵ���޴�");
		SetTextColor(g_mdc, RGB(10, 10, 255));
		TextOut(g_mdc, 183, 395, str, wcslen(str));
	}

	//����ʿͼ����ʾ����ֵ��ħ��ֵ
	if (Hero.NowHp >= 0)   //   ��ʾ��ǰ��������ֵΪ����
	{
		swprintf_s(str, L"Hp: %d/%d", Hero.NowHp, Hero.MaxHp);
		SetTextColor(g_mdc, RGB(255, 10, 10));
		TextOut(g_mdc, 635, 350, str, wcslen(str));

		swprintf_s(str, L"Mp: %d/%d", Hero.NowMp, Hero.MaxMp);
		SetTextColor(g_mdc, RGB(10, 10, 255));
		TextOut(g_mdc, 635, 370, str, wcslen(str));

		SelectObject(g_bufdc, g_hHeroBitmap);
		TransparentBlt(g_mdc, 450, 50, 360, 360, g_bufdc, 0, 0, 360, 360, RGB(0, 0, 0));
	}

	if (g_bGameOver)
	{
		if (Hero.NowHp <= 0)
		{
			SelectObject(g_bufdc, g_hGameOver);
			BitBlt(g_mdc, 120, 50, 543, 396, g_bufdc, 543, 0, SRCAND);
			BitBlt(g_mdc, 120, 50, 543, 396, g_bufdc, 0, 0, SRCPAINT);
		}
		else
		{
			SelectObject(g_bufdc, g_hVictory);
			TransparentBlt(g_mdc, 0, 0, 800, 600, g_bufdc, 0, 0, 800, 600, RGB(0, 0, 0));
		}
	}
	//�����Ϸû�н���������ʿ���Թ���,���������ܰ�ť
	else if (!g_bCanAttack)
	{
		SelectObject(g_bufdc, g_hSkillButton1);
		BitBlt(g_mdc, 555, 345, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton2);
		BitBlt(g_mdc, 605, 395, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton3);
		BitBlt(g_mdc, 655, 445, 50, 50, g_bufdc, 0, 0, SRCCOPY);
		SelectObject(g_bufdc, g_hSkillButton4);
		BitBlt(g_mdc, 725, 445, 50, 50, g_bufdc, 0, 0, SRCCOPY);
	}
	//�����Ϸû�н����������ѡ�񹥻�����,�����ڲ��ɹ���״̬����ʱ��չ�ֹ���Ч��
	else
	{
		g_iFrameNum++;

		//���嵽ʮ�ĸ�����ʱ��ʾ��ҵĹ���Ч��
		if (g_iFrameNum >= 3 && g_iFrameNum <= 5)
		{
			if (g_iFrameNum == 5)
			{
				HeroAction_Logic();
				//��ʿ��������ж϶����Ƿ�����
				Die_Check(Boss.NowHp, false,hwnd);
			}
			HeroAction_Paint();
		}
		else if (g_iFrameNum == 8)
			HeroAction_Paint1();
		else if (g_iFrameNum == 11)
			HeroAction_Paint2();
		else if (g_iFrameNum == 14)
			HeroAction_Paint3();

		//��15�͵�26��30������ʱ�ж϶����Ĺ�������ʾ����Ч��
		if (g_iFrameNum == 15)
		{
			BossAction_Logic();
		}

		if (g_iFrameNum>=25&&g_iFrameNum <= 30)
			BossAction_Paint(hwnd);

		if (g_iFrameNum == 30)                 //   �غϽ���
		{
			g_bCanAttack = false;              //   �»غϿ�ʼ����ҿ����ٴι�����
			g_iFrameNum = 0;                   //   ֡������Ϊ0

											   //ÿ�غϽ���ʱ��ʿ���һ����ħ����Ȼ�ָ���6+2*��0������ֵ֮���һ���������
			if (!g_bGameOver)
			{
				int MpRecover = 2 * (rand() % Hero.Intelligence) + 6;
				Hero.NowMp += MpRecover;
				if (Hero.NowMp > Hero.MaxMp)   //   ��ֹħ��ֵ������Χ   
				{
					Hero.NowMp = Hero.MaxMp;
				}
				swprintf_s(str, L"�غϽ�������ʿ�ָ��ˡ�%d����ħ��", MpRecover);
				Message_Insert(str);
			}
		}   //   �˳���30֡��if
	}   //   �˳���Ϸδ������if

	SetTextColor(g_mdc, RGB(255, 255, 255));
	swprintf_s(str, L"���X���꣺%d", g_Mx);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"���Y���꣺%d", g_My);
	TextOut(g_mdc, 0, 20, str, wcslen(str));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	g_tPre = GetTickCount();   //   ��¼�˴λ�ͼʱ��
}

//                                      �����ս��Ϣ����,���ڲ���������Ϣ
VOID Message_Insert(wchar_t* str)
{
	//���û����8����Ϣ��ֱ���¿�һ��
	if (g_iTxtNum < 8)
	{
		swprintf_s(text[g_iTxtNum], str);
		g_iTxtNum++;
	}
	//�������8����Ϣ�������������һ��
	else
	{
		for (int i = 0; i < g_iTxtNum - 1; i++)
		{
			swprintf_s(text[i], text[i + 1]);
		}
		swprintf_s(text[7], str);
	}
}

//                                      ����Die_Check����,������������(��������Ϣ�����ֲ��ţ�
VOID Die_Check(int NowHp, bool isHero, HWND hwnd)   //   ��һ��������ĳ��ɫ��ǰ����ֵ,�ڶ��������жϸý�ɫ�Ƿ�����ʿ
{
	wchar_t str[100];

	if (NowHp < 0)
	{
		g_bGameOver = true;

		if (isHero)
		{
			PlaySound(L"FirstBlood.wav", NULL, SND_FILENAME | SND_ASYNC);
			swprintf_s(str, L"�����������������˹��������Ҷ���Ӯ......");
		}
		else   //   �������ˣ����Ӯ��
		{
			PlaySound(L"GameMedia\\victory.wav", NULL, SND_FILENAME | SND_ASYNC);
	
			if (Hero.NowHp < 200)
				swprintf_s(str, L"���������ʤ�ˣ�������һ����͹��ˣ�������������һ�ΰ���");
			else
				swprintf_s(str, L"��������,������������Ŀ�������");
		}
		Message_Insert(str);
	}
}

//                                      ����Ӣ�۶����߼�����
VOID HeroAction_Logic()
{
	int damage = 0;
	wchar_t str[100];

	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		if (1 == rand() % 4)   //   �����������
		{
			HeroActionType = ACTION_TYPE_CRITICAL;
			//�����˺�����ͨ������4.5��
			damage = (int)(4.5f*(float)(3 * (rand() % Hero.Agility) + Hero.Level*Hero.Strength + 20));
			swprintf_s(str, L"�չ������������ܡ����ͽ��ѡ�,4.5������...�Զ�������ˡ�%d�����˺�", damage);
		}
		else
		{
			//��ͨ������3���ԣ�0��������֮���һ������������ϵȼ�������������20������˺�
			damage = 3 * (rand() % Hero.Agility) + Hero.Level*Hero.Strength + 20;
			swprintf_s(str, L"��ʿ����ͨ�������޵�ն��,ֻ���˹��%d�����˺�", damage);
		}
		Boss.NowHp -= damage;
		Message_Insert(str);
		break;

	case ACTION_TYPE_MAGIC:
		if (Hero.NowMp >= 30)
		{
			//ħ��������5���ԣ�2*��0��������֮���һ���������+�ȼ�*IQֵ��
			damage = 5 * (2 * rand()%Hero.Agility + Hero.Level*Hero.Intelligence);
			Boss.NowHp -= (int)damage;
			Hero.NowMp -= 30;
			swprintf_s(str, L"�����ͷ��һ𽣷�...�Թ�����ɡ�%d�����˺�ֵ", damage);
		}
		else
		{
			HeroActionType = ACTION_TYPE_MISS;
			swprintf_s(str, L"������ʿ��ħ��ֵû30�㣬ʩ��ʧ�ܣ���غϰ׷���...���������!");
		}
		Message_Insert(str);
		break;

	case ACTION_TYPE_RECOVER:
		if (Hero.NowMp >= 40)
		{
			//������������40ħ��ֵ���ָ�Ѫ����5���ԣ�5*��0��IQֵ֮����������+40�����ָ���
			int HpRecover = 5 * (3 * (rand() % Hero.Intelligence) + 40);
			Hero.NowMp -= 40;
			Hero.NowHp += HpRecover;
			if (Hero.NowHp > Hero.MaxHp)
			{
				Hero.NowHp = Hero.MaxHp;
			}
			swprintf_s(str, L"��ҿ�ʼ�����ˣ�ʹ���������ָ��ˡ�%d��������ֵ��", HpRecover);
		}
		else
		{
			HeroActionType = ACTION_TYPE_MISS;
			swprintf_s(str, L"������Ҫ���ˣ�ħ��ֵ����40�㣬ʩ��ʧ�ܣ�");
		}
		Message_Insert(str);
		break;
	}   //   ����switch���
}   //   �����߼�����

//                                      ����Ӣ�۶�����ͼ����
VOID HeroAction_Paint()
{
	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		SelectObject(g_bufdc, g_hHeroSkill1);
		TransparentBlt(g_mdc, 220, 50, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_MAGIC:
		SelectObject(g_bufdc, g_hHeroSkill2);
		TransparentBlt(g_mdc, 220, 50, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_CRITICAL:
		SelectObject(g_bufdc, g_hHeroSkill3);
		TransparentBlt(g_mdc, 60, 50, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_RECOVER:
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 600, 160, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));
		break;
	}
}
VOID HeroAction_Paint1()
{
	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		SelectObject(g_bufdc, g_hHeroSkill1);
		TransparentBlt(g_mdc, 50, 180, 364, 140, g_bufdc, 0, 0, 364, 140, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_MAGIC:
		SelectObject(g_bufdc, g_hHeroSkill2);
		TransparentBlt(g_mdc, 175, 100, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_CRITICAL:
		SelectObject(g_bufdc, g_hHeroSkill3);
		TransparentBlt(g_mdc, 100, 80, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_RECOVER:
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 620, 200, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));
		break;
	}
}
VOID HeroAction_Paint2()
{
	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		SelectObject(g_bufdc, g_hHeroSkill1);
		TransparentBlt(g_mdc, 80, 210, 364, 140, g_bufdc, 0, 0, 364, 140, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_MAGIC:
		SelectObject(g_bufdc, g_hHeroSkill2);
		TransparentBlt(g_mdc, 130, 50, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_CRITICAL:
		SelectObject(g_bufdc, g_hHeroSkill3);
		TransparentBlt(g_mdc, 140, 100, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_RECOVER:
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 620, 240, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));
		break;
	}
}
VOID HeroAction_Paint3()
{
	switch (HeroActionType)
	{
	case ACTION_TYPE_NORMAL:
		SelectObject(g_bufdc, g_hHeroSkill1);
		TransparentBlt(g_mdc, 110, 240, 364, 140, g_bufdc, 0, 0, 364, 140, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_MAGIC:
		SelectObject(g_bufdc, g_hHeroSkill2);
		TransparentBlt(g_mdc, 85, 100, 374, 288, g_bufdc, 0, 0, 374, 288, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_CRITICAL:
		SelectObject(g_bufdc, g_hHeroSkill3);
		TransparentBlt(g_mdc, 180, 50, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));
		break;

	case ACTION_TYPE_RECOVER:
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 630, 280, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));
		break;
	}
}

//                                      ����Boss�����߼�������ֻ�жϳ����غϵĹ������ͣ��������˺�ֵ�ȣ�
VOID BossAction_Logic()
{
	srand((unsigned)time(NULL));
	if (Boss.NowHp > (Boss.MaxHp / 2))
	{
		switch (rand() % 3)
		{
		case 0:
			BossActionType = ACTION_TYPE_NORMAL;
			break;

		case 1:
			BossActionType = ACTION_TYPE_CRITICAL;
			break;

		case 2:
			BossActionType = ACTION_TYPE_MAGIC;
			break;
		}
	}
	//������ֵС�ڶ���֮һʱ��������һ���Ѫ���ܲ����ڷ�����ͨ����
	else
	{
		switch (rand() % 3)
		{
		case 0:
			BossActionType = ACTION_TYPE_MAGIC;
			break;

		case 1:
			BossActionType = ACTION_TYPE_CRITICAL;
			break;

		case 2:
			BossActionType = ACTION_TYPE_RECOVER;
			break;
		}
	}   //   �˳�if���
}

//                                      ����Boss������ͼ���������������˺�ֵ�ȣ�
VOID BossAction_Paint(HWND hwnd)
{
	int damage = 0, recover = 0;
	wchar_t str[100];

	switch (BossActionType)
	{
	case ACTION_TYPE_NORMAL:
		SelectObject(g_bufdc, g_hBossSkill1);
		TransparentBlt(g_mdc, 520, 150, 234, 188, g_bufdc, 0, 0, 234, 188, RGB(0, 0, 0));

		//����ʮ������ʱ����������˺��̶Ȳ�������ʾ��Ϣ�������caseͬ��
		if (g_iFrameNum == 30)
		{
			//�չ�����ڤ��𡱣�0��Boss������֮���һ�������+Boss����ֵ*����ȼ�
			damage = rand() % Boss.Agility + Boss.Strength*Boss.Level;
			Hero.NowHp -= (int)damage;

			swprintf_s(str, L"ħ���ͷ���ڤ���...��ȥ��ҡ�%d�����˺�", damage);
			Message_Insert(str);

			//ħ����������ж�Ӣ���Ƿ��Ҵ棬�����caseͬ��
			Die_Check(Hero.NowHp, true, hwnd);
		}
		break;

	case ACTION_TYPE_MAGIC:
		SelectObject(g_bufdc, g_hBossSkill2);
		TransparentBlt(g_mdc, 500, 70, 387, 254, g_bufdc, 0, 0, 387, 254, RGB(0, 0, 0));

		if (g_iFrameNum == 30)
		{
			//��Ѫ��ħ��������2���ԣ�2* 0��Boss������֮���һ�������+����ֵ��������ֵ��  
			//ħ��������Ѫ��  ��Ѫ�����˺�ֵ�İٷ�֮20��
			damage = 2 * (2 * (rand() % Boss.Agility) + Boss.Strength*Boss.Intelligence);
			Hero.NowHp -= damage;
			recover = (int)((float)damage*0.2f);
			Boss.NowHp += recover;

			swprintf_s(str, L"а������������������ɡ�%d�����˺�ֵ����������Ѫ��%d��������ֵ.", damage, recover);
			Message_Insert(str);

			Die_Check(Hero.NowHp, true, hwnd);
		}
		break;

	case  ACTION_TYPE_CRITICAL:
		SelectObject(g_bufdc, g_hBossSkill3);
		TransparentBlt(g_mdc, 280, 100, 574, 306, g_bufdc, 0, 0, 574, 306, RGB(0, 0, 0));

		if (g_iFrameNum == 30)
		{
			//�ͷż��ܡ�����һ������2* (������+ �ȼ�*����ֵ)
			damage = 2 * (rand() % Boss.Agility + Boss.Level*Boss.Strength);
			Hero.NowHp -= (int)damage;

			swprintf_s(str, L"������ŭ��������ҡ�%d��������ֵ��", damage);
			Message_Insert(str);

			Die_Check(Hero.NowHp, true,hwnd);
		}
		break;

	case ACTION_TYPE_RECOVER:
		SelectObject(g_bufdc, g_hRecoverSkill);
		TransparentBlt(g_mdc, 150, 150, 150, 150, g_bufdc, 0, 0, 150, 150, RGB(0, 0, 0));

		if (g_iFrameNum == 30)
		{
			//÷��˹ķ��Ѫ���ܣ�2���ԣ�����ֵ)^2
			recover = 2 * Boss.Intelligence*Boss.Intelligence;
			Boss.NowHp += recover;

			swprintf_s(str, L"ħ����ʹ��÷��˹ķ��Ѫ��%d��������ֵ...��������ˣ���������û�뵽�ɣ�", recover);
			Message_Insert(str);
		}
		break;
	}
}

//                                      ����Snow_Paint()������ģ��ѩ���׷�
VOID Snow_Paint()   //   ÿ����һ��Game_Main()��������һ��Snow_Paint()����
{
	//���δ�������,�����ѩ�������Ʈ������Ч��
	//��ѩ��������50ʱ���ڴ��ڶ�������һ��ѩ������
	if (g_SnowNum < PARTICLE_NUMBER)
	{
		SnowFlowers[g_SnowNum].x = rand() % g_rect.right;
		SnowFlowers[g_SnowNum].y = 0;
		SnowFlowers[g_SnowNum].exist = true;   //   ѩ����ʽ���ڣ���ʾ���Ŀ��Խ��л�ͼ
		g_SnowNum++;
	}

	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (SnowFlowers[i].exist)
		{
			SelectObject(g_bufdc, g_hSnow);
			TransparentBlt(g_mdc, SnowFlowers[i].x, SnowFlowers[i].y, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));

			//�����������ѩ���ĺ����ƶ���ƫ����
			if (rand() % 2 == 0)
				SnowFlowers[i].x += rand() % 6;
			else
				SnowFlowers[i].x -= rand() % 6;

			SnowFlowers[i].y += 10;
			//���䵽���ϵ�ѩ���ͻض���
			if (SnowFlowers[i].y > g_rect.bottom)
			{
				SnowFlowers[i].x = rand() % g_rect.right;
				SnowFlowers[i].y = 0;
			}
		}
	}   //   ����for��ͼѭ��
}   //   ����ѩ��Paint����


BOOL Game_ShutDown(HWND hwnd)   //   �ͷ���Դ����
{
	DeleteObject(g_hBackGround);
	DeleteObject(g_hGameOver);
	DeleteObject(g_hVictory);
	DeleteObject(g_hSnow);
	DeleteObject(g_hRecoverSkill);

	DeleteObject(g_hMonsterBitmap);
	DeleteObject(g_hBossSkill1);
	DeleteObject(g_hBossSkill2);
	DeleteObject(g_hBossSkill3);

	DeleteObject(g_hHeroBitmap);
	DeleteObject(g_hHeroSkill1);
	DeleteObject(g_hHeroSkill2);
	DeleteObject(g_hHeroSkill3);
	DeleteObject(g_hSkillButton1);
	DeleteObject(g_hSkillButton2);
	DeleteObject(g_hSkillButton3);
	DeleteObject(g_hSkillButton4);

	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);

	return true;
}