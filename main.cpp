#define _CRT_SECURE_NO_WARNINGS
#include "QInt.h"
#include "QFloat.h"
#include <vector>
#include <sstream>
#include <conio.h>

void processQInt(string strIn)
{
	vector<string> unaryOpr= { "~" };
	vector<string> binaryOpr= { "+"," - ","*","/","<",">","<=",">=","==","&","|","^","<<",">>","rol","ror" };

	int oprType = 0;
	int argCount = 1;
	//Dem so luong tham so trong chuoi
	for (int i = 0; i < strIn.length(); i++)
	{
		if (strIn[i] == ' ')
			argCount++;
	}
	//Tim toan tu 1 ngoi trong chuoi
	for (int i = 0; i < unaryOpr.size(); i++)
	{
		if (strIn.find(unaryOpr[i], 0)!=string::npos)
		{
			oprType = 1;
			break;
		}
	}
	//Tim toan tu 2 ngoi trong chuoi
	if (oprType == 0)
	{
		for (int i = 0; i < binaryOpr.size(); i++)
		{
			if (strIn.find(binaryOpr[i], 0) != string::npos)
			{
				oprType = 2;
				break;
			}
		}
	}
	//Neu khong tim duoc thi trong chuoi khong chua toan tu

	string p1, p2, opr, opdA, opdB;
	stringstream ssIn;

	ssIn << strIn;
	ssIn >> p1;
	//Kiem tra co chua chi thi p2 hay khong
	if (argCount - oprType == 3)
		ssIn >> p2;
	else
		p2 = p1;
	//Kiem tra loai toan tu
	opr = "";
	switch (oprType)
	{
	case 0:	//Khong co toan tu
		ssIn >> opdA;
		break;
	case 1: //Toan tu 1 ngoi
		ssIn >> opr >> opdA;
		break;
	case 2:	//Toan tu 2 ngoi
		ssIn >> opdA >> opr >> opdB;
		break;
	}

	
	//Thuc hien tinh toan va xuat ket qua
	QInt A, B;
	int base1 = stoi(p1);
	int base2 = stoi(p2);
	try
	{
		A.scanQInt(opdA, base1);
		if (opr == "")
		{
			A.printQInt(base2);
		}
		else if (opr == "~")
		{
			(~A).printQInt(base2);
		}
		else if (opr == "+")
		{
			B.scanQInt(opdB, base1);
			(A + B).printQInt(base2);
		}
		else if (opr == "-")
		{
			B.scanQInt(opdB, base1);
			(A - B).printQInt(base2);
		}
		else if (opr == "*")
		{
			B.scanQInt(opdB, base1);
			(A * B).printQInt(base2);
		}
		else if (opr == "/")
		{
			B.scanQInt(opdB, base1);
			(A / B).printQInt(base2);
		}
		else if (opr == "&")
		{
			B.scanQInt(opdB, base1);
			(A & B).printQInt(base2);
		}
		else if (opr == "|")
		{
			B.scanQInt(opdB, base1);
			(A | B).printQInt(base2);
		}
		else if (opr == "^")
		{
			B.scanQInt(opdB, base1);
			(A ^ B).printQInt(base2);
		}
		else if (opr == "<<")
		{
			int nums = stoi(opdB);
			(A << nums).printQInt(base2);
		}
		else if (opr == ">>")
		{
			int nums = stoi(opdB);
			(A.shiftRight(nums)).printQInt(base2);
		}
		else if (opr == "rol")
		{
			int nums = stoi(opdB);
			(A.rol(nums)).printQInt(base2);
		}
		else if (opr == "ror")
		{
			int nums = stoi(opdB);
			(A.ror(nums)).printQInt(base2);
		}
		else if (opr == "<")
		{
			B.scanQInt(opdB, base1);
			cout << ((A < B) ? "TRUE" : "FALSE");
		}
		else if (opr == ">")
		{
			B.scanQInt(opdB, base1);
			cout << ((A > B) ? "TRUE" : "FALSE");
		}
		else if (opr == "<=")
		{
			B.scanQInt(opdB, base1);
			cout << ((A <= B) ? "TRUE" : "FALSE");
		}
		else if (opr == ">=")
		{
			B.scanQInt(opdB, base1);
			cout << ((A >= B) ? "TRUE" : "FALSE");
		}
		else if (opr == "==")
		{
			B.scanQInt(opdB, base1);
			cout << ((A == B) ? "TRUE" : "FALSE");
		}

	}
	catch (const char* msg)
	{
		cout << msg;
	}
	cout << endl;
}

void processQFloat(string strIn)
{
	string p1, p2, strA;

	stringstream ssIn;
	ssIn << strIn;
	ssIn >> p1 >> p2 >> strA;

	int base1 = stoi(p1);
	int base2 = stoi(p2);
	QFloat A;

	try
	{
		A.scanQFloat(strA, base1);
		A.printQFloat(base2);
	}
	catch (const char* msg)
	{
		cout << msg;
	}
	cout << endl;
}

int main(int argc, char** argv)
{
	string inPath, outPath, mode;
	/*Only for debug
	argc = 4;
	inPath = "input.txt";
	outPath = "output.txt";
	mode = "1";
	*/
	if (argc < 4)
		cout << "Du lieu khong hop le!";
	else
	{
		inPath = string(argv[1]);
		outPath = string(argv[2]);
		mode = string(argv[3]);

		freopen(inPath.c_str(), "r", stdin);
		freopen(outPath.c_str(), "w", stdout);


		while (!cin.eof())
		{
			string inStr;
			getline(cin, inStr);
			processQFloat(inStr);
		}
	}
	return 0;
}