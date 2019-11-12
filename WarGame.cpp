// WarGame.cpp : 定义应用程序的入口点。
//

#include "WarGame.h"

using namespace std;


// 全局变量
#pragma region 

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名


// 背景图像资源
HBITMAP bmp_Grass;			//背景图像资源

HBITMAP bmp_Background;		//生成的背景图像

HBITMAP bmp_Victory;        //战斗胜利图像
HBITMAP bmp_Defeat;         //战斗失败图像



// 按钮资源
HBITMAP bmp_AdventureMode;  //冒险模式按钮图像资源
HBITMAP bmp_VersusMode;     //对战模式按钮图像资源
HBITMAP bmp_Help;			//帮助文档按钮图像资源

HBITMAP bmp_StartWAR;	//开始战斗按钮图像资源
HBITMAP bmp_Exit;       //退出游戏返回主界面
HBITMAP bmp_Next;			//下一关按钮图像资源
HBITMAP bmp_Again;			//重来按钮图像资源

HBITMAP bmp_Unit_Red;		//红方主角图像资源
HBITMAP bmp_Unit_Blue;		//蓝方主角图像资源


Stage* currentStage = NULL; //当前场景状态
vector<Unit*> units;		//战斗单位
vector<Unit*> deployUnits;  //部署单位
vector<Button*> buttons;	//按钮

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
Unit* controlUnit=NULL;         //鼠标正在操控的鼠标


