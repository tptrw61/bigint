#include "BigInt.h"
#include "Helper.h"
#include <vector>
#include <stdexcept>


BigInt BigInt::fromStr(const std::string& s) {
	if (!isInt(s)) throw new std::invalid_argument("arg is not an integer");
	if (s == "0" || s == "-0") return 0;
	int i = 0;
	bool sign = POSITIVE;
	BigInt x;
	if (s[0] == '-') {
		sign = NEGATIVE;
		i = 1;
	}
	x = s[i++] - '0';
	for (; i < (int)s.size(); i++) {
		x *= 10;
		x += s[i] - '0';
	}
	x.sign(sign);
	return x;
}

//class functions
BigInt::BigInt() {
	m_bytes.push_back(0);
	m_sign = false;
}

BigInt::BigInt(int64_t x) {
	if (x > 0) {
		m_bytes = numToBytes(x);
		m_sign = false;
	} else if (x < 0) {
		m_bytes = numToBytes(-x);
		m_sign = true;
	} else {
		m_bytes.push_back(0);
		m_sign = false;
	}
	reduce();
}

BigInt::BigInt(const std::string& s) {
	*this = fromStr(s);
}

BigInt::BigInt(BigInt&& other) {
	m_bytes = other.m_bytes;
	m_sign = other.m_sign;
}

BigInt::BigInt(const BigInt& other) {
	m_bytes = other.m_bytes;
	m_sign = other.m_sign;
}

std::vector<byte>& BigInt::bytes() {
	return m_bytes;
}

const std::vector<byte>& BigInt::bytes() const {
	return m_bytes;
}

void BigInt::reduce() {
	removeLeadingZeros(m_bytes);
}

void BigInt::swap(BigInt& other) {
	m_bytes.swap(other.m_bytes);
	bool temp = m_sign;
	m_sign = other.m_sign;
	other.m_sign = temp;
}

size_t BigInt::size() const {
	return m_bytes.size();
}

bool BigInt::bit(int i) const {
	if (i < 0) throw new std::out_of_range("index must be positive or zero");
	int byteIndex = i / 8;
	int bitIndex = i % 8;
	if (byteIndex >= (int)size()) {
		return false;
	}
	return (m_bytes[byteIndex] & (1 << bitIndex)) != 0;
}
bool BigInt::bit(int i, bool value) {
	if (i < 0) throw new std::out_of_range("index must be positive or zero");
	int byteIndex = i / 8;
	int bitIndex = i % 8;
	if (byteIndex >= (int)size()) {
		resizeTo(m_bytes, byteIndex+1);
	}
	if (value) {
		m_bytes[byteIndex] |= 0x01 << bitIndex;
	} else {
		m_bytes[byteIndex] &= ~(0x01 << bitIndex);
	}
	reduce();
	if (IS_ZERO(m_bytes)) {
		m_sign = POSITIVE;
	}
	return value;
}

BigInt& BigInt::operator=(int64_t x) {
	if (x > 0) {
		m_bytes = numToBytes(x);
		m_sign = false;
	} else if (x < 0) {
		m_bytes = numToBytes(-x);
		m_sign = true;
	} else {
		m_bytes.clear();
		m_bytes.push_back(0);
		m_sign = false;
	}
	reduce();
	return *this;
}
BigInt& BigInt::operator=(const std::string& s) {
	return *this = fromStr(s);
}
BigInt& BigInt::operator=(const BigInt& rhs) {
	m_bytes = rhs.m_bytes;
	m_sign = rhs.m_sign;
	return *this;
}
BigInt& BigInt::operator=(BigInt&& rhs) {
	m_bytes = rhs.m_bytes;
	m_sign = rhs.m_sign;
	return *this;
}

BigInt& BigInt::operator<<=(int n) {
	while (n >= 8) {
		m_bytes.insert(m_bytes.begin(), 0);
		n -= 8;
	}
	byte b, next;
	resizeBy(m_bytes, 1);
	for (int i = 0; i < n; i++) {
		b = 0;
		for (int j = 0; j < (int)size(); j++) {
			next = m_bytes[j] >> 7;
			m_bytes[j] <<= 1;
			m_bytes[j] |= b;
			b = next;
		}
	}
	reduce();
	return *this;
}
BigInt BigInt::operator<<(int n) const {
	BigInt x(*this);
	return x <<= n;
}

BigInt& BigInt::operator>>=(int n) {
	while (n >= 8) {
		m_bytes.erase(m_bytes.begin());
		n -= 8;
		if (m_bytes.size() == 0) {
			m_bytes.push_back(0);
			m_sign = 0;
			return *this;
		}
	}
	byte b, next;
	uint16_t help;
	for (int i = 0; i < n; i++) {
		b = 0;
		for (int j = size() - 1; j >= 0; j--) {
			help = m_bytes[j];
			help <<= 7;
			next = (byte)help;
			m_bytes[j] >>= 1;
			m_bytes[j] |= b;
			b = next;
		}
	}
	reduce();
	if (IS_ZERO(m_bytes)) {
		m_sign = 0;
	}
	return *this;
}
BigInt BigInt::operator>>(int n) const {
	BigInt x(*this);
	return x >>= n;
}

