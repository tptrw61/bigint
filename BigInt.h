#pragma once
#include <vector>
#include <stdint.h>
#include <string>
#include <ostream>

class BigInt {
public:
	using byte = uint8_t;

	//for use with sign() and sign(bool)
	static const bool POSITIVE = false;
	static const bool NEGATIVE = true;
private:
	std::vector<byte> m_bytes;
	bool m_sign;

	static BigInt fromStr(const std::string& s);
public:
	BigInt();
	BigInt(int64_t x);
	BigInt(const std::string& s);
	BigInt(BigInt&& other);
	BigInt(const BigInt& other);

	//TODO: assignment with equal sign

	//some helper stuff
	std::vector<byte>& bytes();
	const std::vector<byte>& bytes() const;
	void reduce();
	void swap(BigInt& other);
	size_t size() const;

	bool bit(int i) const;
	bool bit(int i, bool value);

	//assignment
	BigInt& operator=(int64_t x);
	BigInt& operator=(const std::string& s);
	BigInt& operator=(const BigInt& rhs);
	BigInt& operator=(BigInt&& rhs);

	//binary stuff
	//increases size to 'bytes' bytes then inverts with ~
	//BigInt invertAtLength(int bytes); 

	BigInt& operator<<=(int n);
	BigInt operator<<(int n) const;

	BigInt& operator>>=(int n);
	BigInt operator>>(int n) const;

	//TODO: &, |, ^, ~

	//math stuff

	bool isEven() const;
	bool isOdd() const;

	BigInt pos() const;
	BigInt neg() const;

	//boolean versions
	bool sign() const;
	bool sign(bool _sign);

	//int versions
	int getSign() const;
	//int setSign(int _sign);

	BigInt operator-() const;

	BigInt& operator++();    //prefix
	BigInt operator++(int);  //postfix
	BigInt& operator--();
	BigInt operator--(int);

	BigInt& operator+=(const BigInt& rhs);
	BigInt operator+(const BigInt& rhs) const;

	BigInt& operator-=(const BigInt& rhs);
	BigInt operator-(const BigInt& rhs) const;

	//TODO: division, modulus, divmod
	BigInt& operator*=(const BigInt& rhs);
	BigInt operator*(const BigInt& rhs) const;

	BigInt& operator/=(const BigInt& rhs);
	BigInt operator/(const BigInt& rhs) const;

	BigInt& operator%=(const BigInt& rhs);
	BigInt operator%(const BigInt& rhs) const;

	std::pair<BigInt, BigInt> divmod(const BigInt& rhs) const;

	//
	BigInt pow(uint32_t exp) const;

	//comparison
	bool operator==(const BigInt& rhs) const;
	bool operator!=(const BigInt& rhs) const;

	bool operator>(const BigInt& rhs) const;
	bool operator>=(const BigInt& rhs) const;

	bool operator<(const BigInt& rhs) const;
	bool operator<=(const BigInt& rhs) const;

	//to string
	std::string str() const;

	friend std::ostream& operator<<(std::ostream& os, const BigInt& v);
	
	//friend versions of operators +, -, *
	friend BigInt operator+(int64_t lhs, const BigInt& rhs);
	friend BigInt operator-(int64_t lhs, const BigInt& rhs);
	friend BigInt operator*(int64_t lhs, const BigInt& rhs);
};