//帧
int FRAMES_HOLD[] = {0};
int FRAMES_HOLD_COUNT = 1;
int FRAMES_WALK[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2};
int FRAMES_WALK_COUNT = 16;
int FRAMES_ATTACK[] = {3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int FRAMES_ATTACK_COUNT = 24;



// TODO: 在此添加其它全局变量 
map<int,Property*> PROPERTYMAP;        //全局单位属性表
#define PM PROPERTYMAP
int attack[5] = { 20,15,10,3,8 };      //攻击力
int defense[5] = { 100,100,200,400,300 }; //防御力
int health[5] = { 1000,1000,1500,3000,2000 }; //生命值
int speed[5] = { 5,2,3,1,1 }; //速度值
int attackArea[5] = { 50,250,50,50,50}; //攻击范围
int cost[5] = { 200,150,100,50,50 }; //单位花费




double const PI = acos(double(-1));


#pragma endregion

// Win32程序框架
#pragma region 


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WARGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WARGAME));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WARGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindow(szWindowClass, szTitle,
	   WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
	   CW_USEDEFAULT, 
	   CW_USEDEFAULT,
	   WINDOW_WIDTH,
	   WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		// 定时器事件
		if(currentStage!=NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
    case WM_PAINT:
		// 绘图
		Paint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


#pragma endregion


// 事件处理函数
#pragma region 

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	//加载图像资源
	bmp_Grass = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	bmp_Victory= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Victory));
	bmp_Defeat = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	
	//加载战斗人物资源
	bmp_Unit_Red = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RED));
	bmp_Unit_Blue = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLUE));

	//加载开始界面按钮资源
	bmp_AdventureMode = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_AdventureMode));
	bmp_VersusMode = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_VersusMode));
	bmp_Help = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Help));


	//加载战斗按钮资源	
	bmp_StartWAR = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Exit= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_EXIT));
	bmp_Next = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Next));
	bmp_Again = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Again));
	bmp_Victory= LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Victory));
	bmp_Defeat = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Defeat));

	//初始化全局属性表
	for (int i = 0; i < 5; i++) {
		Property* p = new Property();
		p->attack = attack[i];
		p->attackArea = attackArea[i];
		p->defense = defense[i];
		p->health = health[i];
		p->speed = speed[i];
		PROPERTYMAP[i] = p;
	}

	//加载部署人物资源 
	for (int i = 0; i < 10; i++) {
		Unit* unit = CreateUnit(10000+1-i / 5, i % 5, (WINDOW_WIDTH - 10 * UNIT_SIZE_X) / 2 + i * UNIT_SIZE_X, (WINDOW_HEIGHT - UNIT_SIZE_Y));
		deployUnits.push_back(unit);
	}

	//添加按钮
	Button* adventureModeButton = CreateButton(BUTTON_ADVENTUREMODE, bmp_AdventureMode, BUTTON_ADVENTUREMODE_WIDTH,
		BUTTON_ADVENTUREMODE_HEIGHT, (WINDOW_WIDTH - BUTTON_ADVENTUREMODE_WIDTH) / 2, (WINDOW_HEIGHT - 2 * BUTTON_ADVENTUREMODE_HEIGHT) / 2);
	Button* versusModeButton = CreateButton(BUTTON_VERSUSMODE, bmp_VersusMode, BUTTON_VERSUSMODE_WIDTH, BUTTON_VERSUSMODE_HEIGHT,
		(WINDOW_WIDTH - BUTTON_VERSUSMODE_WIDTH) / 2, (WINDOW_HEIGHT + BUTTON_VERSUSMODE_HEIGHT) / 2);
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_Help, BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT,
		(WINDOW_WIDTH - BUTTON_HELP_WIDTH) / 2, (WINDOW_HEIGHT + 4*BUTTON_HELP_HEIGHT) / 2);

	Button* startButton = CreateButton(BUTTON_STARTWAR, bmp_StartWAR, BUTTON_STARTWAR_WIDTH, BUTTON_STARTWAR_HEIGHT,
		(WINDOW_WIDTH - 2*BUTTON_STARTWAR_WIDTH) / 2, BUTTON_STARTWAR_HEIGHT / 2);
	Button* exitButton = CreateButton(BUTTON_EXIT, bmp_Exit, BUTTON_EXIT_WIDTH, BUTTON_EXIT_HEIGHT,
		(WINDOW_WIDTH + BUTTON_EXIT_WIDTH) / 2, BUTTON_EXIT_HEIGHT / 2);
	Button* nextButton = CreateButton(BUTTON_NEXT, bmp_Next, BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT,
		(WINDOW_WIDTH - BUTTON_NEXT_WIDTH) / 2-BUTTON_NEXT_WIDTH, (WINDOW_HEIGHT + 3 * BUTTON_NEXT_HEIGHT) / 2);
	Button* againButton=CreateButton(BUTTON_AGAIN,bmp_Again,BUTTON_AGAIN_WIDTH,BUTTON_AGAIN_HEIGHT,
		(WINDOW_WIDTH - BUTTON_AGAIN_WIDTH) / 2+BUTTON_AGAIN_WIDTH, (WINDOW_HEIGHT + 3 * BUTTON_AGAIN_HEIGHT) / 2);
	Button* victoryButton = CreateButton(BUTTON_VICTORY, bmp_Victory, BUTTON_VICTORY_WIDTH, BUTTON_VICTORY_HEIGHT,
		(WINDOW_WIDTH - BUTTON_VICTORY_WIDTH) / 2, (WINDOW_HEIGHT-BUTTON_VICTORY_HEIGHT)/2);
	Button* defeatButton= CreateButton(BUTTON_DEFEAT, bmp_Defeat, BUTTON_DEFEAT_WIDTH, BUTTON_DEFEAT_HEIGHT,
		(WINDOW_WIDTH - BUTTON_DEFEAT_WIDTH) / 2, (WINDOW_HEIGHT-BUTTON_DEFEAT_HEIGHT)/ 2);

	buttons.push_back(adventureModeButton);
	buttons.push_back(versusModeButton);
	buttons.push_back(helpButton);
	buttons.push_back(startButton);
	buttons.push_back(exitButton);
	buttons.push_back(nextButton);
	buttons.push_back(againButton);
	buttons.push_back(victoryButton);
	buttons.push_back(defeatButton);


	//初始化背景
	bmp_Background = InitBackGround(hWnd, bmp_Grass);


	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		break;
	case VK_RIGHT:
		keyRightDown = true;
		break;
	case VK_ESCAPE:
		Pause();
		InvalidateRect(hWnd, NULL, FALSE);
	default:
		break;
	}
}

