// finalproject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define acc -2
#include <iostream>
#include <string>
#include<stack>
#include<iomanip>
using namespace std;
stack<int> stacksyl;//赋值语句/布尔表达式种别编码栈
stack<int> stackpos;//赋值语句/布尔表达式位置栈
stack<int> Bstack;//赋值语句/布尔表达式状态栈
stack<int> programpos;//主程序状态栈
stack<int> programsyl;//主程序种别编码栈
/*主程序分析表*/
static int action[19][13] =
{{2,-1, -1,3,4,-1,-1 ,5,-1 ,-1,-1, 1,-1},
{-1,-1,-1,-1 ,-1, -1 ,-1 ,-1,-1,-1 ,acc,-1,-1}, 
{-1 ,-1,-1,-1 ,-1 ,-1,-1,-1,-1,6,-1 ,-1 ,-1 },
{-1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1},
{ 2,-1 ,-1,3,4,-1 ,-1,5,-1,-1,-1,9,8 },
{ -1 ,-1 , 104,-1 ,-1,-1, 104,-1 ,104,-1,104,-1,-1 },
{ -1,10,-1 ,-1,-1 ,-1 ,-1 ,-1,-1 ,-1 ,-1 ,-1,-1 },
{-1, -1, -1,-1, -1, 11, -1, -1, -1, -1, -1, -1,-1},
{ -1 ,-1 ,-1,-1 ,-1, -1 ,12,-1 ,-1,-1,-1,-1,-1 },
{ -1,-1,-1,-1 ,-1 ,-1 ,105,-1, 13,-1,-1 ,-1 ,-1 },
{ 2,-1,-1 ,3,4,-1 ,-1,5,-1,-1,-1, 14,-1},
{2,-1,-1 ,3,4,-1 ,-1,5,-1,-1,-1,15, -1},
{-1 ,-1, 103,-1 ,-1,-1,103,-1, 103,-1,103,-1,-1},
{2,-1 ,-1 ,3,4,-1,-1 ,5,-1,-1,-1,9, 16},
{ -1, -1 , 17, -1, -1, -1, -1, -1 , -1,-1, -1 , -1, -1 } ,
{-1 ,-1, 102,-1,-1,-1, 102,-1, 102,-1,102,-1,-1 }, 
{-1 ,-1,-1,-1 ,-1 ,-1,106,-1,-1 ,-1 ,-1 ,-1 ,-1},
{2,-1,-1 ,3,4,-1 ,-1 ,5,-1 ,-1 ,-1 ,18,-1},
{-1 ,-1, 101,-1,-1 ,-1,101,-1 ,101,-1, 101,-1,-1}};
/*算数表达式分析表*/
static int action1[10][7] =
{ {3, -1, -1, 2, -1, -1, 1},
{-1,4,5,-1 ,-1,acc,-1 },
{3,-1,-1,2,-1 ,-1 ,6},
{104,104,104,104, 104,104,-1 },
{3,-1 ,-1 ,2,-1 ,-1,7},
{3,-1 ,-1 ,2,-1 ,-1,8},
{-1,4,5,-1,9,-1,-1},
{101,101,5,101,101,101,-1},
{102,102,102,102,102,102,-1 },
{103,103,103,103,103,103,-1} };
/*布尔表达式分析表*/
static int action2[16][11] =
{ { 1,-1,4,-1,5,-1,-1,-1,13,7,8 },
{ -1,2,-1,101,-1,101,101,101,-1,-1,-1 },
{ 3 ,-1, -1 ,-1 , -1, -1 , -1, -1 , -1 , -1 ,-1 },
{ -1,-1, -1,102,-1,102,102, 102,-1,-1 ,-1},
{ 1,-1,4,-1,5,-1,-1,-1,11,7,8},
{ 1,-1,4,-1,5,-1,-1,-1,6,7,8},
{-1,-1,-1,104,-1,9,10,104,-1,-1,-1 },
{ 1,-1,4,-1,5,-1,-1,-1,14,7,8},
{ 1,-1,4,-1,5,-1,-1,-1,15,7,8},
{ 105,-1, 105,-1, 105,-1,-1 ,105,-1,-1 ,-1},
{ 107,-1,107,-1, 107,-1,-1,107,-1 ,-1 ,-1 },
{-1 ,-1,-1, 12,-1 ,9,10,-1,-1 ,-1 ,-1},
{-1, 103,-1 ,103,-1, 103, 103,103,-1,-1,-1},
{-1,-1,-1 ,-1,-1 ,9,10,acc,-1 ,-1 ,-1},
{-1,-1,-1,106,-1,9,10,106,-1,-1,-1 },
{-1,-1,-1,108,-1,9,10,108,-1,-1,-1} };
void scanner();//词法分析
void printscanner();//词法分析输出
void lrbool(int id);//布尔表达式LR分析
void lrcalculate(int id);//赋值语句LR分析
void lrprogram();//主程序SLR分析
bool test(int value);//获取赋值语句/布尔表达式语句区域
void printsiyuanshi();//输出四元式序列
void chuzhan(int time, int syb);//主程序出栈操作
void chuzhancalculate(int time1, int time2);//赋值语句出栈操作
void chuzhanbool(int time1, int time2, int syb);//布尔表达式出栈操作
void create(string op, int asyl, int apos, int bsyl, int bpos, int result);//创建四元式
void saveword0(int id,string syb,string save);//词法分析保存数据
void huibian();
int merg(int p1, int p2);//布尔表达式拉链
void backpatch(int p, int t);//布尔表达式反填
int changeprogram(int id);//主程序LR分析转换矩阵
int changebool(int id);//布尔表达式LR分析转换矩阵
int changecalculate(int id);//赋值语句LR分析转换矩阵
int nowloc;//slr分析当前读取的saveword地址
int Epos = 0;//Epos表示当前临时变量E
int newt = 0; //newt表示第几个新的临时变量
int temppos0, tempsyl0;//表示赋值/布尔SLR分析时当前元素的种别编码和saveword地址
int lr = 0, lr1 = 0, lr2 = 0;//三种语句的lr表值，lr表示主程序，lr1表示算数表达式，lr2表示布尔表达式
int tempsyl[100], temppos[100];//布尔表达式/赋值语句分析时存入要分析单词的数组
int nxq = 100;//四元式地址
int label = 0;//布尔表达式分析时四元式数组位置
int labeltemp[20];//主程序分析时需要临时存入的四元式地址
int pointmark = -1, pointtemp = -1;//主程序分析时四元式数组位置
int nowsyl;//当前检测单词的种别编码
int sum = 0;//一共读入了多少个单词
int sign = 0;//哪种语句判断
struct saveword {
	int id;//种别编码
	string syb;//语义
	string save;//保存字符内容
}saveword[100];//读取的单词数组
struct siyuanshi {
	string op;//操作数
	int asyl;//第一个操作数种别编码
	int apos;//第一个操作数在saveword中的位置
	int bsyl;//第二个操作数种别编码
	int bpos;//第二个操作数在saveword中的位置
	int result;//操作数转换地址，可以为临时变量
}fexp[200];//四元式序列
struct ntab {
	int tc;//truelist
	int fc;//falselist
}ntab2[200];//布尔表达式的TF操作
struct boolsiyuanshi {
	int nxq;//四元式系列地址
	int tc;//truelist
	int fc;//falselist
}labelmark[20];//主程序的TF操作
int merg(int p1, int p2)
{
	int p;
	if (p2 == 0)return p1;
	else
	{
		p = p2;
		while (fexp[p].result != 0)p = fexp[p].result;
		fexp[p].result = p1;
		return p2;
	}
}
void backpatch(int p, int t)
{
	int tempaa;
	int q;
	q = p;
	while (q != 0)
	{
		tempaa = fexp[q].result;
		fexp[q].result = t;
		//cout << "fexp[" << q << "].result=" << t << endl;
		q = tempaa;
	}
}
void saveword0(int id, string syb, string save)
{
	saveword[sum - 1].id = id;
	saveword[sum - 1].syb = syb;
	saveword[sum - 1].save = save;
}
void scanner()
{
	string A="";//id或者num
	cout << "请输入一段程序语句(输入#~以结束)：" << endl;
	char ch = NULL, temp = NULL;//ch每次读入的字符，temp为连接在一起输入时的临时变量
	//无限读入
	do {
		ch = NULL;//终结的时候要reset
		A = "";
	start://每次读入单词开始，如果是连续输入没有空格，需要保留上一个终结的最后一个字符
		if (temp != NULL) {
			A = temp;
			ch = temp;
			temp = NULL;
		}
		else ch = getchar();//如果是有空格，则直接读入新的字符
		if ((ch == '\t') || (ch == '\n') || (ch == ' '))goto start;//如果是回车或者空格，则继续读入
		else
		{
			sum++;
			if (ch == ';')saveword0(8, "semicolon", ";");//;
			else if (ch == '*')saveword0(9, "multiply", "*");//*
			else if (ch == '+')saveword0(10, "plus", "+");//+
			else if (ch == ':')//:=
			{
				ch = getchar();
				if (ch == '=')saveword0(11, "equal", ":=");
				else exit(0);
			}
			else if (ch == '!')//! or !=
			{
				ch = getchar();
				if (ch == '=')saveword0(17, "rop", "!=");//!=
				else //!
				{
					saveword0(12, "opnot", "!");
					if ((ch != '\t') && (ch != '\n') && (ch != ' ')) temp = ch;//连续输入保留
				}
			}
			else if (ch == '&')//&&
			{
				ch = getchar();
				if (ch == '&')saveword0(13, "opand", "&&");
				else exit(0);
			}
			else if (ch == '|')//||
			{
				ch = getchar();
				if (ch == '|')saveword0(14, "opor", "||");
				else exit(0);
			}
			else if (ch == '(')saveword0(15, "leftparenthesis", "(");//(
			else if (ch == ')')saveword0(16, "rightparenthesis", ")");//)
			else if (ch == '=')saveword0(17, "rop", "=");//=
			else if (ch == '<')//<= or <
			{
				ch = getchar();
				if (ch == '=')saveword0(17, "rop", "<=");//<=
				else //<
				{
					saveword0(17, "rop", "<");
					if ((ch != '\t') && (ch != '\n') && (ch != ' ')) temp = ch;//连续输入保留
				}
			}
			else if (ch == '>')//>= or >
			{
				ch = getchar();
				if (ch == '=')saveword0(17, "rop", ">=");//>=
				else //<
				{
					saveword0(17, "rop", ">");
					if ((ch != '\t') && (ch != '\n') && (ch != ' ')) temp = ch;//连续输入保留
				}
			}
			else if (ch >= '0' && ch <= '9')//数字常量
			{
				if (A == "")A = A + ch;
				do
				{
					ch = getchar();
					if (ch >= '0' && ch <= '9')
					{
						A = A + ch;
					}
					else//终结
					{
						saveword0(19, "intconst", A);
						if ((ch != '\t') && (ch != '\n') && (ch != ' ')) temp = ch;//连续输入保留
						break;
					}
				} while (true);
			}
			else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))//变量OR保留字
			{
				if (A == "")A = A + ch;//保留上一个结尾字符时不需要再重复添加
				do
				{
					ch = getchar();
					if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))	A = A + ch;//循环输入
					else//终结
					{
						//判断是否是保留字
						if (A == "if")saveword0(1, "sy_if", A);
						else if (A == "then")saveword0(2, "sy_then", A);
						else if (A == "else")saveword0(3, "sy_else", A);
						else if (A == "while")saveword0(4, "sy_while", A);
						else if (A == "begin")saveword0(5, "sy_begin", A);
						else if (A == "do")saveword0(6, "sy_do", A);
						else if (A == "end")saveword0(7, "sy_end", A);
						else saveword0(18, "ident", A);
						if ((ch != '\t') && (ch != '\n') && (ch != ' ')) temp = ch;//连续输入保留
						break;
					}
				} while (true);
			}
			else if (ch == '#') 
			{
				cout << "词法分析结束，分析结果如下：" << endl;
				sum--;
				break;
			}
			else {
				cout << "词法分析发现错误，停止分析!" << endl;
				exit(0);
			}
		}
	} while (true);
}
void printscanner()
{
	cout <<   std::right<< setw(22)<<"单词释义" << std::right << setw(8) << "值" << std::right << setw(14) << "种别编码" << endl;
	for (int i = 0; i < sum; i++)
	{
		if (saveword[i].id == 18)
			cout <<  std::left  << "第" << setw(2)<< i + 1 << "个单词为："<< std::left << setw(10)<<"(变量     ," << std::right << setw(5) << saveword[i].save << ")" << std::right << setw(10) << saveword[i].id<<endl;
		else if (saveword[i].id == 19)
			cout << std::left <<  "第" << setw(2) << i + 1 << "个单词为：" << std::left << setw(10) << "(整型     ," << std::right << setw(5) << saveword[i].save << ")" << std::right << setw(10) << saveword[i].id << endl;
		else if (saveword[i].id == 17)
			cout << std::left  << "第" << setw(2) << i + 1 << "个单词为：" << std::left << setw(10) << "(rop      ," << std::right << setw(5) << saveword[i].save << ")" << std::right << setw(10) << saveword[i].id << endl;
		else if (saveword[i].id <= 7 && saveword[i].id >= 1)
			cout << std::left  << "第" << setw(2) << i + 1 << "个单词为：" << std::left << "(" << setw(9) << saveword[i].syb << "," << std::right << setw(6) <<"0)" << std::right << setw(10) << saveword[i].id << endl;
		else cout << std::left  << "第" << setw(2) << i + 1 << "个单词为：" << std::left << "(" << setw(9) << saveword[i].save  << "," << std::right << setw(6) <<"0)" << std::right << setw(10) << saveword[i].id << endl;
	}
}
void printsiyuanshi()
{
	for (int i = 100; i < nxq; i++)
	{
		cout << i << "	(" << fexp[i].op<<",";
		if (fexp[i].asyl == 18 || fexp[i].asyl == 19)//整型或常量，就去找词语数组对应位置保存的内容
		{
			cout << saveword[fexp[i].apos].save << ",";
		}
		else if (fexp[i].asyl == 21)//临时变量，首字母E加上Epos对应的值组合
		{
			cout << 'T'<< fexp[i].apos << ",";
		}
		else cout << "_," ;//都不是，输出_(空)

		if (fexp[i].bsyl == 18 || fexp[i].bsyl == 19)//整型或常量，就去找词语数组对应位置保存的内容
		{
			cout << saveword[fexp[i].bpos].save << ",";
		}
		else if (fexp[i].bsyl == 21)//临时变量，首字母E加上Epos对应的值组合
		{
			cout << 'T' << fexp[i].bpos << ",";
		}
		else cout << "_,";//都不是，输出_(空)
		if (fexp[i].op[0] != 'j')//不是跳转地址
		{
			if (fexp[i].result >= 100)cout << 'T' << fexp[i].result - 100;//result>=100，则说明是保存的临时变量，用result-100得到临时变量
			else cout << saveword[fexp[i].result].save;//其余情况，说明为常量或整型，输出词语数组对应位置保存的内容
		}
		else cout << fexp[i].result;//直接输出跳转地址
		cout<< ')'<< endl;
	}
}
void chuzhan(int time, int syb)
{
	for (int i = 0; i < time; i++)
	{
		programpos.pop();
		programsyl.pop();
	}
	nowsyl = syb;
}
void chuzhanbool(int time1, int time2, int syb)
{
	for (int i = 0; i < time1; i++)
	{
		stacksyl.pop();
		stackpos.pop();
	}
	for (int i = 0; i < time2; i++)
	{
		Bstack.pop();
	}
	tempsyl0 = syb;
}
int changeprogram(int id)
{
	switch (id)
	{
	case 1:return 0;//if
	case 2:return 1;//then
	case 3:return 2;//else
	case 4:return 3;//while
	case 5:return 4;//begin
	case 6:return 5;//do
	case 7:return 6;//end
	case 24:return 7;//a
	case 8:return 8;//;
	case 27:return 9;//e
	case 20:return 10;//#
	case 22:return 11;//S
	case 23:return 12;//L
	}
}
void create(string op, int asyl, int apos, int bsyl, int bpos, int result)
{
	fexp[nxq].op = op;
	fexp[nxq].asyl = asyl;
	fexp[nxq].apos = apos;
	fexp[nxq].bsyl = bsyl;
	fexp[nxq].bpos = bpos;
	fexp[nxq].result = result;
	//cout << "nxq="<<nxq<<"(op=" << op << ",aid=" << asyl << ",apos=" << apos << ",bid=" << bsyl << ",bpos=" << bpos << ",result=" << result << ')'<<endl;
	nxq++;
}
void chuzhancalculate(int time1, int time2)
{
	for (int i = 0; i < time1; i++)
	{
		stacksyl.pop();
		stackpos.pop();
	}
	for (int i = 0; i < time2; i++)
	{
		Bstack.pop();
	}
}
void lrbool(int id)
{
	int templabel;//临时变量
	lr2 = action2[Bstack.top()][changebool(tempsyl0)];//获取lr1值，Bstack.top默认是现在的状态，第二格为当前即将读入的内容转义到lr分析表的列
	cout << "赋值/布尔种别编码栈顶元素sstack[" << stacksyl.size() - 1 << "]=" << stacksyl.top() << "	当前分析单词种别编码=" << tempsyl0 << "	当前状态=" << Bstack.top() << "	lrbool=" << lr2 << endl;
	if (lr2 == -1)//出错且判断
	{
		if (sign == 2)cout << "while语句出错!" << endl;
		if (sign == 3)cout << "if语句出错!" << endl;
		exit(0);
	}
	if (lr2 < 16 && lr2 >= 0)/*移进*/
	{
		Bstack.push(lr2);//存入现在新的状态
		stacksyl.push(tempsyl0);//存入读入内容的值和位置
		stackpos.push(temppos0);
		if (tempsyl0 != 21 && tempsyl0 != 25 && tempsyl0 != 26)id++;//不是非终结符
		tempsyl0 = tempsyl[id];//更新临时变量为要读内容的值和位置（可能不变）
		temppos0 = temppos[id];
		lrbool(id);
	}
	if (lr2 >= 100 && lr2 < 109)/*规约*/
	{
		switch (lr2)
		{
		case 100:/*S'->B*/
			cout << "S'->B规约" << endl;
			break;
		case 101:/*B->i*/
			cout << "B->i规约" << endl;
			ntab2[label].tc = nxq;
			ntab2[label].fc = nxq + 1;
			//cout << "ntab2.[" << label << "].tc=" << ntab2[label].tc << endl;
			//cout << "ntab2.[" << label << "].fc=" << ntab2[label].fc << endl;
			create("jnz",stacksyl.top() ,stackpos.top() ,-1,0,0);//输出四元式
			create("j", -1, 0, -1, 0, 0);
			chuzhanbool(1, 1, 21);//出栈一格，并且保存内容
			label++;
			break;
		case 102:/*B->i rop i*/
			int temp11, temp12,temp21, temp22;
			cout << "B->i rop i规约" << endl;
			ntab2[label].tc = nxq;
			ntab2[label].fc = nxq + 1;
			//cout << "ntab2.[" << label << "].tc=" << ntab2[label].tc << endl;
			//cout << "ntab2.[" << label << "].fc=" << ntab2[label].fc << endl;
			temp11 = stacksyl.top();//栈顶
			temp12 = stackpos.top();
			stacksyl.pop();
			stackpos.pop();
			temp21 = stacksyl.top();//栈顶-1
			temp22 = stackpos.top();
			chuzhanbool(1, 3, 21);
			//判断不同的rop
			if(saveword[temp22].save=="<=")	create("j<=", stacksyl.top(), stackpos.top(), temp11, temp12,0);
			else if (saveword[temp22].save == "<")create("j<", stacksyl.top(), stackpos.top(), temp11, temp12, 0);
			else if (saveword[temp22].save == ">=")create("j>=", stacksyl.top(), stackpos.top(), temp11, temp12, 0);
			else if (saveword[temp22].save == ">")create("j>", stacksyl.top(), stackpos.top(), temp11, temp12, 0);
			else if (saveword[temp22].save == "=")create("j=", stacksyl.top(), stackpos.top(), temp11, temp12, 0);
			else if (saveword[temp22].save == "!=")create("j!=", stacksyl.top(), stackpos.top(), temp11, temp12, 0);
			stacksyl.pop();
			stackpos.pop();
			create("j", -1, 0, -1, 0, 0);
			label++;
			break;
		case 103:/*B->(B)*/
			cout << "B->(B)规约" << endl;
			chuzhanbool(3, 3, 21);
			break;
		case 104:/*B-> !B*/
			cout << "B->!B规约" << endl;
			label--;
			templabel = ntab2[label].tc;
			ntab2[label].tc = ntab2[label].fc;
			ntab2[label].fc = templabel;
			//cout << "ntab2.[" << label << "].tc=" << ntab2[label].tc << endl;
			//cout << "ntab2.[" << label << "].fc=" << ntab2[label].fc << endl;
			chuzhanbool(2, 2, 21);
			label++;
			break;
		case 105:/*A->B &&*/
			cout << "A->B&&" << endl;
			backpatch(ntab2[label - 1].tc, nxq);
			//cout << "backpatch(ntab2[" << label - 1 << "].tc=" << ntab2[label - 1].tc << ',' << nxq << ')' << endl;
			chuzhanbool(2, 2, 25);
			break;
		case 106:/*B->AB*/
			cout << "B->AB规约" << endl;
			label -= 2;
			ntab2[label].tc = ntab2[label + 1].tc;
			ntab2[label].fc = merg(ntab2[label].fc, ntab2[label + 1].fc);
			//cout << "ntab2.[" << label << "].tc=" << ntab2[label].tc << endl;
			//cout << "ntab2.[" << label << "].fc=" << ntab2[label].fc << endl;
			label++;
			chuzhanbool(2, 2, 21);
			break;
		case 107:/*O->B ||*/
			cout << "O->B ||规约" << endl;
			backpatch(ntab2[label - 1].fc, nxq);
			//cout << "backpatch(ntab2[" << label - 1 << "].fc="<< ntab2[label - 1].fc <<','<< nxq << ')' << endl;
			chuzhanbool(2, 2, 26);
			break;
		case 108:/*B->OB*/
			cout << "B->OB规约" << endl;
			label -= 2;
			ntab2[label].fc = ntab2[label + 1].fc;
			ntab2[label].tc = merg(ntab2[label].tc, ntab2[label + 1].tc);
			//cout << "ntab2.[" << label << "].tc=" << ntab2[label].tc << endl;
			//cout << "ntab2.[" << label << "].fc=" << ntab2[label].fc << endl;
			label++;
			chuzhanbool(2, 2, 21);
			break;
		}
		lrbool(id);
	}
}
bool test(int value)
{
	switch (value)
	{
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		return true;
	default:
		return false;
	}
}
void lrcalculate(int id)
{
	int temp1, temp2;//临时变量
	lr1 = action1[Bstack.top()][changecalculate(tempsyl0)];//获取lr1值，Bstack.top默认是现在的状态，第二格为当前即将读入的内容转义到lr分析表的列
	cout << "赋值/布尔种别编码栈顶元素sstack[" << stacksyl.size() - 1 << "]=" << stacksyl.top() << "	当前分析单词种别编码=" << tempsyl0 << "	当前状态=" << Bstack.top() << "	lrcalculate=" << lr1 << endl;
	if (lr1 == -1)
	{
		cout << "赋值语句出错！" << endl;
		exit(0);
	}
	else if (lr1 < 10 && lr1 >= 0)//移进
	{
		Bstack.push(lr1);
		if (tempsyl0 != 21)//不是非终结符
		{
			id++;
			stacksyl.push(tempsyl0);
			stackpos.push(temppos0);
		}
		tempsyl0 = tempsyl[id];//更新即将读取的值（可能不变）
		temppos0 = temppos[id];
		lrcalculate(id);
	}
	else if (lr1 < 105 && lr1 >= 100)//规约
	{
		switch (lr1)
		{
		case 100:/*S'->E*/
			cout << "S'->E规约" << endl;
			break;
		case 101:/*E->E+E*/
			cout << "E->E+E规约" << endl;
			temp1 = stacksyl.top();
			temp2 = stackpos.top();
			Epos=++newt;//设置新的临时变量
			chuzhancalculate(2, 3);
			create("+", stacksyl.top(), stackpos.top(), temp1, temp2, Epos + 100);//Epos+100是为了区分变量/整型和临时变量
			stacksyl.pop();
			stackpos.pop();
			stacksyl.push(21);
			stackpos.push(Epos);
			break;
		case 102:/*E->E*E*/
			cout << "E->E*E规约" << endl;
			temp1 = stacksyl.top();
			temp2 = stackpos.top();
			Epos=++newt;//设置新的临时变量
			chuzhancalculate(2, 3);
			create("*", stacksyl.top(), stackpos.top(), temp1, temp2, Epos + 100);
			stacksyl.pop();
			stackpos.pop();
			stacksyl.push(21);
			stackpos.push(Epos);
			break;
		case 103:/*E->(E)*/
			cout << "E->(E)规约" << endl;
			stacksyl.pop();
			stackpos.pop();
			Epos = stackpos.top();
			chuzhancalculate(2, 3);
			stacksyl.push(21);
			stackpos.push(Epos);
			break;
		case 104:/*E->i*/
			cout << "E->i规约" << endl;
			Epos=stackpos.top();
			Bstack.pop();
			break;
		}
		tempsyl0 = 21;
		temppos0 = Epos;
		lrcalculate(id);
	}
	if (lr1 == acc && !Bstack.empty())
		if (Bstack.top() == 1)/*归约A->i:=E*/
		{
			cout << "A->i:=E规约" << endl;
			create(":=", stacksyl.top(), stackpos.top(), -1, 0, temppos[0]);
			while (!stacksyl.empty()) stacksyl.pop();
			while (!stackpos.empty()) stackpos.pop();
			while (!Bstack.empty()) Bstack.pop();
		}
}
void lrprogram()
{
	int i1=0;
	if (test(nowsyl))//遇到布尔表达式或赋值语句
	{
		if (programsyl.top()==4)sign = 2;//是while语句
		else
		{
			if (programsyl.top() == 1)sign = 3;//是if语句
			else sign = 1;
		}
		do//循环读入以获取整个布尔表达式/赋值语句并保存到专用数组
		{
			tempsyl[i1]=nowsyl;
			temppos[i1]=nowloc;
			nowloc++;
			nowsyl = saveword[nowloc].id;
			i1++;
		} while (test(nowsyl));
		tempsyl[i1]=20;//单赋值/布尔语句
		nowloc--;
		while (!stacksyl.empty()) stacksyl.pop();
		while (!stackpos.empty()) stackpos.pop();
		stacksyl.push(20);//状态栈初始化
		stackpos.push(0);//状态栈初始化
		if (sign == 1)//是赋值语句，从专用数组":="后开始进行lr分析（即算数表达式进行lr分析）
		{
			cout << "遇到赋值语句：";
			for (int sim = 0; sim < i1; sim++)
			{
				cout << saveword[temppos[sim]].save << ' ';
			}
			cout<<"开始分析：" << endl;
			while (!Bstack.empty()) Bstack.pop();
			while (!stacksyl.empty()) stacksyl.pop();
			while (!stackpos.empty()) stackpos.pop();
			Bstack.push(0);
			stackpos.push(0);
			stacksyl.push(20);
			tempsyl0=tempsyl[2];
			temppos0=temppos[2];
			lrcalculate(2);
			cout << "赋值语句语法分析正确！" << endl;
			nowsyl = 24;
		}
		else if (sign == 2 || sign == 3)//是布尔表达式
		{
			cout << "遇到布尔表达式：";
			for (int sim = 0; sim < i1; sim++)
			{
				cout << saveword[temppos[sim]].save << ' ';
			}
			cout << "开始分析：" << endl;
			pointmark++;
			labelmark[pointmark].nxq = nxq;
			//cout << "labelmark[" << pointmark << "].nxq=" << labelmark[pointmark].nxq << endl;
			while (!Bstack.empty()) Bstack.pop();
			while (!stacksyl.empty()) stacksyl.pop();
			while (!stackpos.empty()) stackpos.pop();
			Bstack.push(0);
			stackpos.push(0);
			stacksyl.push(20);
			tempsyl0 = tempsyl[0];
			temppos0 = temppos[0];
			lrbool(0);
			cout << "布尔表达式语法分析正确！" << endl;
			labelmark[pointmark].tc = ntab2[label - 1].tc;
			labelmark[pointmark].fc = ntab2[label - 1].fc;
			//cout << "labelmark.[" << pointmark << "].tc=" << labelmark[pointmark].tc << endl;
			//cout << "labelmark.[" << pointmark << "].fc=" << labelmark[pointmark].fc << endl;
			backpatch(labelmark[pointmark].tc, nxq);
			//cout << "backpatch(labelmark[" << pointmark << "].tc=" << labelmark[pointmark].tc << ',' << nxq << ')' << endl;
			nowsyl = 27;
		}
	}
	lr = action[programpos.top()][changeprogram(nowsyl)];
	cout << "主程序种别编码栈顶元素stack["<<programsyl.size()-1<<"]="<<programsyl.top()<<"	当前分析单词种别编码="<<nowsyl << "	当前状态=" << programpos.top() <<"	lrprogram="<<lr<<endl;
	if (lr < 19 && lr >= 0)/*移进*/
	{
		programpos.push(lr);
		programsyl.push(nowsyl);
		nowloc++;
		nowsyl = saveword[nowloc].id;
		lrprogram();
	}
	if (lr <= 106 && lr >= 100)/*规约*/
	{
		switch (lr)
		{
		case 100:break;/*S'->S*/
		case 101:
			cout << "S -> if e then S else S 规约" << endl;
			chuzhan(6, 22);
			//cout << programsyl.top() << endl;
			fexp[labeltemp[pointtemp]].result = nxq;
			//cout << "labeltemp["<<pointtemp<<"]=" << labeltemp[pointtemp] << endl;
			//cout << "fexp[labeltemp[pointtemp]].result" << fexp[labeltemp[pointtemp]].result << endl;
			pointtemp--;
			if (programsyl.top() == 2)
			{
				create("j", -1, 0, -1, 0, 0);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout<<"programsyl.top() == 2 "<< " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
				pointtemp++;
				labeltemp[pointtemp] = nxq - 1;
				//cout << "labeltemp[" << pointtemp << "]=" << labeltemp[pointtemp] << endl;
			}
			pointmark--;
			if (programsyl.top() == 6)
			{
				create("j", -1, 0, -1, 0, labelmark[pointmark].nxq);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 6 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
			}
			break;
		case 102:
			cout << "S -> while e do S 规约" << endl;
			chuzhan(4, 22);
			pointmark--;
			//cout << programsyl.top() << endl;
			if (programsyl.top() == 6)
			{
				create("j", -1, 0, -1, 0, labelmark[pointmark].nxq);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 6 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
			}
			if (programsyl.top() == 2)
			{
				create("j", -1, 0, -1, 0, 0);
				fexp[labelmark[pointmark].fc].result = nxq;
				//cout << "programsyl.top() == 2 " << " pointmark=" << pointmark << "labelmark[pointmark].fc" << labelmark[pointmark].fc << endl;
				//cout << "fexp[labelmark[pointmark].fc].result" << fexp[labelmark[pointmark].fc].result << endl;
				pointtemp++;
				labeltemp[pointtemp] = nxq - 1;
				//cout << "labeltemp[" << pointtemp << "]=" << labeltemp[pointtemp] << endl;
			}
			break;
		case 103:
			cout << "S -> begin L end 规约" << endl; 
			chuzhan(3, 22);
			//cout << programsyl.top() << endl;
			if (programsyl.top() == 2)
			{
				create("j", -1, 0, -1, 0, 0);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 2 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
				pointtemp++;
				labeltemp[pointtemp] = nxq - 1;
				//cout << "labeltemp[" << pointtemp << "]=" << labeltemp[pointtemp] << endl;
			}
			if (programsyl.top() == 6)
			{
				create("j", -1, 0, -1, 0, labelmark[pointmark].nxq);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 6 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
			}
			break;
		case 104:
			cout << "S -> a 规约" << endl;
			chuzhan(1, 22);
			//cout << programsyl.top() << endl;
			if (programsyl.top() == 2)
			{
				create("j", -1, 0, -1, 0, 0);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 2 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;
				pointtemp++;
				labeltemp[pointtemp] = nxq - 1;
				//cout << "labeltemp[" << pointtemp << "]=" << labeltemp[pointtemp] << endl;
			}
			if (programsyl.top() == 6)
			{
				create("j", -1, 0, -1, 0, labelmark[pointmark].nxq);
				backpatch(labelmark[pointmark].fc, nxq);
				//cout << "programsyl.top() == 6 " << " backpatch(labelmark[" << pointmark << "].fc=" << labelmark[pointmark].fc << ',' << nxq << ')' << endl;

			}
			break;
		case 105:
			cout << "L -> S 规约" << endl;
			chuzhan(1, 23);
			break;
		case 106:
			cout << "L -> S ; L 规约" << endl;
			chuzhan(3, 23);
			break;
		}
		nowloc--;
		lrprogram();		
	}
}
int changebool(int id)
{
	switch (id)
	{
	case 18:case 19:return 0;//变量、整型
	case 17:return 1;//rop
	case 15:return 2;//(
	case 16:return 3;//)
	case 12:return 4;//!
	case 13:return 5;//&&
	case 14:return 6;//||
	case 20:return 7;//#
	case 21:return 8;//B
	case 25:return 9;//A
	case 26:return 10;//O
	}
}
int changecalculate(int id)
{
	switch (id)
	{
	case 18:case 19:return 0;//变量、整型
	case 10:return 1;//+
	case 9:return 2;//*
	case 15:return 3;//(
	case 16:return 4;//)
	case 20:return 5;//#
	case 21:return 6;//E
	}
}