bool BigInt::isEven() const {
	return (m_bytes[0] & 0x01) == 0x00;
}
bool BigInt::isOdd() const {
	return (m_bytes[0] & 0x01) == 0x01;
}

bool BigInt::sign() const {
	return m_sign;
}
bool BigInt::sign(bool _sign) {
	if (IS_ZERO(m_bytes)) {
		return 0;
	}
	return m_sign = _sign;
}

// this should be getSign/setSign
int BigInt::getSign() const {
	//1 for positive, -1 for negative, 0 for zero
	if (IS_ZERO(m_bytes)) return 0;
	return m_sign ? -1 : 1;
}
/*
int BigInt::setSign(int _sign) { //TODO:fix to use int arg
	m_sign = _sign;
	if (IS_ZERO(m_bytes))
		m_sign = false;
	return sign();
}
// */

BigInt BigInt::operator-() const {
	BigInt x(*this);
	x.sign(!m_sign);
	return x;
}

BigInt& BigInt::operator++() {
	//reduce();
	if (IS_ZERO(m_bytes)) {
		m_bytes[0] = 1;
		return *this;
	}
	if (m_sign && IS_ONE(m_bytes)) {
		m_sign = 0;
		m_bytes[0] = 0;
		return *this;
	}
	resizeBy(m_bytes, 1);
	if (m_sign) {
		addVectors(m_bytes, ones(m_bytes.size()));
	} else {
		addOne(m_bytes);
	}
	reduce();
	return *this;
}

BigInt BigInt::operator++(int) {
	BigInt x(*this);
	++(*this);
	return x;
}

BigInt& BigInt::operator--() {
	if (IS_ZERO(m_bytes)) {
		m_bytes[0] = 1;
		m_sign = 1;
		return *this;
	}
	resizeBy(m_bytes, 1);
	if (m_sign) {
		addOne(m_bytes);
		reduce();
	} else {
		addVectors(m_bytes, ones(m_bytes.size()));
		reduce();
	}
	return *this;
}

BigInt BigInt::operator--(int) {
	BigInt x(*this);
	--(*this);
	return x;
}

BigInt& BigInt::operator+=(const BigInt& rhs) {
	int max = MAX(m_bytes.size(), rhs.m_bytes.size()) + 1;
	std::vector<byte> r = rhs.m_bytes;
	resizeTo(m_bytes, max);
	resizeTo(r, max);
	if (m_sign == rhs.m_sign) {
		addVectors(m_bytes, r);
		reduce();
	} else {
		if (compareVectors(m_bytes, r) >= 0) {
			twosCompliment(r);
			addVectors(m_bytes, r);
		} else {
			twosCompliment(m_bytes);
			addVectors(m_bytes, r);
			m_sign = rhs.m_sign;
		}
		reduce();
		if (IS_ZERO(m_bytes))
			m_sign = false;
	}
	return *this;
}

BigInt BigInt::operator+(const BigInt& rhs) const {
	BigInt x(*this);
	x += rhs;
	return x;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
	int max = MAX(m_bytes.size(), rhs.m_bytes.size()) + 1;
	std::vector<byte> r = rhs.m_bytes;
	resizeTo(m_bytes, max);
	resizeTo(r, max);
	if (m_sign == !rhs.m_sign) {
		addVectors(m_bytes, r);
		reduce();
	} else {
		if (compareVectors(m_bytes, r) >= 0) {
			twosCompliment(r);
			addVectors(m_bytes, r);
		} else {
			twosCompliment(m_bytes);
			addVectors(m_bytes, r);
			m_sign = !rhs.m_sign;
		}
		reduce();
		if (IS_ZERO(m_bytes))
			m_sign = false;
	}
	return *this;
}

BigInt BigInt::operator-(const BigInt& rhs) const {
	BigInt x(*this);
	x -= rhs;
	return x;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
	*this = *this * rhs;
	return *this;
}

BigInt BigInt::operator*(const BigInt& rhs) const {
	if (IS_ZERO(m_bytes) || IS_ZERO(rhs.m_bytes))
		return 0;
	BigInt a, big, small;
	if (compareVectors(bytes(), rhs.bytes()) > 0) {
		big = *this;
		small = rhs;
	} else {
		big = rhs;
		small = *this;
	}
	big.m_sign = small.m_sign = POSITIVE;
	while (!IS_ZERO(small.bytes())) {
		if (small.isOdd()) {
			a += big;
		}
		small >>= 1;
		big <<= 1;
	}
	if (m_sign != rhs.m_sign) {
		a.m_sign = 1;
	} else {
		a.m_sign = 0;
	}
	a.reduce();
	return a;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
	return *this = divmod(rhs).first;
}
BigInt BigInt::operator/(const BigInt& rhs) const {
	return divmod(rhs).first;
}

BigInt& BigInt::operator%=(const BigInt& rhs) {
	return *this = divmod(rhs).second;
}
BigInt BigInt::operator%(const BigInt& rhs) const {
	return divmod(rhs).second;
}