//按下ESC键暂停
void Pause() {
	if ((currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_5) || currentStage->stageID == STAGE_VERSUS) {
		currentStage->timerOn = FALSE;
		buttons[BUTTON_NEXT-1000]->visible = TRUE;
		buttons[BUTTON_AGAIN-1000]->visible = TRUE;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	if (controlUnit != NULL) {
		controlUnit->x = mouseX;
		controlUnit->y = mouseY;
	}
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	// 按钮操作
	for(int i=0;i<buttons.size();i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				switch (button->buttonID) {
				case BUTTON_ADVENTUREMODE:
				{
					//TODO：判断进入哪个关卡
					InitStage(hWnd, STAGE_1);
					break;
				}
				case BUTTON_VERSUSMODE:
				{
					InitStage(hWnd, STAGE_VERSUS);
					break;
				}
				case BUTTON_HELP:
				{
					InitStage(hWnd, STAGE_HELP);
					break;
				}
				case BUTTON_STARTWAR:
				{
					currentStage->isWAR = TRUE;
					break;
				}
				case BUTTON_EXIT:
				{
					InitStage(hWnd, STAGE_STARTMENU);
				}
				case BUTTON_NEXT:
				{
					if (currentStage->timerOn == FALSE) {
						currentStage->timerOn = TRUE;
						buttons[BUTTON_NEXT-1000]->visible = FALSE;
						buttons[BUTTON_AGAIN-1000]->visible = FALSE;
					}
					else {
						InitStage(hWnd, currentStage->stageID + 1);
					}
					break;
				}
				case BUTTON_AGAIN:
				{
					InitStage(hWnd, currentStage->stageID);
					break;
				}
				default:
					break;
				}
			}
		}
	}

	//战斗状态下不能操作单位
	if (currentStage->isWAR == TRUE) return;

	// 部署操作
	if (currentStage->stageID == STAGE_VERSUS) {
		for (int i = 0; i < deployUnits.size(); i++) {
			Unit* unit = deployUnits[i];
			if (mouseX > unit->x-UNIT_SIZE_X/2
				&& mouseX < unit->x + UNIT_SIZE_X/2
				&& mouseY> unit->y-UNIT_SIZE_Y/2
				&& mouseY < unit->y + UNIT_SIZE_Y/2)
			{
				controlUnit = new Unit();
				(*controlUnit) = *deployUnits[i];
				units.push_back(controlUnit);
				return;
			}
			for (int i = 0; i < units.size(); i++) {
				Unit* unit = units[i];
				if (mouseX > unit->x-UNIT_SIZE_X/2
					&& mouseX < unit->x + UNIT_SIZE_X/2
					&& mouseY> unit->y-UNIT_SIZE_Y/2
					&& mouseY < unit->y + UNIT_SIZE_Y/2)
				{
					controlUnit = units[i];
					return;
				}
			}
		}
	}
	if (currentStage->stageID >= STAGE_1 && currentStage->stageID < STAGE_VERSUS) {
		for (int i = 0; i < deployUnits.size()/2; i++) {
			Unit* unit = deployUnits[i];
			if (mouseX > unit->x-UNIT_SIZE_X/2
				&& mouseX < unit->x + UNIT_SIZE_X/2
				&& mouseY> unit->y-UNIT_SIZE_Y/2
				&& mouseY < unit->y + UNIT_SIZE_Y/2)
			{
				controlUnit = new Unit();
				(*controlUnit) = *deployUnits[i];
				units.push_back(controlUnit);
				return;
			}
		}
		for (int i = 0; i < units.size(); i++) {
			Unit* unit = units[i];
			if (unit->side == UNIT_SIDE_BLUE) {
				if (mouseX > unit->x-UNIT_SIZE_X/2
					&& mouseX < unit->x + UNIT_SIZE_X/2
					&& mouseY> unit->y-UNIT_SIZE_Y/2
					&& mouseY < unit->y + UNIT_SIZE_Y/2)
				{
					controlUnit = units[i];
					return;
				}
			}
		}
	}
}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
	if (controlUnit != NULL) {
		controlUnit->x = mouseX;
		controlUnit->y = mouseY;
	}
	controlUnit = NULL;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateUnits(hWnd);

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


//其它游戏状态处理函数
#pragma region 


