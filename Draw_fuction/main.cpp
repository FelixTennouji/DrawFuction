//Hello ,Sion.
//Good night ,Sion.
//设定区

#include <math.h>//#define M_E   2.71828182845904523536   #define M_PI  3.14159265358979323846
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <easyx.h>
#define _BACKGROUND_COLOR    WHITE//背景色设置
#define _TEXT_COLOR          BLACK
//全局变量

//函数定义
void start_menu();
//函数声明
void start_menu()
{
	
	initgraph(900,600);
	setbkcolor(_BACKGROUND_COLOR);
	cleardevice();//用背景色清空屏幕
	settextstyle(30, 0, _T("黑体"));
	settextcolor(_TEXT_COLOR);
	outtextxy(350, 0,_T("函数图绘制程序"));
	settextstyle(16, 0, _T("宋体"));
	settextcolor(_TEXT_COLOR);
	outtextxy(50, 100,   _T("1.本程序支持加减乘除四则混合运算【 +-* / 】，支持使用括号【 () 】，支持使用取整，取模运算 【\\  %】"));//取整运算符必须使用\\ 才能表现出来    
	outtextxy(30, 120,  _T("支持高次幂，平方运算【x^n   sqr()】,三角函数【sin(ax),cos(ax),tan(ax)】"));
	outtextxy(30, 140, _T("指数运算【e^x 或 exp（）】，对数运算【log（ax）或ln() 与 lg（）  】"));
	outtextxy(50, 200,  _T("2.本程序绘制出图像可自动判断值域，并进行缩放。具有通过鼠标滚轮缩放，拖动功能"));
	outtextxy(50, 260,  _T("3.本程序可自定义精度进行积分，并且具有初步报错功能，不必在意大小写，但禁止中文字符哦"));
	settextstyle(24, 0, _T("宋体"));
	outtextxy(330, 400, _T("ヽ(*ﾟдﾟ)ノ　"));
	outtextxy(360, 480,  _T("按任意字母键继续"));

	getch();
	closegraph();		

}

int main(void)
{
	start_menu();
	return 0;
}