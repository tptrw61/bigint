#pragma once
#include <vector>
#include <stdint.h>
#include <string>

class BigInt {
public:
	using byte = uint8_t;
	static const bool POSITIVE = false;
	static const bool NEGATIVE = true;
private:
	std::vector<byte> m_bytes;
	bool m_sign;

public:
	BigInt();
	BigInt(int64_t x);
	BigInt(BigInt&& other);
	BigInt(const BigInt& other);

	//TODO: assignment with equal sign

	//some helper stuff
	std::vector<byte>& bytes();
	const std::vector<byte>& bytes() const;
	void reduce();
	void swap(BigInt& other);
	size_t size();

	//assignment
	BigInt& operator=(int64_t x);
	BigInt& operator=(const BigInt& rhs);
	BigInt& operator=(BigInt&& rhs);

	//binary stuff
	//void invertSize(int bytes); //??? idk what this was supposed to do

	BigInt& operator<<=(int n);
	BigInt operator<<(int n) const;

	BigInt& operator>>=(int n);
	BigInt operator>>(int n) const;

	//TODO: &, |, ^, ~

	//math stuff

	bool isEven() const;
	bool isOdd() const;

	//boolean versions
	bool sign() const;
	bool sign(bool _sign);

	//int versions
	//int getSign() const;
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
};

