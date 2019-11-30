#pragma once

//头文件引用
#pragma region 

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")


// TODO:  在此处引用程序需要的其他头文件
#include <vector>
#include <math.h>
#include<map>
#include<tuple>
#include<string>
#include<atlstr.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#pragma endregion


//宏定义
#pragma region 

#define WINDOW_TITLEBARHEIGHT	32			//标题栏高度
#define WINDOW_WIDTH			1024		//游戏窗口宽度
#define WINDOW_HEIGHT			768			//游戏窗口高度

#define IMG_HELP1_WIDTH        708     //帮助页面1宽度
#define IMG_HELP1_HEIGHT       191      //帮助页面1高度

#define IMG_HELP2_WIDTH        708      //帮助页面2宽度
#define IMG_HELP2_HEIGHT       488      //帮助页面2高度


#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_2                 2       //第二个游戏场景的ID
#define STAGE_3                 3       //第三个游戏场景的ID
#define STAGE_4                 4       //第四个游戏场景的ID
#define STAGE_5                 5       //第五个游戏场景的ID
#define STAGE_VERSUS            1000    //对战游戏场景
#define STAGE_HELP              10000    //帮助场景



#define UNIT_SIZE_X				64		//单位的宽度
#define UNIT_SIZE_Y				64		//单位的高度
#define UNIT_LAST_FRAME			11		//单位的动画帧最大值

//单位阵营定义
#define UNIT_SIDE_RED			10000	//红方
#define UNIT_SIDE_BLUE			10001	//蓝方


//单位类型定义
#define UNIT_TYPE_REAPER		0		//收割者
#define UNIT_TYPE_CASTER		1		//魔术师
#define UNIT_TYPE_SABER			2		//剑士
#define UNIT_TYPE_SHIELDER		3		//盾卫
#define UNIT_TYPE_HOPLITE		4		//重装步兵

//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走
#define UNIT_STATE_ATTACK		2		//攻击

//单位方向定义
#define UNIT_DIRECT_RIGHT		0		//向右
#define UNIT_DIRECT_LEFT		1		//向左


//背景
#define BG_SRC_COUNT			6		//背景资源数量
#define BG_COLUMNS				16		//背景列数
#define BG_ROWS					12		//背景行数
#define BG_ROWS_SKY				3		//背景天空行数
#define BG_ROWS_LAND			9		//背景地面行数
#define BG_CELL_WIDTH			64		//背景单格宽度
#define BG_CELL_HEIGHT			64		//背景单格高度

//按钮
#define BUTTON_ADVENTUREMODE		1000    //冒险模式按钮ID
#define BUTTON_ADVENTUREMODE_WIDTH   212    //冒险模式按钮宽度
#define BUTTON_ADVENTUREMODE_HEIGHT   76    //冒险模式按钮高度

#define BUTTON_VERSUSMODE           1001    //对战模式按钮ID
#define BUTTON_VERSUSMODE_WIDTH      212    //对战模式按钮宽度
#define BUTTON_VERSUSMODE_HEIGHT      76    //对战模式按钮高度

#define	BUTTON_HELP                 1002    //帮助按钮ID
#define BUTTON_HELP_WIDTH            212    //帮助按钮宽度
#define BUTTON_HELP_HEIGHT            76    //帮助按钮高度

#define BUTTON_STARTWAR		        1003	//开始游戏按钮ID
#define BUTTON_STARTWAR_WIDTH	 	 159	//开始游戏按钮宽度
#define BUTTON_STARTWAR_HEIGHT		  51	//开始游戏按钮高度

#define BUTTON_EXIT                 1004    //退出游戏按钮
#define BUTTON_EXIT_WIDTH            159    //退出游戏按钮宽度
#define BUTTON_EXIT_HEIGHT            51    //退出游戏按钮高度

#define BUTTON_NEXT                 1005    //下一关按钮ID
#define BUTTON_NEXT_WIDTH             90    //下一关按钮宽度
#define BUTTON_NEXT_HEIGHT            39   //下一关按钮高度

