//Hello ,Sion.
//Good night ,Sion.
#define _BACKGROUND_COLOR    0x444444//背景色设置
#define _TEXT_COLOR          0xffffff//xy文字
#define _XY_COLOR            0xb0f8ff//xy轴颜色
#define _GRID_COLOR          0x00fff0//网格颜色
#define _BUTTON_COLOR        0x00a552//按钮颜色
#define _TEXT0_COLOR         0x0096ff//右边文字0
#define _LINE_COLOR          0xb86bff//线条颜色
#define _OTHER_COLOR         0x706d72//副区块颜色
#define _TEXT2_COLOR         0xffd200//右边文字1
#define _TEXT3_COLOR         0xff3c00//右边文字2
#define _LINE_WIDTH          2

#include <math.h>//#define M_E   2.71828182845904523536   #define M_PI  3.14159265358979323846
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <easyx.h>
#include <string>
#include <cstdlib>
#include <wchar.h>
#include <cctype>
#include <string.h>//extern void *memset(void *buffer, int c, int count)  



//全局变量与结构体



static wchar_t infix_expression[128];//未处理的中值表达式子
struct
{
	long long int x;//当前中心点在窗口上的坐标
	long long int y;
	short shrink;//放缩    0~10  x: 0.25~1024  y:0.1875-768
}_attributes;//绘制图像的属性
struct {
	int IsNull;
	double Max;
	double Min;
} _domain;//定义域
typedef struct ___EXPRESSION
{
	short exist;//是否存在
	short prority;
	/*
	-1 未知变量
	0 浮点数//补充一句 我终于TM明白c语言优先级是干啥的！！！
	1 + -（双目运算符）
	2 乘除
	3 ^
	4 sin cos tan  log lg ln sqr
	5 （）
	*/
	double num;
	//+ - * / ^  sin cos tan log{ln}  lg  sqr (    )
	//0 1 2 3 4  5   6    7   8       9   10  11   12  

}_Expression	;
struct
{
	short y;//是否有故障  0 无 1括号不匹配 2 未知字符 3 高次幂非正整常数
	short k;//括号是否匹配
	int n;   //从第几个字符开始出现问题

}M_error;
typedef struct __RESULT
{
	
	double x;
	short exist;
	double y;
	
}_point;
_Expression infix[128];//中缀表达式
_Expression postfix[128];//逆波兰表达式
double ratio;



//函数声明



void start_menu();//开始界面渲染
void Initialization_Data();//数据初始化， 防止上一轮数据影响此次数据
void Data_INPUT();//数据录入
int  Data_process(void);
void check();
void wcharTOinfix();
void infixTOpostfix();
void draw();
void calculus(void);
int USERerror();//只能报一个错误
_point postfix_operation(double x);
double _power(double x, double n);
void fuctionANDxyDRAW();
void xyDRAW();
void fuctionDRAW();


//函数定义


