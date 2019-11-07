// WarGame.cpp : 定义应用程序的入口点。
//

#include "WarGame.h"

using namespace std;



#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名



HBITMAP bmp_Grass;			//背景图像资源
HBITMAP bmp_StartButton;	//开始按钮图像资源
HBITMAP bmp_Unit_Red;		//红方主角图像资源
HBITMAP bmp_Unit_Blue;		//蓝方主角图像资源

HBITMAP bmp_Background;		//生成的背景图像



Stage* currentStage = NULL; //当前场景状态
vector<Unit*> units;		//单位
vector<Button*> buttons;	//按钮

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;


//帧
int FRAMES_HOLD[] = {0};
int FRAMES_HOLD_COUNT = 1;
int FRAMES_WALK[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2};
int FRAMES_WALK_COUNT = 16;
int FRAMES_ATTACK[] = {3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int FRAMES_ATTACK_COUNT = 24;



// TODO: 在此添加其它全局变量





double const PI = acos(double(-1));


#pragma endregion


#pragma region Win32程序框架



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


#pragma region 事件处理函数

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam) 
{
	//加载图像资源
	bmp_Grass = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_GRASS));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	bmp_Unit_Red = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_RED));
	bmp_Unit_Blue = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_BLUE));

	//添加按钮

	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT, 
		(WINDOW_WIDTH - BUTTON_STARTGAME_WIDTH)/2, (WINDOW_WIDTH - BUTTON_STARTGAME_HEIGHT) / 2);
	buttons.push_back(startButton);

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
	default:
		break;
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
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

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
				case BUTTON_STARTGAME:
				{
					//TODO：判断进入哪个关卡
					InitStage(hWnd, STAGE_1);
				}
				break;
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
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	UpdateUnits(hWnd);

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


#pragma endregion


#pragma region 其它游戏状态处理函数



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
Unit* CreateUnit(int side, int type, int x, int y, int health)
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
	unit->health = health;
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
		currentStage->bg = bmp_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//显示开始界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTGAME) 
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}


	}
	else if(stageID >= STAGE_1 && stageID <= STAGE_1) //TODO：添加多个游戏场景
	{
		currentStage->bg = bmp_Grass;
		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//显示游戏界面的按钮
		for(int i=0;i<buttons.size();i++)
		{
			Button* button = buttons[i];
			if (false) //TODO：加载游戏界面需要的按钮
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
				units.push_back(CreateUnit(UNIT_SIDE_RED, UNIT_TYPE_CASTER, 200, 200, 100));
				units.push_back(CreateUnit(UNIT_SIDE_BLUE, UNIT_TYPE_HOPLITE, 600, 200, 100));
				break;
			default:
				break;
		}


	}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


// 刷新单位状态函数
void UpdateUnits(HWND hWnd)
{
	for(int i=0;i<units.size();i++){
		Unit* unit = units[i];

		//根据单位类型选择行为函数
		switch(unit->type){
			case UNIT_TYPE_CASTER:
				UnitBehaviour_1(unit);
				break;
			case UNIT_TYPE_REAPER:
			case UNIT_TYPE_SABER:
			case UNIT_TYPE_SHIELDER:
			case UNIT_TYPE_HOPLITE:
				UnitBehaviour_2(unit);
				
				break;
		}



	}


}

//单位行为函数
void UnitBehaviour_1(Unit* unit){
	
	double dirX = mouseX - unit->x;
	double dirY = mouseY - unit->y;
	double dirLen = sqrt(dirX * dirX + dirY * dirY) + 0.0001;


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
			if(dirLen<400){
				next_state = UNIT_STATE_WALK;
			}
			break;
		case UNIT_STATE_WALK:
			if(dirLen >= 400){
				next_state = UNIT_STATE_HOLD;
			}else if(dirLen < 200){
				next_state = UNIT_STATE_ATTACK;
			}else{
				unit->vx = dirX / dirLen * UNIT_SPEED;
				unit->vy = dirY / dirLen * UNIT_SPEED;
			}
			break;
		case UNIT_STATE_ATTACK:
			if(dirLen >= 200){
				next_state = UNIT_STATE_WALK;
			}
			break;
	};

	if(next_state != unit->state){
		//状态变化
		unit->state = next_state;
		unit->frame_id = -1;

		switch(unit->state){
			case UNIT_STATE_HOLD:
				unit->frame_sequence = FRAMES_HOLD;
				unit->frame_count = FRAMES_HOLD_COUNT;
				unit->vx=0;
				unit->vy=0;
				break;
			case UNIT_STATE_WALK:
				unit->frame_sequence = FRAMES_WALK;
				unit->frame_count = FRAMES_WALK_COUNT;
				unit->vx = dirX / dirLen * UNIT_SPEED;
				unit->vy = dirY / dirLen * UNIT_SPEED;
				break;
			case UNIT_STATE_ATTACK:
				unit->frame_sequence = FRAMES_ATTACK;
				unit->frame_count = FRAMES_ATTACK_COUNT;
				unit->vx=0;
				unit->vy=0;
				break;
		};
	}

	//动画运行到下一帧
	unit->x += unit->vx;
	unit->y += unit->vy;

	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;

	int column = unit->frame_sequence[unit->frame_id];
	unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);


}

void UnitBehaviour_2(Unit* unit){
	
	double dirX = mouseX - unit->x;
	double dirY = mouseY - unit->y;
	double dirLen = sqrt(dirX * dirX + dirY * dirY) + 0.0001;


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
				unit->vx = dirX / dirLen * UNIT_SPEED;
				unit->vy = dirY / dirLen * UNIT_SPEED;
			}
			break;
		case UNIT_STATE_ATTACK:
			if(dirLen >= 32){
				next_state = UNIT_STATE_WALK;
			}
			break;
	};

	if(next_state != unit->state){
		//状态变化
		unit->state = next_state;
		unit->frame_id = -1;

		switch(unit->state){
			case UNIT_STATE_HOLD:
				unit->frame_sequence = FRAMES_HOLD;
				unit->frame_count = FRAMES_HOLD_COUNT;
				unit->vx=0;
				unit->vy=0;
				break;
			case UNIT_STATE_WALK:
				unit->frame_sequence = FRAMES_WALK;
				unit->frame_count = FRAMES_WALK_COUNT;
				unit->vx = dirX / dirLen * UNIT_SPEED;
				unit->vy = dirY / dirLen * UNIT_SPEED;
				break;
			case UNIT_STATE_ATTACK:
				unit->frame_sequence = FRAMES_ATTACK;
				unit->frame_count = FRAMES_ATTACK_COUNT;
				unit->vx=0;
				unit->vy=0;
				break;
		};
	}

	//动画运行到下一帧
	unit->x += unit->vx;
	unit->y += unit->vy;

	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;

	int column = unit->frame_sequence[unit->frame_id];
	unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);


}



#pragma endregion


#pragma region 绘图函数
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
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) //TODO：添加多个游戏场景
	{
		// 绘制单位到缓存
		for(int i=0;i<units.size();i++){
			Unit* unit = units[i];
			SelectObject(hdc_loadBmp, unit->img);
			TransparentBlt(
			hdc_memBuffer, unit->x - 0.5 * UNIT_SIZE_X, unit->y - 0.5 * UNIT_SIZE_Y,
			UNIT_SIZE_X, UNIT_SIZE_Y,
			hdc_loadBmp, UNIT_SIZE_X * unit->frame_column, UNIT_SIZE_Y * unit->frame_row, UNIT_SIZE_X, UNIT_SIZE_Y,
			RGB(255, 255, 255)
		);
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