#define BUTTON_AGAIN                1006    //重试按钮ID
#define BUTTON_AGAIN_WIDTH            90    //重试按钮宽度
#define	BUTTON_AGAIN_HEIGHT           39    //重试按钮高度

#define BUTTON_VICTORY              1007    //显示胜利图标
#define BUTTON_VICTORY_WIDTH         212   
#define BUTTON_VICTORY_HEIGHT         76    

#define BUTTON_DEFEAT               1008    //显示失败图标
#define BUTTON_DEFEAT_WIDTH          212    
#define BUTTON_DEFEAT_HEIGHT          76

#define BUTTON_BLUEMONEY                1009    // 蓝方金币按钮
#define BUTTON_BLUEMONEY_WIDTH            77    
#define BUTTON_BLUEMONEY_HEIGHT           77

#define BUTTON_REDMONEY                1010    // 红方金币按钮
#define BUTTON_REDMONEY_WIDTH            77    
#define BUTTON_REDMONEY_HEIGHT           77

#define BUTTON_TRASH                     1011
#define BUTTON_TRASH_WIDTH                64
#define BUTTON_TRASH_HEIGHT               64

//计时器
#define TIMER_GAMETIMER				1		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）
	bool isWAR=FALSE;   //是否正在自动战斗
};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 技能结构体
struct Skill
{
	int frameLeft;     //技能持续帧数
	int frameCD;       //技能CD，小于等于零时发动技能
	int attackGain;    //攻击力增益
	int defenseGain;   //防御力增益
	int speedGain;     //速度增益
};

struct SkillIcon
{
	HBITMAP bmp;
	int width;
	int height;
};

// 单位结构体
struct Unit
{
	HBITMAP img;	//图片

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;		//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几个


	int side;		//单位阵营
	int type;		//单位类型
	int state;		//单位状态
	int direction;	//单位方向

	double x;			//坐标x
	double y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	double health;     //血量
	bool visible;   //血量为0时死亡，不显示
	Skill* skill;   //单位技能
};

// 单位属性结构体
struct Property
{
	int speed;      //速度
	int health;     //生命值
	int attack;     //攻击力
	int defense;    //防御力
	int attackArea;       //攻击范围
	int cost;          //花费
	SkillIcon* skillicon;  //技能图标
	Skill* skill; //技能
	bool isLock;   //是否解锁
	CString name; //角色名字
};

// 图标结构体 图片资源
struct Icon
{
	int frameLeft;    //图标显示帧数 默认8帧
	int x;            //图标中心点横坐标
	int y;            //图标中心点纵坐标
	int width; 
	int height;
	HBITMAP bmp;
};


#pragma endregion

//事件处理函数声明
#pragma region 


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

#pragma endregion


//其它游戏状态处理函数声明
#pragma region 

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加单位函数
Unit* CreateUnit(int side, int type, int x, int y);

// 添加图标函数
Icon* CreateIcon(HBITMAP img,int frameLeft, int x, int y,int width,int height);



// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 刷新单位状态函数
void UpdateUnits(HWND hWnd);



//单位行为函数
void UnitBehaviour_1(Unit* unit); //收割者行为
void UnitBehaviour_2(Unit* unit); //魔术师行为
void UnitBehaviour_3(Unit* unit); //剑士行为
void UnitBehaviour_4(Unit* unit); //盾卫行为
void UnitBehaviour_5(Unit* unit); //重装步兵行为
void ChangeState(Unit* unit, int next_state,double dirX,double dirY,double dirLen); //改变单位状态
std::tuple<double,double,double,Unit*> FindNearestEnemy(Unit* unit); //发现最近地方单位
void ArrangeDeployUnits(); //根据模式改变部署单位位置
int JudgeState(Unit* unit,Unit* enemy,double dirLen); //判断当前状态并更改
void JudgeAttack(Unit* u, Unit* enemy);
void Attack(Unit* u, Unit* enemy);

// 按键控制函数
void Pause();  //按下ESC键可使游戏暂停

#pragma endregion

//绘图函数声明
#pragma region 

// 绘图函数
void Paint(HWND hWnd);

// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

#pragma endregion