#include "QFloat.h"



QFloat::QFloat()
{
	fill(bytes, bytes + QFLOAT_SIZE, 0);
}

QFloat::~QFloat()
{
}

//done
void QFloat::scanQFloat(string src, int base)
{
	if (base == 2)
	{
		fill(bytes, bytes + QFLOAT_SIZE, 0);
		for (int i = 0; i < src.length(); i++)
		{
			setBit(QFLOAT_SIZE * 8 - 1 - i, src[i] == '1');
		}
	}
	else if (base == 10)
	{
		*this = decToBinF(src);
	}
}

//done
string QFloat::printQFloat(int base)
{
	if (base == 2)
	{
		return toBinStrF();
	}
	else if (base == 10)
	{
		return binToDecF(*this);
	}
}

//done
bool QFloat::getBit(int pos) const
{
	int bytePos = QFLOAT_SIZE - 1 - pos / 8;
	int bitPos = pos % 8;
	return ((this->bytes[bytePos] >> bitPos) & 1);
}

void QFloat::setBit(int pos, bool value)
{
	int bytePos = QFLOAT_SIZE - 1 - pos / 8;
	int bitPos = pos % 8;
	if (value)
	{
		this->bytes[bytePos] = ((1 << bitPos) | this->bytes[bytePos]);
	}
	else
	{
		this->bytes[bytePos] = ((~(1 << bitPos))&this->bytes[bytePos]);
	}
}

QFloat & QFloat::operator=(const QFloat & B)
{
	for (int i = 0; i < QFLOAT_SIZE; i++)
	{
		this->bytes[i] = B.bytes[i];
	}
	return *this;
}

string QFloat::toBinStrF() const
{
	string res = "";
	for (int i = 0; i < QFLOAT_SIZE; i++)
	{
		res += bitset<8>(this->bytes[i]).to_string();
	}
	return res;
}

string mulByTwo(string src)
{
	string res = "";
	int carry = 0;

	for (int i = src.length() - 1; i >= 0; i--)
	{
		int temp = src[i] - '0';
		temp = temp * 2 + carry;
		res += (temp % 10 + '0');
		carry = temp / 10;
	}
	if (carry > 0)
		res += (carry + '0');

	reverse(res.begin(), res.end());

	return res;
}

bool isFullOf(string src, char c)
{
	for (int i = 0; i < src.length(); i++)
		if (src[i] != c)
			return false;
	return true;
}

string mulByHalf(string src, bool additive)
{
	string res = "";
	int carry = 0;

	for (int i = src.length() - 1; i >= 0; i--)
	{
		int temp = src[i] - '0';
		temp = temp * 5 + carry;
		res += (temp % 10 + '0');
		carry = temp / 10;
	}
	res += (carry + '0');

	reverse(res.begin(), res.end());
	if (additive)
	{
		res[0] = res[0] + 5;
	}

	while (res.length() > 1 && res[res.length() - 1] == '0')
		res.erase(res.length() - 1, 1);

	return res;
}