//TODO: 添加游戏需要的更多函数

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// 添加主角函数
Unit* CreateUnit(int side, int type, int x, int y)
{
	Unit* unit = new Unit();
	unit->side = side;
	if(side==UNIT_SIDE_RED){
		unit->img = bmp_Unit_Red;
		unit->direction = UNIT_DIRECT_LEFT;
	}else if(side==UNIT_SIDE_BLUE){
		unit->img = bmp_Unit_Blue;
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	
	unit->type=type;
	unit->state = UNIT_STATE_HOLD;
	

	unit->frame_row = type;
	unit->frame_column = UNIT_LAST_FRAME * unit->direction;

	unit->frame_sequence = FRAMES_HOLD;
	unit->frame_count = FRAMES_HOLD_COUNT;
	unit->frame_id = 0;

	unit->x = x;
	unit->y = y;
	unit->vx = 0;
	unit->vy = 0;
	unit->health = PROPERTYMAP[type]->health;
	unit->visible = TRUE;
	return unit;
}



// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例
	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;


	if (stageID == STAGE_STARTMENU) {
		currentStage->isWAR = FALSE;
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//清空单位
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
		units.clear();

		//显示开始界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_ADVENTUREMODE || button->buttonID==BUTTON_VERSUSMODE || button->buttonID==BUTTON_HELP) 
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}


	}
	if(stageID >= STAGE_1 && stageID < STAGE_VERSUS) //TODO：添加多个游戏场景
	{
		currentStage->isWAR = FALSE;
		currentStage->bg = bmp_Grass;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//清空单位
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
		units.clear();

		//显示游戏界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (button->buttonID==BUTTON_STARTWAR || button->buttonID==BUTTON_EXIT) //TODO：加载游戏界面需要的按钮
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}


		// 按场景初始化单位
		switch(stageID){
			case STAGE_1: 
				// 场景一
				// 一个盾卫，一个重装步兵
			{
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 600, 200));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 600, 300));
				break;
			}
			case STAGE_2:
				// 场景二
				// 两个盾卫，两个重装步兵
			{
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 600, 300));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 500, 200));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 700, 250));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 700, 350));
				break;
			}
			case STAGE_3:
				// 场景三
				// 一个盾卫，两个重装步兵，一个剑士
			{
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 500, 300));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 550, 400));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SABER, 650, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 550, 350));
				break;
			}
			case STAGE_4:
				// 场景四
				// 两个盾卫，一个重装步兵，两个剑士，一个魔术师
			{
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 500, 300));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 550, 400));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SABER, 550, 450));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SABER, 650, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 550, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_CASTER, 700, 350));
				break;
			}
			case STAGE_5:
				// 场景五
				// 两个盾卫，两个重装步兵，两个剑士，两个魔术师，一个收割者
			{
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 500, 300));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SHIELDER, 550, 400));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SABER, 550, 450));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_SABER, 650, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 550, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_HOPLITE, 450, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_CASTER, 700, 350));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_CASTER, 700, 450));
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_REAPER, 600, 550));
				break;
			}
			default:
			{
				int nums[5] = { 0,0,0,0,0 };
				srand((unsigned int)time(NULL));
				for (int i = 0; i < 5; i++) {
					nums[i] = rand() % 4;
					for (int j = 0; j < nums[i]; j++) {
						units.push_back(CreateUnit(UNIT_SIDE_RED, i, rand() % 20 * 40+UNIT_SIZE_X, rand() % 20 * 30+UNIT_SIZE_Y));
					}
				}
				break;
			}
		}

		ArrangeDeployUnits();

	}
	

	if (stageID >=STAGE_VERSUS && stageID<STAGE_HELP) {
		currentStage->isWAR = FALSE;
		currentStage->bg = bmp_Grass;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//清空单位
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
		units.clear();

		//显示游戏界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTWAR || button->buttonID==BUTTON_EXIT) //TODO：加载游戏界面需要的按钮
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}
		ArrangeDeployUnits();
	}

	if (stageID == STAGE_HELP) {
		//帮助界面
		return;
	}
	



	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