/*
if D = 0 then error(DivisionByZeroException) end
Q := 0                  -- Initialize quotient and remainder to zero
R := 0                     
for i := n − 1 .. 0 do  -- Where n is number of bits in N
  R := R << 1           -- Left-shift R by 1 bit
  R(0) := N(i)          -- Set the least-significant bit of R equal to bit i of the numerator
  if R ≥ D then
    R := R − D
    Q(i) := 1
  end
end
*/
std::pair<BigInt, BigInt> BigInt::divmod(const BigInt& rhs) const {
	if (IS_ZERO(rhs.m_bytes)) throw new std::domain_error("divide by zero");
	if (IS_ZERO(bytes())) return std::pair<BigInt, BigInt>(0, 0);
	BigInt q, r, n = *this, d = rhs;
	n.sign(POSITIVE);
	d.sign(POSITIVE);
	for (int i = size() * 8 - 1; i >= 0; i--) {
		r <<= 1;
		r.bit(0, n.bit(i));
		if (r >= d) {
			r -= d;
			q.bit(i, 1);
		}
	}
	if (sign() && !rhs.sign()) {
		++q;
		q.sign(NEGATIVE);
		r = d - r;
	} else if (!sign() && rhs.sign()) {
		q.sign(NEGATIVE);
	} else if (sign() && rhs.sign()) {
		++q;
		r = d - r;
	}
	return std::pair<BigInt, BigInt>(q, r);
}

BigInt BigInt::pow(uint32_t exp) const {
	if (exp == 0 && IS_ZERO(m_bytes)) throw new std::domain_error("0^0 is undefined");
	if (IS_ZERO(m_bytes)) return 0;
	if (exp == 0) return 1;
	if (exp == 1) return *this;
	if (exp == 2) return (*this) * (*this);
	if (exp == 3) return (*this) * (*this) * (*this);
	std::vector<uint32_t> walk({1});
	std::vector<BigInt> mult;
	mult.push_back(*this);
	uint32_t n = 2;
	BigInt x = *this * *this;
	while (n <= exp) {
		walk.push_back(n);
		mult.push_back(x);
		n *= 2;
		x *= x;
	}
	for (uint32_t i = walk.size() - 2; walk.back() != exp; i--) {
		n = walk.back() + walk[i];
		if (n <= exp) {
			walk.push_back(n);
			mult.push_back(mult.back() * mult[i]);
		}
	}
	return mult.back();
}

bool BigInt::operator==(const BigInt& rhs) const {
	if (m_sign != rhs.m_sign)
		return false;
	return compareVectors(m_bytes, rhs.m_bytes) == 0;
}
bool BigInt::operator!=(const BigInt& rhs) const {
	return !(*this == rhs);
}
bool BigInt::operator>(const BigInt& rhs) const {
	if (m_sign && !rhs.m_sign)
		return false;
	if (!m_sign && rhs.m_sign)
		return true;
	if (!m_sign)
		return compareVectors(m_bytes, rhs.m_bytes) > 0;
	return compareVectors(m_bytes, rhs.m_bytes) < 0; //mult by -1 flips inequality
}
bool BigInt::operator>=(const BigInt& rhs) const {
	if (m_sign && !rhs.m_sign)
		return false;
	if (!m_sign && rhs.m_sign)
		return true;
	if (!m_sign)
		return compareVectors(m_bytes, rhs.m_bytes) >= 0;
	return compareVectors(m_bytes, rhs.m_bytes) <= 0; //mult by -1 flips inequality
}
bool BigInt::operator<(const BigInt& rhs) const {
	if (m_sign && !rhs.m_sign)
		return true;
	if (!m_sign && rhs.m_sign)
		return false;
	if (!m_sign)
		return compareVectors(m_bytes, rhs.m_bytes) < 0;
	return compareVectors(m_bytes, rhs.m_bytes) > 0; //mult by -1 flips inequality
}
bool BigInt::operator<=(const BigInt& rhs) const {
	if (m_sign && !rhs.m_sign)
		return true;
	if (!m_sign && rhs.m_sign)
		return false;
	if (!m_sign)
		return compareVectors(m_bytes, rhs.m_bytes) <= 0;
	return compareVectors(m_bytes, rhs.m_bytes) >= 0; //mult by -1 flips inequality
}

//TODO: complete this method
std::string BigInt::str() const {
	BigInt n = *this;
	std::pair<BigInt, BigInt> ans;
	n.sign(POSITIVE);
	std::string s;
	std::string lookup = "0123456789";
	while (n != 0) {
		ans = n.divmod(10);
		s = lookup[ans.second.bytes()[0]] + s;
		n = ans.first;
	}
	if (sign())
		s = '-' + s;
	return s;
}

std::ostream& operator<<(std::ostream& os, const BigInt& v) {
	return os << v.str();
}

BigInt operator+(int64_t lhs, const BigInt& rhs) {
	return rhs + lhs;
}
BigInt operator-(int64_t lhs, const BigInt& rhs) {
	return BigInt(lhs) - rhs;
}
BigInt operator*(int64_t lhs, const BigInt& rhs) {
	return rhs * lhs;
}