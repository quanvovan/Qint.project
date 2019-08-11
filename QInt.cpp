#include "QInt.h"


QInt::QInt()
{
	fill(bytes, bytes + QINT_SIZE, 0);
}

QInt::~QInt()
{
}

QInt QInt::zero()
{
	static QInt staticZero;
	static bool zeroInit = false;
	if (!zeroInit)
	{
		fill(staticZero.bytes, staticZero.bytes + QINT_SIZE, 0);
		zeroInit = true;
	}

	return staticZero;
}

QInt QInt::one()
{
	static QInt staticOne;
	static bool oneInit = false;
	if (!oneInit)
	{
		fill(staticOne.bytes, staticOne.bytes + QINT_SIZE, 0);
		oneInit = true;
	}
	staticOne.bytes[QINT_SIZE - 1] = 1;

	return staticOne;
}

//Nhap QInt theo co so
void QInt::scanQInt(string src, int base)
{
	int bitPos = 0;
	switch (base)
	{
	case 2:
		*this = QInt::zero();
		for (int i = src.length() - 1; i >= 0; i--)
		{
			setBit(bitPos, src[i] == '1');
			bitPos++;
		}
		break;
	case 10:
		*this = decToBin(src);
		break;
	case 16:
		*this = hexToBin(src);
	}
}

//Xuat QInt theo co so
void QInt::printQInt(int base)
{
	string res;
	switch (base)
	{
	case 2:
		res = toBinStr();
		while (res[0] == '0' && res.length() > 1)
			res.erase(0, 1);
		cout << res;
		break;
	case 10:
		cout << binToDec(*this);
		break;
	case 16:
		cout << binToHex(*this);
		break;
	}
}

QInt plusQInt(const QInt & A, const QInt & B)
{
	QInt res;
	bool carry = 0;

	for (int i = 0; i <= QINT_SIZE * 8 - 1; i++)
	{
		int temp = A.getBit(i) + B.getBit(i) + carry;
		switch (temp)
		{
		case 0: //carry=0; bitA=0; bitB=0;
			carry = 0;
			res.setBit(i, 0);
			break;
		case 1: //carry=0; bitA+bitB=1 || carry=1; bitA=0; bitB=0;
			carry = 0;
			res.setBit(i, 1);
			break;
		case 2:	// carry=1; bitA+bitB=1; || carry=0; bitA=1; bitB=1;
			carry = 1;
			res.setBit(i, 0);
			break;
		case 3:	//carry=1; bitA=1; bitB=1;
			carry = 1;
			res.setBit(i, 1);
		}
	}
	return res;
}

// dich phai so hoc
QInt QInt::shiftRight(int nums) const
{
	//bool msb = this->getBit(QINT_SIZE * 8 - 1); // Luu lai bit trai nhat.
	//QInt res = (*this) >> nums; // Dich logic.
	//res.setBit(QINT_SIZE * 8 - 1, msb); // gan lai bit trai nhat.
	//return res;

	QInt res = *this;
	while (nums > 0)
	{
		bool msb = res.getBit(QINT_SIZE * 8 - 1);
		for (int i = QINT_SIZE - 1; i >= 1; i--)
		{
			res.bytes[i] = res.bytes[i] >> 1;
			if (res.bytes[i - 1] & 1)
			{
				res.bytes[i] = ((1 << 7) | res.bytes[i]);
			}
		}
		res.bytes[0] = res.bytes[0] >> 1;
		res.setBit(QINT_SIZE * 8 - 1, msb);
		nums--;
	}
	return res;
}

