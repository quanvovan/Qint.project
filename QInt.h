#pragma once
#include <iostream>
#include <string>
#include <bitset>
using namespace std;

#define QINT_SIZE 16
#define HEX_CHARS "0123456789ABCDEF"

class QInt
{
private:
	unsigned char bytes[QINT_SIZE];
public:
	QInt();
	~QInt();

	static QInt zero();
	static QInt one();


	void scanQInt(string src, int base);
	void printQInt(int base);

	friend QInt decToBin(string src);
	friend QInt hexToBin(string src);
	friend string binToDec(const QInt& src);
	friend string binToHex(const QInt& src);

	//Calculate A+B without checking overflow
	friend QInt plusQInt(const QInt& A, const QInt& B);
	void divide(const QInt & divisor, QInt & div, QInt & mod) const;
	QInt shiftRight(int nums) const;

	QInt operator+ (const QInt& B) const;
	QInt operator- (const QInt& B) const;
	QInt operator* (const QInt& B) const;
	QInt operator/ (const QInt& B) const;

	QInt operator& (const QInt& B) const;
	QInt operator| (const QInt& B) const;
	QInt operator^ (const QInt& B) const;
	QInt operator~ () const;

	QInt operator>> (int nums) const;
	QInt operator<< (int nums) const;
	QInt rol(int nums) const;
	QInt ror(int nums) const;

	bool operator> (const QInt& B) const;
	bool operator< (const QInt& B) const;
	bool operator== (const QInt& B) const;
	bool operator!= (const QInt& B) const;
	bool operator>= (const QInt& B) const;
	bool operator<= (const QInt& B) const;
	QInt& operator= (const QInt& B);

	bool getBit(int pos) const;
	void setBit(int pos, bool value);

	bool isNegative() const;
	QInt toInverse() const;

	string toBinStr() const;
};

QInt decToBin(string src);
QInt hexToBin(string src);
string binToDec(const QInt& src);
string binToHex(const QInt& src);
QInt plusQInt(const QInt& A, const QInt& B);