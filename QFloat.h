#pragma once
#include "QInt.h"

#define QFLOAT_SIZE 16
#define SIGN_BIT 127
#define EXP_BIT 112
#define SIGNIF_BIT 0
#define NUMS_OF_SIGNIF 112
#define NUMS_OF_EXP 15
#define BIAS 16383

class QFloat
{
private:
	unsigned char bytes[QFLOAT_SIZE];
public:
	QFloat();
	~QFloat();

	void scanQFloat(string src, int base);
	string printQFloat(int base);

	bool getBit(int pos) const;
	void setBit(int pos, bool value);

	QFloat& operator= (const QFloat& B);
	string toBinStrF() const;

	friend QFloat decToBinF(string src);
	friend string binToDecF(const QFloat& src);
};

QFloat decToBinF(string src);
string binToDecF(const QFloat& src);