void huibian()
{
	int QQ = 100;
	int tttt;
	printf("\n 目标代码如下： \n");
	printf("data ends;                 数据段定义结束\n");
	printf(";****************************************\n");
	printf("code segment;              定义代码段\n");
	printf("main proc far;             程序的执行部分\n");
	printf("             assume cs:code,ds:data\n");
	printf("start:\n");
	printf("                            ;为返回操作系统入栈\n");
	printf("push ds\n");
	printf("sub bx, bx\n");
	printf("push bx\n");
	printf("                            ;设置DS段为当前数据段\n");
	printf("mov bx, data\n");
	printf("mov ds, bx\n");

	int temp2 = 100;
	for (temp2 = 100; temp2 < nxq; temp2++, QQ++)
	{
		printf("%d : \n", QQ);
		if (fexp[temp2].op[0] == '+')
		{
			cout<<"               MOV     AX,"<< saveword[fexp[temp2].apos].save <<"         ;把在存储器中的被加数赋给结果寄存器\n";
			if (fexp[temp2].bsyl == 18)
				cout<<"               ADD     AX," << saveword[fexp[temp2].bpos].save << "          ;把被加数和加数立即数相加\n";
			else
			{
				if (fexp[temp2].bsyl == 19)
					cout<<"               ADD     AX," << fexp[temp2].bpos << "          ;把被加数和加数立即数相加\n";
				else
				{
					if (fexp[temp2].bsyl == 21)
						cout<<"               ADD     BX,AX           ;把被加数和在寄存器中的加数相加\n"<<fexp[temp2].bpos;
				}
			}
		}
		if (fexp[temp2].op[0] == '-')
		{
			cout<<"               MOV     AX," << saveword[fexp[temp2].apos].save << "          ;把在存储器中的被减数赋给结果寄存器\n";
			if (fexp[temp2].bsyl == 18)
				cout<<"               SUB     AX," << saveword[fexp[temp2].bpos].save << "          ;把被减数和减数相减\n";
			else
			{
				if (fexp[temp2].bsyl == 19)
					cout<<"               SUB     AX," << fexp[temp2].bpos << "          ;把被减数和减数相减\n";
				else
				{
					if (fexp[temp2].bsyl == 21)
						cout<<"               SUB     BX,AX           ;把被减数和在寄存器中的减数相加\n"<<fexp[temp2].bpos;
				}
			}
		}
		if (fexp[temp2].op[0] == '*')
		{
			cout<<"               MUL        " << saveword[fexp[temp2].apos].save << "          ;把被乘数和存储器中的加数相乘\n";
		}
		if (fexp[temp2].op[0] == '/')
		{
			cout<<"               DIV        " << saveword[fexp[temp2].apos].save << "          ;把被除数和存储器中的数相除\n";
		}


		if (fexp[temp2].op[0] == 'j')
		{
			if (fexp[temp2].op[1] == '>')
			{
				cout<<"               MOV     AX," << saveword[fexp[temp2].apos].save << "          ;条件跳转指令的第一操作数读入寄存器\n";
				cout<<"               CMP     AX," << saveword[fexp[temp2].bpos].save << "          ;条件跳转指令的第一操作数和第二操作数相减\n";
				if (fexp[temp2].op[2] == '=')
				{
					cout<<"               JGE      " << fexp[temp2].result << "            ;大于或等于则跳转\n";
				}
				else
					cout<<"               JG      " << fexp[temp2].result << "             ;大于则跳转\n";
			}
			else if (fexp[temp2].op[1] == '=')
			{
				cout<<"               MOV     AX," << saveword[fexp[temp2].apos].save << "          ;条件跳转指令的第一操作数读入寄存器\n";
				cout<<"               CMP     AX," << saveword[fexp[temp2].bpos].save << "          ;条件跳转指令的第一操作数和第二操作数相减\n";
				cout<<"               JE      " << fexp[temp2].result << "             ;相等则跳转\n";
			}
			else if (fexp[temp2].op[1] == '<')
			{
				cout<<"               MOV     AX," << saveword[fexp[temp2].apos].save << "          ;条件跳转指令的第一操作数读入寄存器\n";
				cout<<"               CMP     AX," << saveword[fexp[temp2].bpos].save << "          ;条件跳转指令的第一操作数和第二操作数相减\n";
				if (fexp[temp2].op[2] == '=')
				{
					cout<<"               JLE      " << fexp[temp2].result << "        ;小于或等于则跳转\n";
				}
				else
					cout<<"               JL      " << fexp[temp2].result << "          ;小于则跳转\n";
			}
			else
			{
				cout<<"               JMP      " << fexp[temp2].result << "             ;产生直接跳转指令\n";
			}
		}
		if (fexp[temp2].op[0] == ':')
		{
			cout<<"               MOV     BX,AX          ;执行赋值语句，寄存器中的值赋给结果变量\n";
			cout<<"               MOV     " << saveword[fexp[temp2].result].save << ",BX          ;在跳出基本快之前保存寄存器中已改变的变量\n";
		}
	}

	printf("ret\n");
}

int main()
{
	scanner();
	printscanner();
	programpos.push(0);
	programsyl.push(20);
	saveword[sum].id = 20;//最后一个#结尾
	nowloc = 0;
	nowsyl = saveword[nowloc].id;
	cout << "词法分析结束，接下来进行语法分析、语义分析：" << endl;
	lrprogram();
	if (lr == acc)cout << "程序语法分析、语义分析结束且正确！" << endl;
	cout << "该语法的四元式序列如下：" << endl;
	printsiyuanshi();
	cout << nxq << endl;
	huibian();
}