// Ham phu bo tro toan tu / 2 so QInt
void QInt::divide(const QInt & divisor, QInt & div, QInt & mod) const
{
	mod = QInt::zero();
	div = *this;
	QInt M = divisor;
	// Luu lai dau cua phep chia.
	bool isDNegative = div.isNegative();
	bool isMNegative = M.isNegative();
	if (div.getBit(QINT_SIZE * 8 - 1) == true) // Bao dam div va M luon >= 0
	{
		QInt A;
		//div = A-div;
		div = plusQInt(A, div.toInverse());
	}
	if (M.getBit(QINT_SIZE * 8 - 1) == true)
	{
		QInt A;
		//M = A-M;
		M = plusQInt(A, M.toInverse());
	}
	for (int i = 0; i < QINT_SIZE * 8; i++) // Thuc hien chia theo thuat toan trong tai lieu.
	{
		mod = mod << 1;
		mod.setBit(0, div.getBit(QINT_SIZE * 8 - 1)); // Copy MSB cua Q vao LSB cua A.
		div = div << 1;
		//QInt tmp = mod - M;
		QInt tmp = plusQInt(mod, M.toInverse());
		if (tmp.getBit(QINT_SIZE * 8 - 1) == false) // tmp >= 0
		{
			mod = tmp;
			div.setBit(0, 1);
		}
	}

	if (isDNegative)		//So du va so bi chia cung dau
		mod = mod.toInverse();
	if (isDNegative + isMNegative == 1) //Neu so bi chia va so chia trai dau thi doi dau thuong
		div = div.toInverse();
}

QInt QInt::operator+(const QInt & B) const
{
	QInt res = plusQInt(*this, B);
	bool isANegative = this->isNegative();
	bool isBNegative = B.isNegative();

	if ((isANegative + isBNegative != 1) && (res.isNegative() != isANegative)) //(A, B cung dau) va (res trai dau A)
	{
		throw "OVERFLOW!";
	}

	return res;
}

QInt QInt::operator-(const QInt & B) const
{
	//A-B = A+(-B)
	QInt res = plusQInt(*this, B.toInverse());
	bool isANegative = this->isNegative();
	bool isInvBNegative = !(B.isNegative());

	if ((isANegative + isInvBNegative != 1) && (res.isNegative() != isANegative)) //(A, -B cung dau) va (res trai dau A)
	{
		throw "OVERFLOW!";
	}

	return res;
}

// Nhan 2 so QInt su dung thuat toan Booth
QInt QInt::operator*(const QInt & B) const
{
	if (*this == QInt::zero() || B == QInt::zero())
	{
		return QInt();
	}

	//Truong hop tran so dac biet cua phep nhan
	QInt min;
	min.setBit(QINT_SIZE * 8 - 1, 1);
	QInt negOne;
	negOne.scanQInt("-1", 10);
	if ((*this == min && B == negOne) || (*this == negOne && B == min))
	{
		throw "OVERFLOW!";
	}

	QInt A;
	QInt Q = B;
	bool P = false; // Gan Q_0 = 0 theo tai lieu

	for (int i = 0; i < QINT_SIZE * 8; i++)
	{
		bool lsbQ = Q.getBit(0); // lay LSB cua Q

		if (lsbQ == false && P == true) // 01
			//A = A + *this;
			A = plusQInt(A, *this);
		if (lsbQ == true && P == false) // 10
			//A = A - *this;
		{
			QInt temp = *this;
			A = plusQInt(A, temp.toInverse());
		}
		// Shift
		P = lsbQ;
		Q = Q >> 1;

		Q.setBit(QINT_SIZE * 8 - 1, A.getBit(0));
		A = A.shiftRight(1);
	}
	// check overflow
	if ((Q / B) != *this)
	{
		throw "OVERFLOW!";
		return QInt();
	}

	return Q;
}

// Toan tu chia 2 so QInt
QInt QInt::operator/(const QInt & B) const
{
	//TH1: So chia bang 0
	if (B == QInt::zero())
	{
		throw "DIVIDE BY ZERO!";
	}
	//TH2: So chia bang 1 hoac so bi chia bang 0
	if (B == QInt::one() || *this==QInt::zero())
	{
		return *this;
	}

	//TH3: Truong hop tran so duy nhat cua phep chia
	QInt min;
	min.setBit(QINT_SIZE * 8 - 1, 1);
	QInt negOne;
	negOne.scanQInt("-1", 10);
	if (*this == min && B == negOne)
	{
		throw "OVERFLOW!";
		return QInt();
	}

	QInt div; // ket qua phep chia
	QInt mod; // so du
	this->divide(B, div, mod);

	return div;
}

