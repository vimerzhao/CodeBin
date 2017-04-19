#include <iostream>
#include <string>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <map>
using namespace std;
bool isInOPTR(char ch){
	switch (ch)	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '#':{
			return true;
			break;
		}
		default:{
			return false;
			break;
		}
	}
}
char Precede(char a, char b){
	map<char, int> myMap;
	myMap['+'] = 0;
	myMap['-'] = 1;
	myMap['*'] = 2;
	myMap['/'] = 3;
	myMap['('] = 4;
	myMap[')'] = 5;
	myMap['#'] = 6;
	char R[7][7] =  {{'>','>','<','<','<','>','>'},
					{'>','>','<','<','<','>','>'},
					{'>','>','>','>','<','>','>'},
					{'>','>','>','>','<','>','>'},
					{'<','<','<','<','<','=','#'},
					{'>','>','>','>','#','>','>'},
					{'<','<','<','<','<','#','='}};
	if ('#' == R[myMap[a]][myMap[b]]){
		printf("Precede ERROE\n");
		printf("%c%c\n", a, b);
		exit(-1);
	}
	return R[myMap[a]][myMap[b]];
}
float Caculate(float a, char theta, float b){
	switch (theta){
		case '+':{
			return a+b;
			break;
		}
		case '-':{
			return a-b;
			break;
		}
		case '*':{
			return a*b;
			break;
		}
		case '/':{
			return a/b;
		}
		default:{
			printf("Operator ERROR\n");
			exit(-1);
		}
	}
}
void expression(void){
	string Str;
	getline(cin, Str);
	cout <<"Your input: " + Str + '\b' + ' ' << endl;
	char Operator = ' ';
	float Operand = 0;
	stack<char> OPTR;
	stack<float> OPND;
	OPTR.push('#');
	int i = 0;
	char number[20];
	while (Operator != '#' || OPTR.top() != '#'){
		//fool code!!!
		for (int k = 0;i <= Str.length();){
			if (isInOPTR(Str.at(i)))
			{
				Operator = Str.at(i++);
				break;
			}
			else
			{
				number[k] = Str.at(i);
				k++;
				i++;
				if (isInOPTR(Str.at(i)))
				{
					number[k] = '\0';
					Operand = atof(number);
					OPND.push(Operand);
					continue;
				}

			}
		}
		switch (Precede(OPTR.top(), Operator)){
			case '<':{
				OPTR.push(Operator);
				break;
			}
			case '=':{
				OPTR.pop();
				break;
			}
			case '>':{
				float a, b;
				char theta;
				theta = OPTR.top();
				OPTR.pop();
				b = OPND.top();
				OPND.pop();
				a = OPND.top();
				OPND.pop();
				OPND.push(Caculate(a, theta, b));
				i--;//attention!!!!!!!
				break;
			}
		}
	}
	cout << "Answer: " <<OPND.top() << endl;

}
int main(int argc, char const *argv[])
{
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	for (int i = 0; i < 5; ++i)
	{
		expression();
	}
	return 0;
}