// 刷新单位状态函数
void UpdateUnits(HWND hWnd)
{
	if (currentStage->isWAR ==FALSE) return;

	//判断战斗是否结束
	int blueCount = 0;
	int redCount = 0;
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->health > 0 && units[i]->side==UNIT_SIDE_BLUE) {
			blueCount++;
		}
		if (units[i]->health > 0 && units[i]->side == UNIT_SIDE_RED) {
			redCount++;
		}
	}
	if (redCount == 0) {
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
		units.clear();
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i]->buttonID == BUTTON_VICTORY || buttons[i]->buttonID == BUTTON_NEXT || buttons[i]->buttonID == BUTTON_AGAIN) buttons[i]->visible = TRUE;
			else buttons[i]->visible = FALSE;
		}
		currentStage->isWAR = FALSE;
		return;
	}
	if (blueCount == 0) {
		for (int i = 0; i < units.size(); i++) {
			delete units[i];
		}
		units.clear();
		for (int i = 0; i < buttons.size(); i++) {
			if (buttons[i]->buttonID == BUTTON_DEFEAT || buttons[i]->buttonID == BUTTON_NEXT || buttons[i]->buttonID == BUTTON_AGAIN) buttons[i]->visible = TRUE;
			else buttons[i]->visible = FALSE;
		}
		currentStage->isWAR = FALSE;
		return;
	}


	for(int i=0;i<units.size();i++){
		Unit* unit = units[i];
		if (unit->health <= 0) {
			unit->visible = FALSE;
			continue;
		}
		//根据单位类型选择行为函数
		switch(unit->type){
			case UNIT_TYPE_REAPER:
				UnitBehaviour_1(unit);
				break;
			case UNIT_TYPE_CASTER:
				UnitBehaviour_2(unit);
				break;
			case UNIT_TYPE_SABER:
				UnitBehaviour_3(unit);
				break;
			case UNIT_TYPE_SHIELDER:
				UnitBehaviour_4(unit);
				break;
			case UNIT_TYPE_HOPLITE:
				UnitBehaviour_5(unit);
				break;
		}
	}

}

//单位行为函数