QInt QInt::operator&(const QInt & B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res.bytes[i] = this->bytes[i] & B.bytes[i];
	}
	return res;
}

QInt QInt::operator|(const QInt & B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res.bytes[i] = this->bytes[i] | B.bytes[i];
	}
	return res;
}

QInt QInt::operator^(const QInt & B) const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res.bytes[i] = this->bytes[i] ^ B.bytes[i];
	}
	return res;
}

//Overload toan tu dao bit
QInt QInt::operator~() const
{
	QInt res;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res.bytes[i] = ~this->bytes[i];
	}
	return res;
}

QInt QInt::operator>>(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		for (int i = QINT_SIZE - 1; i >= 1; i--)
		{
			res.bytes[i] = res.bytes[i] >> 1;
			if (res.bytes[i - 1] & 1)
			{
				res.bytes[i] = ((1 << 7) | res.bytes[i]);
			}
		}
		res.bytes[0] = res.bytes[0] >> 1;
		nums--;
	}
	return res;
}

QInt QInt::operator<<(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		for (int i = 0; i < QINT_SIZE - 1; i++)
		{
			res.bytes[i] = res.bytes[i] << 1;
			if ((res.bytes[i + 1] >> 7) & 1)
			{
				res.bytes[i] = (1 | res.bytes[i]);
			}
		}
		res.bytes[QINT_SIZE - 1] = res.bytes[QINT_SIZE - 1] << 1;
		nums--;
	}
	return res;
}

QInt QInt::rol(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		bool additive = ((res.bytes[0] >> 7) & 1);
		for (int i = 0; i < QINT_SIZE - 1; i++)
		{
			res.bytes[i] = res.bytes[i] << 1;
			if ((res.bytes[i + 1] >> 7) & 1)
			{
				res.bytes[i] = (1 | res.bytes[i]);
			}
		}
		res.bytes[QINT_SIZE - 1] = res.bytes[QINT_SIZE - 1] << 1;
		if (additive)
			res.bytes[QINT_SIZE - 1] = (1 | res.bytes[QINT_SIZE - 1]);
		nums--;
	}
	return res;
}

QInt QInt::ror(int nums) const
{
	QInt res = *this;
	while (nums > 0)
	{
		bool additive = (res.bytes[QINT_SIZE-1] & 1);
		for (int i = QINT_SIZE - 1; i >= 1; i--)
		{
			res.bytes[i] = res.bytes[i] >> 1;
			if (res.bytes[i - 1] & 1)
			{
				res.bytes[i] = ((1 << 7) | res.bytes[i]);
			}
		}
		res.bytes[0] = res.bytes[0] >> 1;
		if (additive)
			res.bytes[0] = ((1 << 7) | res.bytes[0]);
		nums--;
	}
	return res;
}

bool QInt::operator>(const QInt & B) const
{
	bool isANegative = this->isNegative();
	bool isBNegative = B.isNegative();

	if (isANegative + isBNegative == 1) //A va B trai dau nhau
	{
		if (isANegative)
			return false;
		else return true;
	}

	for (int i = 0; i < QINT_SIZE; i++)
	{
		//A > B
		if (this->bytes[i] > B.bytes[i])
			return true;
		//A < B
		if (this->bytes[i] < B.bytes[i])
			return false;
	}

	//A = B
	return false;
}

bool QInt::operator<(const QInt & B) const
{
	bool isANegative = this->isNegative();
	bool isBNegative = B.isNegative();

	if (isANegative + isBNegative == 1) //A va B trai dau nhau
	{
		if (isANegative)
			return true;
		else return false;
	}

	for (int i = 0; i < QINT_SIZE; i++)
	{
		//A > B
		if (this->bytes[i] > B.bytes[i])
			return false;
		//A < B
		if (this->bytes[i] < B.bytes[i])
			return true;
	}

	//A = B
	return false;
}

//Overload toan tu ==
bool QInt::operator==(const QInt & B) const
{
	for (int i = 0; i < QINT_SIZE; i++)
	{
		if (this->bytes[i] != B.bytes[i])
			return false;
	}
	return true;
}

bool QInt::operator!=(const QInt & B) const
{
	return !(*this == B);
}