void calculus(void)//这个函数从全局变量读入，因此不需要形参，也没有返回值
{
	wchar_t max_char[16];//因为我们接下来要调用的是unicode字符串，而char类型只支持0 -128的ascii码，所以必须要调用宽字符来支持
	wchar_t min_char[16];
	wchar_t definite_integral_char[64];//definite_integral_char翻译成中文就是 定积分 字符
	_point r;
	double i;
	double max, min;
	double differential;
	double definite_integral=0;//该变量用于累加积分，definite_integral中文意思是定积分
	InputBox(max_char, 16, TEXT("定积分上限是"), TEXT("请输入定积分上限"), TEXT("0"), 200, 0, true);
	//inputbox意思是弹出一个可供用户输入的对话框，接受用户输入的字符并将其转换为宽字符串并传给max_char这个宽字符数组的指针，16 意思是我最多接受16个宽字符 ，TEXT（）是一个 可以接受参数的宏，它能够将里面的字符强制转换为unicode字符编码而不是GBK字符编码
	//如果unicode GBK 不知道说什么，请自己百度
	max = std::stod(max_char);
	//std::stod()你们可以你接我一种比较特殊的函数，我们先不讨论两个冒号连一起什么意思。暂时理解成一种函数，可以将传入的宽字符串转换为双精度浮点数
	InputBox(min_char, 16, TEXT("定积分下限是"), TEXT("请输入定积分下限"), TEXT("0"), 200, 0, true);
	min = std::stod(min_char);
	InputBox(min_char, 16, TEXT("定积分精度是"), TEXT(""), TEXT("0.0001"), 200, 0, true);
	differential= std::stod(min_char);
	if (max > min)
	{
		for (i = min; i <= max; i +=differential )
		{
			r = postfix_operation(i);
			definite_integral = definite_integral + r.y * differential;
		}
	}
	else if (min > max)
	{
		for (i = max; i <= min; i += differential)
		{
			r = postfix_operation(i);
			definite_integral = definite_integral + r.y * differential;
		}
		definite_integral = -definite_integral;

	}
	else
		definite_integral = 0;
	swprintf(definite_integral_char, L"您计算的定积分为：%lf", definite_integral);
	//swprintf   这个函数名可以拆分成 s+w+printf 
	//s的运算是我不往屏幕上输出，该项程序员提供的字符数组的地址输出
	//w意思是我处理的字符串全部是宽字符
	//printf不解释
	MessageBox(NULL, definite_integral_char, TEXT("积分"), MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_YESNO);
	//messagebox 意思是弹出一个消息框 NULL代表它只是一个弹窗而不是一个窗口 
	//, definite_integral_char,是我提供给他的弹窗正文的宽字符串指针
	//TEXT("积分")是我提供给他的标题
	// MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_YESNO  代表弹窗含有一个i图标 | 置于所有Windows窗口最前 |拥有yes和 no 两个按钮
}
void fuctionANDxyDRAW()
{
	wchar_t  a[16];
	BeginBatchDraw();
	setfillcolor(_BACKGROUND_COLOR);
	solidrectangle(0,0,800,600);
	
	xyDRAW();
	fuctionDRAW();
	setfillcolor(_OTHER_COLOR);
	solidrectangle(800, 0, 900, 600);
	setfillcolor(_BUTTON_COLOR);
	solidrectangle(800, 550, 900, 600);
	settextcolor(_TEXT0_COLOR);
	settextstyle(20, 0, _T("黑体"));
	setbkmode(TRANSPARENT);
	outtextxy(815, 570, _T("计算积分"));
	settextstyle(15, 0, _T("黑体"));
	outtextxy(800, 400, _T("右击右处的空"));
	outtextxy(800, 420, _T("白地方，来重"));
	outtextxy(800, 440, _T("新输入函数式"));
	settextcolor(_TEXT2_COLOR);
	settextstyle(16, 0, _T("黑体"));
	outtextxy(810, 100, _T("放缩比例:"));
	setbkmode(OPAQUE);
	setfillcolor(_OTHER_COLOR);
	solidrectangle(800, 120, 900, 200);
	settextstyle(18, 0, _T("Consolas"));
	settextcolor(_TEXT3_COLOR);
	swprintf(a, L"x%.1f", _power(2, _attributes.shrink));
	setbkmode(TRANSPARENT);
	outtextxy(805, 125, a);
	setbkmode(OPAQUE);
	FlushBatchDraw();
}
void xyDRAW()
{
	int Xn[8], Yn[6];
	int Xm, Ym;
	int Xf, Yf;
	short b;
	short k;
	wchar_t* w = new wchar_t[16];
	//x
	setlinecolor(_XY_COLOR);
	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 3);
	if (_attributes.y <= 50)
	{
		line(0, 50, 800, 50);
		Yf = 50;
	}
	else if (_attributes.y > 50 && _attributes.y < 550)
	{
		line(0, _attributes.y, 800, _attributes.y);
		Yf = _attributes.y;

	}
	else
	{
		line(0, 550, 800, 550);
		Yf = 550;
	}
	if (_attributes.x <= 50)
	{
		line(50, 0, 50, 600);
		Xf = 50;
	}
	else if (_attributes.x > 50 && _attributes.x < 750)
	{
		line(_attributes.x, 0, _attributes.x, 600);
		Xf = _attributes.x;
	}
	else
	{
		line(750, 0, 750, 600);
		Xf = 750;
	}

	Xm = -1;
	do
	{
		Xm++;
		b = (_attributes.x - Xm) % 100;

	} while (!(Xm > 99 || b == 0));
	Xn[0] = Xm;
	for (k = 1; k <= 7; k++)
		Xn[k] = Xn[k - 1] + 100;
	Ym = -1;
	do
	{
		Ym++;
		b = (_attributes.y - Ym) % 100;

	} while (!(Ym > 99 || b == 0));
	Yn[0] = Ym;
	for (k = 1; k <= 5; k++)
		Yn[k] = Yn[k - 1] + 100;

	setlinecolor(_GRID_COLOR);
	setlinestyle(PS_DOT | PS_ENDCAP_SQUARE,1);
	settextstyle(15, 0, _T("Consolas"));
	settextcolor(_TEXT_COLOR);

	for (k = 0; k <= 7; k++)
	{
		line(Xn[k], 0, Xn[k], 600);
		swprintf(w,L"%.4lf",(double)((Xn[k]-_attributes.x)*ratio));
		outtextxy(Xn[k], Yf, w);
	}
	for (k = 0; k <= 5; k++)
	{
		line(0, Yn[k], 800, Yn[k]);
		swprintf(w, L"%.4lf", (double)(-(Yn[k] - _attributes.y) * ratio));
		outtextxy(Xf, Yn[k], w);
	}
	delete[] w;

}
void fuctionDRAW()
{
	setfillcolor(_LINE_COLOR);
	_point fst, sec;
	long long int fstz, secz;
	double xz;
	int xzx = 1;
	short xzxk=1;
	int d;
	xz = (1 - _attributes.x) * ratio;
	for (; xzx <= 800; xzx++, xz += ratio)
	{
		if (_domain.IsNull == 0 || (_domain.IsNull == 1 && (xz >= _domain.Min && xz <= _domain.Max)))
		{
			if (xzxk == 1)
			{
				xzxk = 0;
				fst = postfix_operation(xz);
				if (fst.exist == 1)
				{
					fstz = _attributes.y - (long long int)(fst.y / ratio);
					if (fstz >= 0 && fstz <= 600)
						solidcircle(xzx, fstz, _LINE_WIDTH);
				}

			}
			else
			{
				xzxk = 1;
				sec= postfix_operation(xz+ratio);
				secz = _attributes.y - (long long int)(sec.y / ratio);
				if (fst.exist == 1 && sec.exist == 1)
				{
					if (fstz >= secz)
					{
						d = secz;
						do
						{
							solidcircle(xzx, d, _LINE_WIDTH);
							d++;
						} while (d < fstz && (d < 600 && d>0));
					}
					else
					{
						d = fstz;
						do
						{
							solidcircle(xzx, d, _LINE_WIDTH);
							d++;
						} while (d < secz && (d < 600 && d>0));
					}
				}
			}
		}
	}

}
void draw()
{
	MOUSEMSG m;
	_attributes.x = 400;
	_attributes.y = 300;
	_attributes.shrink = 3;
	ratio = 0.02;
	struct
	{
		long long int x;
		long long int y;
	}first,second;
	int k;
	int dx, dy;

		cleardevice();
		fuctionANDxyDRAW();
	while (TRUE)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();
		if (m.mkLButton && ((m.x > 800 && m.x < 900) && (m.y > 550 && m.y < 600)))
			calculus();
		else if (m.mkRButton && ((m.x > 800 && m.x < 900) && (m.y > 0 && m.y < 600)))
		{
			cleardevice();
			break;
		}
		else if(m.wheel==-120 && ((m.x > 0 && m.x < 800) && (m.y > 0 && m.y < 600)))
		{
			if (_attributes.shrink > 10)
			{
				MessageBox(NULL, TEXT("放大到极限了"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONWARNING | MB_SYSTEMMODAL | MB_OK);
				continue;
			}
			_attributes.shrink++;
			ratio *= 2;
			dx = (_attributes.x - 400) * 2;
			dy = (_attributes.y - 300) * 2;
			_attributes.x = 400 + dx;
			_attributes.y = 300 + dy;
			fuctionANDxyDRAW();
		}
		else if (m.wheel == 120 && ((m.x > 0 && m.x < 800) && (m.y > 0 && m.y < 600)))
		{
			if (_attributes.shrink <0)
			{
				MessageBox(NULL, TEXT("缩小到极限了"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONWARNING | MB_SYSTEMMODAL | MB_OK);
				continue;
			}
			_attributes.shrink--;
			ratio =ratio/2;
			dx = (_attributes.x - 400) / 2;
			dy = (_attributes.y - 300) / 2;
			_attributes.x = 400 + dx;
			_attributes.y = 300 + dy;
			fuctionANDxyDRAW();
		}
		else if (m.mkLButton && ((m.x > 0 && m.x < 800) && (m.y > 0 && m.y < 600)))
		{
			k = 0;
			first.x = m.x;
			first.y = m.y;
			second.x = m.x;
			second.y = m.y;
			do
			{
				FlushMouseMsgBuffer();
				m = GetMouseMsg();
				if (k == 0)
				{
					k = 1;
					first.x = m.x;
					first.y = m.y;
					dx = first.x - second.x;
					dy = first.y - second.y;

				}
				else
				{
					k = 0;
					second.x = m.x;
					second.y = m.y;
					dx = second.x - first.x;
					dy = second.y - first.y;
				}
				_attributes.x += dx;
				_attributes.y += dy;
				fuctionANDxyDRAW();
			} while (m.mkLButton);
		}
		else
			;

	}
}
double _power(double x, double n)//此函数由李剑寒编写
{
	double i;
	double t = 1.0;
	if (n == 0)
		return 1.0;
	if (n == 1)
		return x;
	for (i = 1; i <= n; i++)
	{
		t *= x;
	}
	return t;
}
void check()
{
	int n = 0;
	do
	{
		if (infix_expression[n] == ' ')
			n++;
		else if (infix_expression[n] == '(')
		{
			M_error.k++;
			n++;
		}
		else if (infix_expression[n] == ')')
		{
			M_error.k--;
			n++;
		}
		else if ('0' <= infix_expression[n] && '9' >= infix_expression[n])
			n++;
		else if (infix_expression[n] == '.')
			n++;
		else if (infix_expression[n] == '+' || infix_expression[n] == '-' || infix_expression[n] == '*' || infix_expression[n] == '/')
			n++;
		else if (std::tolower(infix_expression[n]) == 'e')
		{
			infix_expression[n] = 'e';
			n++;
		}
		else if (std::tolower(infix_expression[n]) == 'x')
		{
			infix_expression[n] = 'x';
			n++;
		}
		else if (infix_expression[n] == '^')
		{
			if (infix_expression[n + 1] <= '9' && infix_expression[n + 1] >= '0')
				do
				{
					n++;
			
				} while (infix_expression[n ] <= '9' && infix_expression[n ] >= '0');
			else
			{
				M_error.y = 3;
				M_error.n = n;
				break;
			}
			if (infix_expression[n] == '.')
			{
				M_error.y = 3;
				M_error.n = n;
				break;
			}
			
		}
		else if (std::tolower(infix_expression[n]) == 's' && std::tolower(infix_expression[n + 1]) == 'i' && std::tolower(infix_expression[n + 2]) == 'n')
			n += 3;
		else if (std::tolower(infix_expression[n]) == 'c' && std::tolower(infix_expression[n + 1]) == 'o' && std::tolower(infix_expression[n + 2]) == 's')
			n += 3;
		else if (std::tolower(infix_expression[n]) == 't' && std::tolower(infix_expression[n + 1]) == 'a' && std::tolower(infix_expression[n + 2]) == 'n')
			n += 3;
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'o' && std::tolower(infix_expression[n + 2]) == 'g')
			n += 3;
		else if (std::tolower(infix_expression[n]) == 's' && std::tolower(infix_expression[n + 1]) == 'q' && std::tolower(infix_expression[n + 2]) == 'r')
			n += 3;
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'n')
			n += 2;
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'g')
			n += 2;

		else
		{
			M_error.y = 2;
			M_error.n = n;
			break;
		}

	} while (infix_expression[n] != '\0');
	if (M_error.k != 0)
		M_error.y = 1;


}
int USERerror()//0 no error ; 1 exist error
{
	wchar_t a[64];
	if (M_error.y <= 3 && M_error.y >= 1)
	{
		if (M_error.y == 1)
		{
			MessageBox(NULL, TEXT("括号不匹配"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
			return 1;
		}
		else if (M_error.y == 2)
		{
			swprintf(a, L"在您输入的%d个字符及其之后，程序无法识别", M_error.n+1);
			MessageBox(NULL, a, TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
			return 2;
		}
		else if (M_error.y == 3)
		{
			swprintf(a, L"在您输入的%d个字符及其之后，高次幂上指数位非正整常数", M_error.n+1);
			MessageBox(NULL, a, TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
			return 3;
		}
		else
			return 0;
	}
	else
		return 0;
}
void wcharTOinfix()
{
	int n = 0;
	int t = 0;
	wchar_t w[16];
	int m;
	do
	{
		if (infix_expression[n] == ' ')
			n++;
		else if (infix_expression[n] == '(')
		{
			infix[t].exist = 1;
			infix[t].prority = 5;
			infix[t].num = 11.0;
			n++;
			t++;
		}
		else if (infix_expression[n] == ')')
		{
			infix[t].exist = 1;
			infix[t].prority = 5;
			infix[t].num = 12.0;
			n++;
			t++;
		}
		else if ('0' <= infix_expression[n] && '9' >= infix_expression[n])
		{
			m = 0;
			memset(w, 0, sizeof(w));
			do
			{
				w[m] = infix_expression[n];
				n++;
				m++;
			} while ('0' <= infix_expression[n] && '9' >= infix_expression[n]|| '.' == infix_expression[n]);
			infix[t].exist = 1;
			infix[t].prority = 0;
			infix[t].num =_ttof(w);
			t++;


		}
		else if (infix_expression[n] == '+' || infix_expression[n] == '-' || infix_expression[n] == '*' || infix_expression[n] == '/')
		{
			infix[t].exist = 1;
			if (infix_expression[n] == '+')
			{
				infix[t].prority = 1;
				infix[t].num = 0;
			}
			else if (infix_expression[n] == '-')
			{
				infix[t].prority = 1;
				infix[t].num = 1;
			}
			else if (infix_expression[n] == '*')
			{
				infix[t].prority = 2;
				infix[t].num = 2;
			}
			else 
			{
				infix[t].prority = 2;
				infix[t].num = 3;
			}
			t++;
			n++;
		}
		else if (std::tolower(infix_expression[n]) == 'e')
		{
			infix[t].exist = 1;
			infix[t].prority = 0;
			infix[t].num = 2.7182;
			n++;
			t++;
		}
		else if (std::tolower(infix_expression[n]) == 'x')
		{

			infix[t].exist = 1;
			infix[t].prority = -1;
			
			n++;
			t++;
		}
		else if (infix_expression[n] == '^')
		{
			infix[t].exist = 1;
			infix[t].prority =3 ;
			infix[t].num = 4;
			n++;
			t++;
		}
		else if (std::tolower(infix_expression[n]) == 's' && std::tolower(infix_expression[n + 1]) == 'i' && std::tolower(infix_expression[n + 2]) == 'n')
		{
			infix[t].exist = 1;
			infix[t].prority =4 ;
			infix[t].num = 5;
			n+=3;
			t++;
		}
		
		else if (std::tolower(infix_expression[n]) == 'c' && std::tolower(infix_expression[n + 1]) == 'o' && std::tolower(infix_expression[n + 2]) == 's')
		{
			infix[t].exist = 1;
			infix[t].prority = 4;
			infix[t].num = 6;
			n += 3;
			t++;
		}
		else if (std::tolower(infix_expression[n]) == 't' && std::tolower(infix_expression[n + 1]) == 'a' && std::tolower(infix_expression[n + 2]) == 'n')
		{
		infix[t].exist = 1;
		infix[t].prority = 4;
		infix[t].num = 7;
		n += 3;
		t++;
		}
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'o' && std::tolower(infix_expression[n + 2]) == 'g')
			{
			infix[t].exist = 1;
			infix[t].prority = 4;
			infix[t].num = 8;
			n += 3;
			t++;
		}
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'n')
			{
			infix[t].exist = 1;
			infix[t].prority = 4;
			infix[t].num = 8;
			n += 2;
			t++;
		}
		else if (std::tolower(infix_expression[n]) == 'l' && std::tolower(infix_expression[n + 1]) == 'g')
		{
		infix[t].exist = 1;
		infix[t].prority = 4;
		infix[t].num = 9;
		n += 2;
		t++;
		}
		else if (std::tolower(infix_expression[n]) == 's' && std::tolower(infix_expression[n + 1]) == 'q' && std::tolower(infix_expression[n + 2]) == 'r')
			{
			infix[t].exist = 1;
			infix[t].prority = 4;
			infix[t].num = 10;
			n += 3;
			t++;
		}
		else
		{
		MessageBox(NULL, TEXT("0x001"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
		exit(0);
		}

	} while (infix_expression[n] != '\0');
	if (M_error.k != 0)
		M_error.y = 1;


}
void infixTOpostfix()
{
	_Expression * stack1 = new _Expression[128];//运算符堆栈

	short stack1_cursor = -1;
	short infix_cursor=0;
	short postfix_cursor = -1;

	/*
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
 */
	while (infix[infix_cursor].exist != 0)//1、从左至右扫描 中缀表达式。
	{
		if (infix[infix_cursor].prority == 0 || infix[infix_cursor].prority == -1)
		{
			postfix_cursor++;//向堆栈放入数据，先将游标往上拨动一格，再放入
			postfix[postfix_cursor] = infix[infix_cursor];
			infix_cursor++;
		}
		else
		{
			if (infix[infix_cursor].prority == 5 && infix[infix_cursor].num == 11)//(1) 该运算符为左括号"("，则直接存入运算符堆栈。
			{
				stack1_cursor++;
				stack1[stack1_cursor]= infix[infix_cursor];
				infix_cursor++;
			}
			else if (infix[infix_cursor].prority == 5 && infix[infix_cursor].num == 12)//(2) 该运算符为右括号")"，则输出运算符堆栈中的运算符到操作数堆栈，直到遇到左括号为止。
			{
				
				while (stack1[stack1_cursor].num != 11)//只读操作，游标不拨动
				{
					postfix_cursor++;//向堆栈放入数据，先将游标往上拨动一格，再放入
					postfix[postfix_cursor] = stack1[stack1_cursor];//向堆栈推出数据，先推出，再把游标往下拨动
					stack1_cursor--;
				}
				infix_cursor++;
				stack1_cursor--;
			}
			else//(3) 该运算符为非括号运算符：
			{
				if (stack1_cursor == -1)
				{
					stack1_cursor++;
					stack1[stack1_cursor] = infix[infix_cursor];
					infix_cursor++;
				}
				else if (stack1[stack1_cursor].prority == 5 && stack1[stack1_cursor].num == 11)//(a) 若运算符堆栈栈顶的运算符为括号，则直接存入运算符堆栈。
				{
					stack1_cursor++;
					stack1[stack1_cursor] = infix[infix_cursor];
					infix_cursor++;
				}
				else
				{
					//(b) 若比运算符堆栈栈顶的运算符优先级高或相等，则直接存入运算符堆栈。
					//(c)若比运算符堆栈栈顶的运算符优先级低，则输出栈顶运算符到操作数堆栈，{括号除外}并将当前运算符压入运算符堆栈。
					while (infix[infix_cursor].prority < stack1[stack1_cursor].prority)
					{
						if (stack1[stack1_cursor].prority == 5 && stack1[stack1_cursor].num == 11)
							break;//括号优先级搞错的临时补救
						postfix_cursor++;
						postfix[postfix_cursor] = stack1[stack1_cursor];
						stack1_cursor--;
					}
					stack1_cursor++;
					stack1[stack1_cursor] = infix[infix_cursor];
					infix_cursor++;
				}
			}
		}
	}
	while (stack1_cursor > -1)
	{
		postfix_cursor++;
		postfix[postfix_cursor] = stack1[stack1_cursor];
		stack1_cursor --;
	}
	delete[] stack1;
}
_point postfix_operation(double x)
{
	_point result;
	result.exist = 1;
	result.x = x;
	double a, b;
	double c;//从该栈弹出的两个操作数。	例：a ? b, 先弹出b，后弹出a。[这里abc都可以简化掉，因为逻辑不熟，故没有]
	_Expression* stack2 = new _Expression[128];
	short postfix_cursor=0;
	short stack2_cursor = -1;

	//注意定义域问题
	/*调度场算法
逆波兰表达式

二、逆波兰表达式求值算法：
1、循环扫描语法单元的项目。
2、如果扫描的项目是操作数，则将其压入操作数堆栈，并扫描下一个项目。
3、如果扫描的项目是一个二元运算符，则对栈的顶上两个操作数执行该运算。

4、如果扫描的项目是一个一元运算符，则对栈的最顶上操作数执行该运算。
5、将运算结果重新压入堆栈。
6、重复步骤2-5，全部运算完后，最后堆栈中数 即为结果值。
 */


	while (postfix[postfix_cursor].exist ==1)//1、循环扫描语法单元的项目。
	{
		if (postfix[postfix_cursor].prority == -1)
		{
			stack2_cursor++;
			stack2[stack2_cursor] = postfix[postfix_cursor];
			stack2[stack2_cursor].prority = 0;
			stack2[stack2_cursor].num = x;
			postfix_cursor++;
		}
		else if (postfix[postfix_cursor].prority == 0)
		{
			stack2_cursor++;
			stack2[stack2_cursor] = postfix[postfix_cursor];
			postfix_cursor++;
		}
		else
		{

			b= stack2[stack2_cursor ].num;
			if(postfix[postfix_cursor].num==0)//+
			{
				a = stack2[stack2_cursor - 1].num;
				c = a + b;
				stack2[stack2_cursor - 1].num = c;
				stack2_cursor--;
			}
			else if(postfix[postfix_cursor].num == 1)//-

			{
				a = stack2[stack2_cursor - 1].num;
				c = a -b;
				stack2[stack2_cursor - 1].num = c;
				stack2_cursor--;
			}
			else if (postfix[postfix_cursor].num == 2)//*

			{
				a = stack2[stack2_cursor - 1].num;
				c = a * b;
				stack2[stack2_cursor - 1].num = c;
				stack2_cursor--;
			}
			else if (postfix[postfix_cursor].num == 3)//　　/

			{
				a = stack2[stack2_cursor - 1].num;
				if (b == 0)
				{
					result.exist = 0;
					result.y = 0;
					break;
				}
				c = a / b;
				stack2[stack2_cursor - 1].num = c;
				stack2_cursor--;
			}
			else if(postfix[postfix_cursor].num == 4)//幂
			{
				a = stack2[stack2_cursor - 1].num;
				c = _power(a,b);
				stack2[stack2_cursor - 1].num = c;
				stack2_cursor--;
			}
			else if (postfix[postfix_cursor].num == 5)//sin
			{
				c = sin(b);
				stack2[stack2_cursor].num = c;
			}
			else if (postfix[postfix_cursor].num == 6)//cos
			{
				c = cos(b);
				stack2[stack2_cursor ].num = c;
			}
			else if (postfix[postfix_cursor].num == 7)//tan因为无法准确定位到π，所以不必要精准判断是否存在
			{
				c = tan(b);
				stack2[stack2_cursor ].num = c;
			}
			else if (postfix[postfix_cursor].num == 8)//log [ln]
			{
				if (b <= 0)
				{
					result.exist = 0;
					result.y = 0;
					break;
				}
				c = log(b);
				stack2[stack2_cursor ].num = c;
			}
			else if (postfix[postfix_cursor].num == 9)//lg
			{
				if (b <= 0)
				{
					result.exist = 0;
					result.y = 0;
					break;
				}
				c = log10(b);
				stack2[stack2_cursor].num = c;
			}
			else if (postfix[postfix_cursor].num == 10)//sqr
			{
				
				if (b <0)
				{
					result.exist = 0;
					result.y = 0;
					break;
				}c = sqrt(b);
				stack2[stack2_cursor ].num = c;
			}
			else
			{
				MessageBox(NULL, TEXT("0x002"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
				exit(0);
			}
			postfix_cursor++;
		}
	}
	result.y = stack2[0].num;
	delete []stack2;

	return result;
}

//数据初始化， 防止上一轮数据影响此次数据
void Initialization_Data()
{
	memset(infix_expression, 0, sizeof(infix_expression));
	memset(infix, 0, sizeof(infix));
	memset(postfix, 0, sizeof(postfix));
	memset(&M_error, 0, sizeof(M_error));
}//利用memset函数进行快速初始化
int Data_process(void)
{	
	int k;
	 check();
	 k = USERerror();
	 if (k == 0)
	 {
		 wcharTOinfix();
		 infixTOpostfix();
	 }
	 return k;
}
void start_menu()
{
	
	initgraph(900,600);
	setbkcolor(_BACKGROUND_COLOR);
	cleardevice();//用背景色清空屏幕
	IMAGE imgback;
	loadimage(&imgback, _T("image//001.jpg"));// 加载副界面
	putimage(0, 0, &imgback);// 显示副界面
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
		InputBox(a1, 15, TEXT("请输入定义域上限"), TEXT("ヽ(*ﾟдﾟ)ノ"), TEXT("0"), 200, 0, true);
		_domain.Max = std::stod(a1);
		InputBox(b1, 15, TEXT("请输入定义域下限"), TEXT("ヽ(*ﾟдﾟ)ノ"), TEXT("0"), 200, 0, true);
		_domain.Min = std::stod(b1);
	}
	
	else
		_domain.IsNull = 0;
}

int main(void)
{
	int k;

	start_menu();
	do
	{
		do
		{
			Initialization_Data();//数据初始化， 防止上一轮数据影响此次数据
			Data_INPUT();//数据录入
			k = Data_process();
		} while (k==1);

		draw();

	} while (MessageBox(NULL, TEXT("是否重新绘制图像"), TEXT("ヽ(*ﾟдﾟ)ノ"), MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_YESNO)==IDYES);
	//利用messagebox进行对话操作  MB_ICONINFORMATION 意思是显示 i 图标  MB_SYSTEMMODAL 意思是设置对话框在所有窗口最前端 MB_YESNO代表按钮为是否类型
	closegraph();
	exit (0);
	return 0;
}
