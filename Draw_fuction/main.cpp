//Hello ,Sion.
//Good night ,Sion.
//设定区
#define _BACKGROUND_COLOR    WHITE//背景色设置
#define _TEXT_COLOR          BLACK//字体颜色
/*调度场算法
逆波兰表达式
一、 将中缀表达式转换成后缀表达式算法：

1、从左至右扫描 中缀表达式。
2、若读取的是操作数（数字），则判断该操作数的类型，并将该操作数存入操作数堆栈
3、若读取的是运算符
(1) 该运算符为左括号"("，则直接存入运算符堆栈。
(2) 该运算符为右括号")"，则输出运算符堆栈中的运算符到操作数堆栈，直到遇到左括号为止。
(3) 该运算符为非括号运算符：
(a) 若运算符堆栈栈顶的运算符为括号，则直接存入运算符堆栈。
(b) 若比运算符堆栈栈顶的运算符优先级高或相等，则直接存入运算符堆栈。
(c) 若比运算符堆栈栈顶的运算符优先级低，则输出栈顶运算符到操作数堆栈，并将当前运算符压入运算符堆栈。
4、当表达式读取完成后运算符堆栈中尚有运算符时，则依序取出运算符到操作数堆栈，直到运算符堆栈为空。
 
二、逆波兰表达式求值算法：
1、循环扫描语法单元的项目。
2、如果扫描的项目是操作数，则将其压入操作数堆栈，并扫描下一个项目。
3、如果扫描的项目是一个二元运算符，则对栈的顶上两个操作数执行该运算。

4、如果扫描的项目是一个一元运算符，则对栈的最顶上操作数执行该运算。
5、将运算结果重新压入堆栈。
6、重复步骤2-5，全部运算完后，最后堆栈中数 即为结果值。
 */
#include <math.h>//#define M_E   2.71828182845904523536   #define M_PI  3.14159265358979323846
#include <Windows.h>
#include <conio.h>//getch()
#include <stdlib.h>
#include <easyx.h>
#include <string>
#include <string.h>//extern void *memset(void *buffer, int c, int count)  

//全局变量
static wchar_t infix_expression[128];//中值表达式
struct {
	int IsNull;
	double Max;
	double Min;
} _domain;//定义域
//函数声明
void start_menu();//开始界面渲染
void Initialization_Data();//数据初始化， 防止上一轮数据影响此次数据
void Data_INPUT();//数据录入
void Data_process();

//函数定义

//数据初始化， 防止上一轮数据影响此次数据
void Initialization_Data()
{
	memset(infix_expression, 0, sizeof(infix_expression));
}//利用memset函数进行快速初始化
void Data_process()
{

}
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
	outtextxy(50, 100, _T("1.本程序支持加减乘除四则混合运算【 +-* / 】，支持使用括号【 () 】，不支持使用正负号"));
	outtextxy(30, 120,  _T("支持高次幂，平方运算【x^n   sqr()】,三角函数【sin(ax),cos(ax),tan(ax)】"));
	outtextxy(30, 140, _T("指数运算【e^x 】，对数运算【log（ax）或ln() 与 lg（）  】"));
	outtextxy(50, 200,  _T("2.本程序绘制出图像具有通过鼠标滚轮缩放，拖动功能"));
	outtextxy(50, 260,  _T("3.本程序可自定义精度进行积分，并且具有初步报错功能，不必在意大小写，但禁止中文字符哦"));
	outtextxy(50, 320, _T("4.由于数据精度问题，小数点后最多允许5位数字"));
	settextstyle(24, 0, _T("宋体"));
	outtextxy(360, 480,  _T("按任意字母键继续"));

	getch();
	cleardevice();

}
//数据录入
void Data_INPUT()
{
	
	wchar_t a1[16], b1[16];
	InputBox(infix_expression,129,TEXT("请输入函数表达式（中值表达式，非逆波兰），未知量用x代替\n最多128个字符，禁止中文\n\n\n f(x)="),TEXT("INPUT"),TEXT("x+1"),600,0,true);
	if (MessageBox(NULL, TEXT("是否给定定义域【计算积分定义域之后会再次询问】"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_YESNO) == IDYES)
	{
		_domain.IsNull = 1;
		InputBox(a1, 15, TEXT("请输入定义域上限"), TEXT("ヽ(*ﾟдﾟ)ノ"), TEXT(""), 200, 0, true);
		_domain.Max = std::stod(a1);
		InputBox(a1, 15, TEXT("请输入定义域下限"), TEXT("ヽ(*ﾟдﾟ)ノ"), TEXT(""), 200, 0, true);
		_domain.Min = std::stod(a1);
	}
	
	else
		_domain.IsNull = 0;
}

int main(void)
{
	start_menu();
	do
	{
		Initialization_Data();//数据初始化， 防止上一轮数据影响此次数据
		Data_INPUT();//数据录入
		Data_process;

	} while (MessageBox(NULL, TEXT("是否重新绘制图像"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_YESNO)==IDYES);
	//利用messagebox进行对话操作  MB_ICONINFORMATION 意思是显示 i 图标  MB_SYSTEMMODAL 意思是设置对话框在所有窗口最前端 MB_YESNO代表按钮为是否类型
	closegraph();
	exit (0);
	return 0;
}