//Overload toan tu >=
bool QInt::operator>=(const QInt & B) const
{
	return (!(*this < B));
}

//Overload toan tu <=
bool QInt::operator<=(const QInt & B) const
{
	return (!(*this > B));
}

//Overload toan tu =
QInt & QInt::operator=(const QInt & B)
{
	for (int i = 0; i < QINT_SIZE; i++)
	{
		this->bytes[i] = B.bytes[i];
	}
	return *this;
}

//Lay bit
bool QInt::getBit(int pos) const
{
	int bytePos = QINT_SIZE - 1 - pos / 8;
	int bitPos = pos % 8;
	return ((this->bytes[bytePos] >> bitPos) & 1);
}

//Gan bit
void QInt::setBit(int pos, bool value)
{
	int bytePos = QINT_SIZE - 1 - pos / 8;
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

//Kiem tra so am
bool QInt::isNegative() const
{
	return ((this->getBit(QINT_SIZE * 8 - 1)) == 1);
}

//Doi dau
QInt QInt::toInverse() const
{
	if (*this == QInt::zero())
		return *this;

	QInt res;

	//Dao bit
	res = ~(*this);

	//Cong 1
	for (int i = QINT_SIZE - 1; i >= 0; i--)
	{
		if (res.bytes[i] < 255)
		{
			res.bytes[i] += 1;
			break;
		}
		else
			res.bytes[i] = 0;
	}

	//Khong can danh dau bit trai nhat, vi moi truong hop so am luon co bit trai nhat = 1

	return res;
}

string QInt::toBinStr() const
{
	string res = "";
	for (int i = 0; i < QINT_SIZE; i++)
	{
		res += bitset<8>(this->bytes[i]).to_string();
	}
	return res;
}

string divByTwo(string src)
{
	string res = "";

	int additive = 0;
	for (int i = 0; i < src.length(); i++)
	{
		int digit = src[i] - '0';
		res += ((digit / 2 + additive) + '0');
		additive = ((digit % 2 == 0) ? 0 : 5);
	}

	if (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}

string mulByTwo(string src, int additive)
{
	string res = "";
	int carry = additive;

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

QInt decToBin(string src)
{
	QInt res;
	bool isNegative = false;
	if (src[0] == '-')
	{
		isNegative = true;
		src.erase(0, 1);
	}

	string binStr = "";
	do
	{
		int lastDigit = src[src.length() - 1] - '0';
		binStr += ((lastDigit % 2 == 0) ? '0' : '1');
		src = divByTwo(src);
	} while (src != "0");

	reverse(binStr.begin(), binStr.end());
	res.scanQInt(binStr, 2);

	if (isNegative)
		res = res.toInverse();
	return res;
}

QInt hexToBin(string src)
{
	QInt res;
	string binStr = "";
	for (int i = 0; i < src.length(); i++)
	{
		int pos = ((string)HEX_CHARS).find(src[i], 0);
		binStr += bitset<4>(pos).to_string();
	}
	res.scanQInt(binStr, 2);
	return res;
}

string binToDec(const QInt & src)
{
	string res = "";
	if (src == QInt::zero())
	{
		res = "0";
		return res;
	}

	QInt posSrc = src;
	if (src.isNegative())
	{
		posSrc = posSrc.toInverse();
	}

	string binStr = posSrc.toBinStr();
	int pos = binStr.find('1', 0) + 1;
	res = "1";
	while (pos < binStr.length())
	{
		int additive = binStr[pos] - '0';
		res = mulByTwo(res, additive);
		pos++;
	}

	if (src.isNegative())
		res = '-' + res;

	return res;
}

string binToHex(const QInt & src)
{
	string res = "";
	for (int i = 0; i < QINT_SIZE; i++)
	{
		unsigned char lBits, rBits;
		lBits = (src.bytes[i] >> 4);
		rBits = (src.bytes[i] & 0x0F);
		res += HEX_CHARS[lBits];
		res += HEX_CHARS[rBits];
	}
	while (res[0] == '0' && res.length() > 1)
		res.erase(0, 1);
	return res;
}