//收割者
void UnitBehaviour_1(Unit* unit){
	
	// 发现敌方最近的人
	double dirX, dirY,dirLen;
	Unit* enemy;
	tie(dirX, dirY,dirLen, enemy) = FindNearestEnemy(unit);
	if(dirX>0){
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else{
		unit->direction = UNIT_DIRECT_LEFT;
	}

	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch(unit->state){
		case UNIT_STATE_HOLD:
			if(dirLen<PM[unit->type]->attackArea){
				next_state = UNIT_STATE_ATTACK;
			}
			else {
				next_state = UNIT_STATE_WALK;
			}
			break;
		case UNIT_STATE_WALK:
			if(dirLen <= PM[unit->type]->attackArea){
				next_state = UNIT_STATE_ATTACK;
			}
			break;
		case UNIT_STATE_ATTACK:
			if(dirLen > PM[unit->type]->attackArea){
				next_state = UNIT_STATE_WALK;
			}
			else {
				enemy->health -= PM[unit->type]->attack;
			}

			break;
	};
	ChangeState(unit, next_state, dirX, dirY, dirLen);
}

void UnitBehaviour_2(Unit* unit) {

	// 发现敌方最近的人
	double dirX, dirY, dirLen;
	Unit* enemy;
	tie(dirX, dirY, dirLen, enemy) = FindNearestEnemy(unit);
	if (dirX > 0) {
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else {
		unit->direction = UNIT_DIRECT_LEFT;
	}

	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch (unit->state) {
	case UNIT_STATE_HOLD:
		if (dirLen < PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		else {
			next_state = UNIT_STATE_WALK;
		}
		break;
	case UNIT_STATE_WALK:
		if (dirLen <= PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		break;
	case UNIT_STATE_ATTACK:
		if (dirLen > PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_WALK;
		}
		else {
			enemy->health -= PM[unit->type]->attack;
		}
		break;
	};
	ChangeState(unit, next_state, dirX, dirY, dirLen);
}

void UnitBehaviour_3(Unit* unit) {

	// 发现敌方最近的人
	double dirX, dirY, dirLen;
	Unit* enemy;
	tie(dirX, dirY, dirLen, enemy) = FindNearestEnemy(unit);
	if (dirX > 0) {
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else {
		unit->direction = UNIT_DIRECT_LEFT;
	}

	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch (unit->state) {
	case UNIT_STATE_HOLD:
		if (dirLen < PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		else {
			next_state = UNIT_STATE_WALK;
		}
		break;
	case UNIT_STATE_WALK:
		if (dirLen <= PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		break;
	case UNIT_STATE_ATTACK:
		if (dirLen > PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_WALK;
		}
		else {
			enemy->health -= PM[unit->type]->attack;
		}
		break;
	};
	ChangeState(unit, next_state, dirX, dirY, dirLen);
}

void UnitBehaviour_4(Unit* unit) {

	// 发现敌方最近的人
	double dirX, dirY, dirLen;
	Unit* enemy;
	tie(dirX, dirY, dirLen, enemy) = FindNearestEnemy(unit);
	if (dirX > 0) {
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else {
		unit->direction = UNIT_DIRECT_LEFT;
	}

	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch (unit->state) {
	case UNIT_STATE_HOLD:
		if (dirLen < PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		else {
			next_state = UNIT_STATE_WALK;
		}
		break;
	case UNIT_STATE_WALK:
		if (dirLen <= PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_ATTACK;
		}
		break;
	case UNIT_STATE_ATTACK:
		if (dirLen > PM[unit->type]->attackArea) {
			next_state = UNIT_STATE_WALK;
		}
		else {
			enemy->health -= PM[unit->type]->attack;
		}
		break;
	};
	ChangeState(unit, next_state, dirX, dirY, dirLen);
}

void UnitBehaviour_5(Unit* unit){
	// 发现敌方最近的人
	double dirX, dirY, dirLen;
	Unit* enemy;
	tie(dirX, dirY, dirLen, enemy) = FindNearestEnemy(unit);
	if (dirX > 0) {
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else {
		unit->direction = UNIT_DIRECT_LEFT;
	}

	if(dirX>0){
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else{
		unit->direction = UNIT_DIRECT_LEFT;
	}


	//判断当前状态, 以及判断是否需要状态变化
	int next_state = unit->state;
	switch(unit->state){
		case UNIT_STATE_HOLD:
			next_state = UNIT_STATE_WALK;
			break;
		case UNIT_STATE_WALK:
			if(dirLen < 32){
				next_state = UNIT_STATE_ATTACK;
			}else{
				unit->vx = dirX / dirLen * PM[unit->type]->speed;
				unit->vy = dirY / dirLen * PM[unit->type]->speed;
			}
			break;
		case UNIT_STATE_ATTACK:
			if(dirLen >= 32){
				next_state = UNIT_STATE_WALK;
			}
			break;
	};
	ChangeState(unit, next_state, dirX, dirY, dirLen);
}

void ChangeState(Unit* unit, int next_state,double dirX,double dirY,double dirLen) {
	if (next_state != unit->state) {
		//状态变化
		unit->state = next_state;
		unit->frame_id = -1;

		switch (unit->state) {
		case UNIT_STATE_HOLD:
			unit->frame_sequence = FRAMES_HOLD;
			unit->frame_count = FRAMES_HOLD_COUNT;
			unit->vx = 0;
			unit->vy = 0;
			break;
		case UNIT_STATE_WALK:
			unit->frame_sequence = FRAMES_WALK;
			unit->frame_count = FRAMES_WALK_COUNT;
			unit->vx = dirX / dirLen * PM[unit->type]->speed;
			unit->vy = dirY / dirLen * PM[unit->type]->speed;
			break;
		case UNIT_STATE_ATTACK:
			unit->frame_sequence = FRAMES_ATTACK;
			unit->frame_count = FRAMES_ATTACK_COUNT;
			unit->vx = 0;
			unit->vy = 0;
			break;
		};
	}
	else {
		switch (unit->state) {
		case UNIT_STATE_WALK:
			unit->vx = dirX / dirLen * PM[unit->type]->speed;
			unit->vy = dirY / dirLen * PM[unit->type]->speed;
			break;
		}
	}
	//动画运行到下一帧
	unit->x += unit->vx;
	unit->y += unit->vy;

	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;

	int column = unit->frame_sequence[unit->frame_id];
	unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);
}

tuple<double, double,double,Unit*> FindNearestEnemy(Unit* unit) {
	Unit* enemy;
	double dirLen = INT_MAX;
	double dirX = 0;
	double dirY = 0;
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->visible==TRUE && units[i]->side != unit->side && sqrt(pow(unit->x - units[i]->x, 2) + pow(unit->y - units[i]->y, 2)) < dirLen) {
			dirLen = sqrt(pow(unit->x - units[i]->x, 2) + pow(unit->y - units[i]->y, 2));
			dirX = units[i]->x- unit->x;
			dirY = units[i]->y- unit->y;
			enemy = units[i];
		}
	}
	return make_tuple(dirX, dirY, dirLen,enemy);
}

void ArrangeDeployUnits() {
	if (currentStage->stageID == STAGE_VERSUS) {
		for (int i = 0; i < deployUnits.size(); i++) {
			deployUnits[i]->x = (double(WINDOW_WIDTH) - 10 * double(UNIT_SIZE_X)) / 2 + i * double(UNIT_SIZE_X);
			deployUnits[i]->y = double(WINDOW_HEIGHT) - double(UNIT_SIZE_Y);
			deployUnits[i]->visible = TRUE;
		}
	}
	else {
		for (int i = 0; i < deployUnits.size(); i++) {
			deployUnits[i]->x = 4 * double(UNIT_SIZE_X)+ (double(WINDOW_WIDTH) - 10 * double(UNIT_SIZE_X)) / 2 + i * double(UNIT_SIZE_X);
			if (deployUnits[i]->side == UNIT_SIDE_RED) deployUnits[i]->visible = FALSE;
			else deployUnits[i]->visible = TRUE;
		}
	}
}
#pragma endregion

// 绘图函数
#pragma region 
// 绘图函数
void Paint(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 绘制背景到缓存
	//SelectObject(hdc_loadBmp, currentStage->bg);
	SelectObject(hdc_loadBmp, bmp_Background);

	BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {

	}
	if ((currentStage->stageID >= STAGE_1 && currentStage->stageID < STAGE_HELP)) //TODO：添加多个游戏场景
	{
		// 绘制单位到缓存
		for(int i=0;i<units.size();i++){
			Unit* unit = units[i];
			if (unit->visible == TRUE) {
				SelectObject(hdc_loadBmp, unit->img);
				TransparentBlt(
					hdc_memBuffer, unit->x - 0.5 * UNIT_SIZE_X, unit->y - 0.5 * UNIT_SIZE_Y,
					UNIT_SIZE_X, UNIT_SIZE_Y,
					hdc_loadBmp, UNIT_SIZE_X * unit->frame_column, UNIT_SIZE_Y * unit->frame_row, UNIT_SIZE_X, UNIT_SIZE_Y,
					RGB(255, 255, 255)
				);
			}
		}
		//绘制部署单位到缓存
		for (int i = 0; i < deployUnits.size(); i++) {
			Unit* unit = deployUnits[i];
			if (unit->visible == TRUE) {
				SelectObject(hdc_loadBmp, unit->img);
				TransparentBlt(
					hdc_memBuffer, unit->x - 0.5 * UNIT_SIZE_X, unit->y - 0.5 * UNIT_SIZE_Y,
					UNIT_SIZE_X, UNIT_SIZE_Y,
					hdc_loadBmp, UNIT_SIZE_X * unit->frame_column, UNIT_SIZE_Y * unit->frame_row, UNIT_SIZE_X, UNIT_SIZE_Y,
					RGB(255, 255, 255)
				);
			}
		}	
		
	}

	// 绘制按钮到缓存
	for(int i=0;i<buttons.size();i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}
	
	
	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}



// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src){

	srand(time(NULL));
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	//加载资源
	SelectObject(hdc_loadBmp, bmp_src);


	for(int i=0;i<BG_ROWS;i++){

		int src_row = 1;
		if(i >= BG_ROWS_SKY)
			src_row = 0;

		for(int j=0;j<BG_COLUMNS;j++){
			int src_column = rand() % BG_SRC_COUNT;

			TransparentBlt(
				hdc_memBuffer, j*BG_CELL_WIDTH, i*BG_CELL_HEIGHT,
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				hdc_loadBmp, src_column * BG_CELL_WIDTH, src_row * BG_CELL_HEIGHT, 
				BG_CELL_WIDTH, BG_CELL_HEIGHT,
				RGB(255, 255, 255)
			);

		}
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);

	return bmp_output;
}

#pragma endregion