QFloat decToBinF(string src)
{
	QFloat res;
	if (src == "0")
		return res;
	//Xet dau
	if (src[0] == '-')
	{
		res.setBit(SIGN_BIT, 1);
		src.erase(0, 1);
	}

	//Neu chuoi la so nguyen (khong co dau .) thi chuan hoa ve dang A.0
	if (src.find('.', 0) == string::npos)
		src += ".0";

	int dotPos = src.find('.', 0);
	string strInt = src.substr(0, dotPos);
	string strFrac = src.substr(dotPos + 1, src.length() - dotPos - 1);
	
	string strExp = "";
	string strSignif = "";

	//Neu phan nguyen !=0
	if (strInt != "0")
	{
		string binInt = "";
		string binFrac = "";

		QInt qInt;
		qInt.scanQInt(strInt, 10);
		binInt = qInt.toBinStr();
		while (binInt.length() > 0)
			if (binInt[0] == '0')
				binInt.erase(0, 1);
			else break;

		int count = 0;
		while (count < NUMS_OF_SIGNIF)
		{
			string newStrFrac = mulByTwo(strFrac);
			if (newStrFrac.length() > strFrac.length())
			{
				binFrac += '1';
				strFrac = newStrFrac.substr(1, strFrac.length());
			}
			else
			{
				binFrac += '0';
				strFrac = newStrFrac;
			}
			if (isFullOf(strFrac, '0'))
				break;
			count++;
		}

		unsigned int exponent = binInt.length() - 1;
		if (exponent > 0)
			strSignif = binInt.substr(1, exponent) + binFrac;
		else
			strSignif = binFrac;
		exponent += BIAS;

		strExp = bitset<NUMS_OF_EXP>(exponent).to_string();
		strSignif = strSignif.substr(0, NUMS_OF_SIGNIF);
	}
	else
	{
		//Tim exponent
		unsigned int exponent = 0;
		while (true)
		{
			exponent++;
			string newStrFrac = mulByTwo(strFrac);
			if (newStrFrac.length() > strFrac.length())
			{
				strFrac = newStrFrac.substr(1, strFrac.length());
				break;
			}
			else
			{

				strFrac = newStrFrac;
			}
		}
	
		exponent = BIAS - exponent;
		strExp = bitset<NUMS_OF_EXP>(exponent).to_string();

		//Tim significant
		int count = 0;
		while (count < NUMS_OF_SIGNIF)
		{
			string newStrFrac = mulByTwo(strFrac);
			if (newStrFrac.length() > strFrac.length())
			{
				strSignif += '1';
				strFrac = newStrFrac.substr(1, strFrac.length());
			}
			else
			{
				strSignif += '0';
				strFrac = newStrFrac;
			}
			if (isFullOf(strFrac, '0'))
				break;
			count++;
		}
	}

	//Gan exponent bit cho res
	for (int i = 0; i < strExp.length(); i++)
	{
		res.setBit(SIGN_BIT-1-i,strExp[i]=='1');
	}

	//Gan significant bit cho res
	for (int i = 0; i < strSignif.length(); i++)
	{
		res.setBit(EXP_BIT - 1 - i, strSignif[i] == '1');
	}
	return res;
}

string binToDecF(const QFloat& src)
{
	string res = "";

	string binStr = src.toBinStrF();
	string strExp = binStr.substr(1, 15);
	string strSignif = binStr.substr(16, 112);
	//Xet dau
	string sign = "";
	if (binStr[0] == '1')
		sign = "-";
	
	//Xet cac truong hop dac biet
	if (isFullOf(strExp, '0'))
	{
		if (isFullOf(strSignif, '0'))
		{
			return string("0");
		}
		else
		{
			return string("DENORMALIZED");
		}
	}

	if (isFullOf(strExp, '1'))
	{
		if (isFullOf(strSignif, '0'))
		{
			return string("INFINITY");
		}
		else
		{
			return string("NaN");
		}
	}

	//Tim exponent
	unsigned int uexp = stoi(strExp, nullptr, 2);
	int exp = uexp - BIAS;

	//Tim phan nguyen, phan thap phan o co so 2
	string binInt = "1";
	string binFrac = strSignif;
	while (exp != 0)
	{
		if (exp < 0)
		{
			binFrac = binInt[binInt.length() - 1] + binFrac;
			binInt.erase(binInt.length() - 1, 1);
			binInt = '0' + binInt;
			exp++;
		}
		else
		{
			binInt += binFrac[0];
			binFrac.erase(0, 1);
			binFrac += '0';
			exp--;
		}
	}
	while (binInt.length() > 1 && binInt[0] == '0')
		binInt.erase(0, 1);
	while (binFrac.length() > 1 && binFrac[binFrac.length() - 1] == '0')
		binFrac.erase(binFrac.length() - 1, 1);

	//Chuyen doi phan nguyen sang he 10
	string strInt = "";
	QInt qInt;
	qInt.scanQInt(binInt, 2);
	strInt = binToDec(qInt);

	//Chuyen doi phan thap phan sang he 10
	string strFrac = "";
	int pos = binFrac.find_last_of('1');

	//Kiem tra phan thap phan co khac 0 hay khong
	if (pos != string::npos)
	{
		strFrac = "5";
		for (int i = pos - 1; i >= 0; i--)
		{
			strFrac = mulByHalf(strFrac, binFrac[i] == '1');
		}
		strFrac = '.' + strFrac;
	}

	res = sign + strInt + strFrac;

	return res;
}
