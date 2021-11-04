#include "catch.hpp"
#include "BigInt.h"
#include "Helper.h"
#include <vector>
#include <stdint.h>
#include <sstream>

#define MACRO_LABEL "[.][macro][all]"
#define HELPER_LABEL "[helper][all]"
#define BIGINT_LABEL "[bigint][all]"
#define FRIEND_LABEL "[friend][bigint][all]"

//helper test cases
TEST_CASE("GET_BYTE", MACRO_LABEL) {
	SECTION("int32") {
		int32_t x = 0x9834abcd;
		REQUIRE((GET_BYTE(x, 0) == 0xcd));
		REQUIRE((GET_BYTE(x, 1) == 0xab));
		REQUIRE((GET_BYTE(x, 2) == 0x34));
		REQUIRE((GET_BYTE(x, 3) == 0x98));
	}
	SECTION("uint32") {
		uint32_t x = 0x9834abcd;
		REQUIRE((GET_BYTE(x, 0) == 0xcd));
		REQUIRE((GET_BYTE(x, 1) == 0xab));
		REQUIRE((GET_BYTE(x, 2) == 0x34));
		REQUIRE((GET_BYTE(x, 3) == 0x98));
	}
	SECTION("int64") {
		int64_t x = 0x9876543210abcdef;
		REQUIRE((GET_BYTE(x, 0) == 0xef));
		REQUIRE((GET_BYTE(x, 1) == 0xcd));
		REQUIRE((GET_BYTE(x, 2) == 0xab));
		REQUIRE((GET_BYTE(x, 3) == 0x10));
		REQUIRE((GET_BYTE(x, 4) == 0x32));
		REQUIRE((GET_BYTE(x, 5) == 0x54));
		REQUIRE((GET_BYTE(x, 6) == 0x76));
		REQUIRE((GET_BYTE(x, 7) == 0x98));
	}
	SECTION("uint64") {
		uint64_t x = 0x9876543210abcdef;
		REQUIRE((GET_BYTE(x, 0) == 0xef));
		REQUIRE((GET_BYTE(x, 1) == 0xcd));
		REQUIRE((GET_BYTE(x, 2) == 0xab));
		REQUIRE((GET_BYTE(x, 3) == 0x10));
		REQUIRE((GET_BYTE(x, 4) == 0x32));
		REQUIRE((GET_BYTE(x, 5) == 0x54));
		REQUIRE((GET_BYTE(x, 6) == 0x76));
		REQUIRE((GET_BYTE(x, 7) == 0x98));
	}
}
TEST_CASE("MAX", MACRO_LABEL) {
	int a = 10;
	int b = 15;

	REQUIRE((MAX(a, b) == 15));
	REQUIRE((MAX(b, a) == 15));
	REQUIRE((MAX(a, a) == 10));
	REQUIRE((MAX(b, b) == 15));
}
TEST_CASE("IS_ZERO", MACRO_LABEL) {
	std::vector<byte> v;
	SECTION("normal zero vector") {
		v.push_back(0);

		REQUIRE((IS_ZERO(v)));
	}
	SECTION("nonzero vector") {
		v.push_back(1);

		REQUIRE_FALSE((IS_ZERO(v)));
	}
	SECTION("long zero vector") {
		v.push_back(0);
		v.push_back(0);

		REQUIRE_FALSE((IS_ZERO(v)));
	}
	SECTION("long nonzero vector") {
		v.push_back(0);
		v.push_back(1);

		REQUIRE_FALSE((IS_ZERO(v)));
	}
}
TEST_CASE("IS_ONE", MACRO_LABEL) {
	std::vector<byte> v;
	SECTION("normal 'one' vector") {
		v.push_back(1);

		REQUIRE((IS_ONE(v)));
	}
	SECTION("'non-one' vector") {
		v.push_back(2);

		REQUIRE_FALSE((IS_ONE(v)));
	}
	SECTION("long 'one' vector") {
		v.push_back(1);
		v.push_back(0);

		REQUIRE_FALSE((IS_ONE(v)));
	}
	SECTION("long 'non-one' vector") {
		v.push_back(0);
		v.push_back(1);

		REQUIRE_FALSE((IS_ONE(v)));
	}
}

TEST_CASE("invert", HELPER_LABEL) {
	std::vector<byte> v;
	byte a, b;
	SECTION("part 1") {
		a = 34;
		b = 222;
		v.push_back(a);
		v.push_back(b);

		invert(v);
		REQUIRE(v[0] == (byte)~a);
		REQUIRE(v[1] == (byte)~b);
	}
	SECTION("part 2") {
		a = 0;
		b = 0;
		v.push_back(a);
		v.push_back(b);

		invert(v);
		REQUIRE(v[0] == (byte)~a);
		REQUIRE(v[1] == (byte)~b);
	}
	SECTION("part 3") {
		a = 255;
		b = 255;
		v.push_back(a);
		v.push_back(b);

		invert(v);
		REQUIRE(v[0] == (byte)~a);
		REQUIRE(v[1] == (byte)~b);
	}
}

//TODO: addX

TEST_CASE("addOne", HELPER_LABEL) {
	std::vector<byte> v;
	SECTION("no carry") {
		v.push_back(24);
		v.push_back(80);
		addOne(v);
		REQUIRE(v[0] == 25);
		REQUIRE(v[1] == 80);
	}
	SECTION("one carry") {
		v.push_back(255);
		v.push_back(80);
		addOne(v);
		REQUIRE(v[0] == 0);
		REQUIRE(v[1] == 81);
	}
	SECTION("two carries") {
		v.push_back(255);
		v.push_back(255);
		v.push_back(80);
		addOne(v);
		REQUIRE(v[0] == 0);
		REQUIRE(v[1] == 0);
		REQUIRE(v[2] == 81);
	}
	SECTION("overflow 1 byte") {
		v.push_back(255);
		addOne(v);
		REQUIRE(v[0] == 0);
	}
	SECTION("overflow 2 bytes") {
		v.push_back(255);
		v.push_back(255);
		addOne(v);
		REQUIRE(v[0] == 0);
		REQUIRE(v[1] == 0);
	}
}

TEST_CASE("removeLeadingZeros", HELPER_LABEL) {
	std::vector<byte> v;
	SECTION("zero vector") {
		v.push_back(0);
		removeLeadingZeros(v);
		REQUIRE(v.size() == 1);
		REQUIRE(IS_ZERO(v));
	}
}

TEST_CASE("isInt", HELPER_LABEL) {
	SECTION("positive") {
		REQUIRE(isInt("15498410840654098"));
	}
	SECTION("negative") {
		REQUIRE(isInt("-15498410840654098"));
	}
	SECTION("zero") {
		REQUIRE(isInt("0"));
	}
	SECTION("negative zero") {
		REQUIRE(isInt("-0"));
	}
	SECTION("non-int") {
		REQUIRE_FALSE(isInt("asdfsada"));
	}
	SECTION("minus sign") {
		REQUIRE_FALSE(isInt("-"));
	}
	SECTION("empty string") {
		REQUIRE_FALSE(isInt(""));
	}
	SECTION("partial int") {
		REQUIRE_FALSE(isInt("1234lkjh"));
	}
}

TEST_CASE("operator==", BIGINT_LABEL) {
	SECTION("part 1") {
		BigInt a = 500, b = 500;
		REQUIRE(a == b);
	}
	SECTION("part 2") {
		BigInt a = 50, b = 50;
		REQUIRE(a == b);
	}
	SECTION("part 3") {
		BigInt a = 60, b = 600;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 4") {
		BigInt a = 600, b = 700;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 5") {
		BigInt a = 60, b = 70;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 6") {
		BigInt a = 600, b = 60;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 7") {
		BigInt a = 700, b = 600;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 8") {
		BigInt a = 70, b = 60;
		REQUIRE_FALSE(a == b);
	}
	SECTION("part 9") {
		BigInt a = -50, b = 50;
		CHECK_FALSE(a == b);
		REQUIRE_FALSE(b == a);
	}
	SECTION("part 10") {
		BigInt a = -500, b = 500;
		CHECK_FALSE(a == b);
		REQUIRE_FALSE(b == a);
	}
}

TEST_CASE("operator>/operator<", BIGINT_LABEL) {
	SECTION("part 1") {
		BigInt a = 500, b = 500;
		CHECK_FALSE(a > b);
		CHECK_FALSE(a < b);
		CHECK_FALSE(b > a);
		CHECK_FALSE(b < a);
		CHECK(a >= b);
		CHECK(a <= b);
		CHECK(b >= a);
		REQUIRE(b <= a);
	}
	SECTION("part 2") {
		BigInt a = 50, b = 50;
		CHECK_FALSE(a > b);
		CHECK_FALSE(a < b);
		CHECK_FALSE(b > a);
		CHECK_FALSE(b < a);
		CHECK(a >= b);
		CHECK(a <= b);
		CHECK(b >= a);
		REQUIRE(b <= a);
	}
	SECTION("part 3") {
		BigInt a = 50, b = 500;
		CHECK_FALSE(a > b);
		CHECK(a < b);
		CHECK(b > a);
		CHECK_FALSE(b < a);
		CHECK_FALSE(a >= b);
		CHECK(a <= b);
		CHECK(b >= a);
		REQUIRE_FALSE(b <= a);
	}
	SECTION("part 4") {
		BigInt a = -50, b = 50;
		CHECK_FALSE(a > b);
		CHECK(a < b);
		CHECK(b > a);
		CHECK_FALSE(b < a);
		CHECK_FALSE(a >= b);
		CHECK(a <= b);
		CHECK(b >= a);
		REQUIRE_FALSE(b <= a);
	}
	SECTION("part 5") {
		BigInt a = -50, b = 500;
		CHECK_FALSE(a > b);
		CHECK(a < b);
		CHECK(b > a);
		CHECK_FALSE(b < a);
		CHECK_FALSE(a >= b);
		CHECK(a <= b);
		CHECK(b >= a);
		REQUIRE_FALSE(b <= a);
	}
}

TEST_CASE("operator+", BIGINT_LABEL) {
	SECTION("positive + positive") {
		BigInt a = 1234, b = 2345, c = 1234+2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("negative + positive") {
		BigInt a = -1234, b = 2345, c = -1234+2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("positive + negative") {
		BigInt a = 1234, b = -2345, c = 1234+-2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("negative + negative") {
		BigInt a = -1234, b = -2345, c = -1234+-2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("zero + positive") {
		BigInt a = 0, b = 2345, c = 0+2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("zero + negative") {
		BigInt a = 0, b = -2345, c = 0+-2345;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("positive + zero") {
		BigInt a = 1234, b = 0, c = 1234+0;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("negative + zero") {
		BigInt a = -1234, b = 0, c = -1234+0;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
	SECTION("zero + zero") {
		BigInt a = 0, b = 0, c = 0+0;
		BigInt ab = a + b;
		REQUIRE(ab == c);
	}
}

TEST_CASE("operator-", BIGINT_LABEL) {
	SECTION("positive - positive") {
		BigInt a = 1234, b = 2345, c = 1234-2345;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("negative - positive") {
		BigInt a = -1234, b = 2345, c = -1234-2345;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("positive - negative") {
		BigInt a = 1234, b = -2345, c = 1234-(-2345);
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("negative - negative") {
		BigInt a = -1234, b = -2345, c = -1234-(-2345);
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("zero - positive") {
		BigInt a = 0, b = 2345, c = 0-2345;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("zero - negative") {
		BigInt a = 0, b = -2345, c = 0-(-2345);
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("positive - zero") {
		BigInt a = 1234, b = 0, c = 1234-0;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("negative - zero") {
		BigInt a = -1234, b = 0, c = -1234-0;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
	SECTION("zero - zero") {
		BigInt a = 0, b = 0, c = 0-0;
		BigInt ab = a - b;
		REQUIRE(ab == c);
	}
}

TEST_CASE("operator++", BIGINT_LABEL) {
	SECTION("zero") {
		BigInt a(0);
		BigInt b(1);
		a++;
		REQUIRE(a == b);
	}
	SECTION("positive") {
		BigInt a(1);
		BigInt b(2);
		a++;
		REQUIRE(a == b);
	}
	SECTION("negative") {
		BigInt a(-2);
		BigInt b(-1);
		a++;
		REQUIRE(a == b);
	}
	SECTION("neg one") {
		BigInt a(-1);
		BigInt b(0);
		//CHECK(IS_ONE(a.bytes()));
		//CHECK(a.sign());
		//CHECK(IS_ZERO(b.bytes()));
		//CHECK(!b.sign());
		a++;
		REQUIRE(a == b);
	}
}

TEST_CASE("operator--", BIGINT_LABEL) {
	SECTION("zero") {
		BigInt a(0);
		BigInt b(-1);
		a--;
		REQUIRE(a == b);
	}
	SECTION("positive") {
		BigInt a(3);
		BigInt b(2);
		a--;
		REQUIRE(a == b);
	}
	SECTION("negative") {
		BigInt a(-2);
		BigInt b(-3);
		a--;
		REQUIRE(a == b);
	}
	SECTION("one") {
		BigInt a(1);
		BigInt b(0);
		a--;
		REQUIRE(a == b);
	}
}

//TODO: operator*
TEST_CASE("operator*", BIGINT_LABEL) {
	SECTION("positive * positive") {
		BigInt a = 1234, b = 2345, c = 1234*2345;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("negative * positive") {
		BigInt a = -1234, b = 2345, c = -1234*2345;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("positive * negative") {
		BigInt a = 1234, b = -2345, c = 1234*(-2345);
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("negative * negative") {
		BigInt a = -1234, b = -2345, c = -1234*(-2345);
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("zero * positive") {
		BigInt a = 0, b = 2345, c = 0*2345;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("zero * negative") {
		BigInt a = 0, b = -2345, c = 0*(-2345);
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("positive * zero") {
		BigInt a = 1234, b = 0, c = 1234*0;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("negative * zero") {
		BigInt a = -1234, b = 0, c = -1234*0;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
	SECTION("zero * zero") {
		BigInt a = 0, b = 0, c = 0*0;
		BigInt ab = a * b;
		REQUIRE(ab == c);
	}
}

//TODO: pow

//TODO: operator>> / operator<<
TEST_CASE("operator>>") {
	BigInt a = 12345678789L, b;
	int x;
	SECTION("shift 0") {
		x = 0;
		a >>= x;
		b = 12345678789L >> x;
		REQUIRE(a == b);
	}
	SECTION("shift 1") {
		x = 1;
		a >>= x;
		b = 12345678789L >> x;
		REQUIRE(a == b);
	}
	SECTION("shift 8") {
		x = 8;
		a >>= x;
		b = 12345678789L >> x;
		REQUIRE(a == b);
	}
	SECTION("shift 9") {
		x = 9;
		a >>= x;
		b = 12345678789L >> x;
		REQUIRE(a == b);
	}
	SECTION("shift 69") {
		x = 69;
		a >>= x;
		b = 0;
		REQUIRE(a == b);
	}
}

TEST_CASE("operator<<") {
	BigInt a = 1234L, b;
	int x;
	SECTION("shift 0") {
		x = 0;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
	SECTION("shift 1") {
		x = 1;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
	SECTION("shift 8") {
		x = 8;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
	SECTION("shift 9") {
		x = 9;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
	SECTION("shift 10") {
		x = 10;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
	SECTION("shift 15") {
		x = 15;
		a <<= x;
		b = 1234L << x;
		REQUIRE(a == b);
	}
}

TEST_CASE("operator- (unary)", BIGINT_LABEL) {
	BigInt a = 456321, b = -456321;
	SECTION("pos to neg") {
		REQUIRE(-a == b);
	}
	SECTION("neg to pos") {
		REQUIRE(a == -b);
	}
}

static int64_t loopPow(int64_t base, int exp) {
	int64_t ans = 1;
	for (int i = 0; i < exp; i++)
		ans *= base;
	return ans;
}

TEST_CASE("pow", BIGINT_LABEL) {
	SECTION("0^10") {
		int b = 0;
		int e = 10;
		BigInt base = b;
		BigInt ans = base.pow(e);
		REQUIRE(ans == loopPow(b, e));
	}
	SECTION("10^0") {
		int b = 10;
		int e = 0;
		BigInt base = b;
		BigInt ans = base.pow(e);
		REQUIRE(ans == loopPow(b, e));
	}
	SECTION("1^12") {
		int b = 1;
		int e = 12;
		BigInt base = b;
		BigInt ans = base.pow(e);
		REQUIRE(ans == loopPow(b, e));
	}
	SECTION("12^5") {
		int b = 12;
		int e = 5;
		BigInt base = b;
		BigInt ans = base.pow(e);
		REQUIRE(ans == loopPow(b, e));
	}
}

TEST_CASE("isEven()/isOdd()") {
	BigInt a;
	SECTION("zero") {
		a = 0;
		CHECK(a.isEven());
		REQUIRE_FALSE(a.isOdd());
	}
	SECTION("positive even") {
		a = 1000000;
		CHECK(a.isEven());
		REQUIRE_FALSE(a.isOdd());
	}
	SECTION("negative even") {
		a = -1000000;
		CHECK(a.isEven());
		REQUIRE_FALSE(a.isOdd());
	}
	SECTION("positive odd") {
		a = 1000005;
		CHECK_FALSE(a.isEven());
		REQUIRE(a.isOdd());
	}
	SECTION("negative odd") {
		a = -1000005;
		CHECK_FALSE(a.isEven());
		REQUIRE(a.isOdd());
	}
}

TEST_CASE("bit", BIGINT_LABEL) {
	BigInt a = 5;
	SECTION("bit(index)") {
		CHECK(a.bit(0));
		CHECK_FALSE(a.bit(1));
		CHECK(a.bit(2));
		CHECK_FALSE(a.bit(3));
		REQUIRE_FALSE(a.bit(10));
	}
	SECTION("bit(index, value)") {
		a.bit(0, 0);
		CHECK_FALSE(a.bit(0));
		a.bit(1, 1);
		CHECK(a.bit(1));
		a.bit(2, 0);
		CHECK_FALSE(a.bit(2));
		a.bit(3, 1);
		CHECK(a.bit(3));
		a.bit(10, 1);
		REQUIRE(a.bit(10));
	}
}

TEST_CASE("divmod", BIGINT_LABEL) {
	BigInt n, d, x, q, r;
	int a, b;
	std::pair<BigInt, BigInt> ans;
	SECTION("part 1") {
		a = 7;
		b = 5;
		n = a;
		d = b;
		SECTION("pos DIVMOD pos") {
			q = n / d;
			r = n % d;
			x = d * q + r;
			CHECK(q == (a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD pos") {
			q = -n / d;
			r = -n % d;
			x = d * q + r;
			CHECK(q == -((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
		SECTION("pos DIVMOD neg") {
			q = n / -d;
			r = n % -d;
			x = -d * q + r;
			CHECK(q == -(a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD neg") {
			q = -n / -d;
			r = -n % -d;
			x = -d * q + r;
			CHECK(q == ((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
	}
	SECTION("part 2") {
		a = 55;
		b = 11;
		n = a;
		d = b;
		SECTION("pos DIVMOD pos") {
			q = n / d;
			r = n % d;
			x = d * q + r;
			CHECK(q == (a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD pos") {
			q = -n / d;
			r = -n % d;
			x = d * q + r;
			CHECK(q == -((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
		SECTION("pos DIVMOD neg") {
			q = n / -d;
			r = n % -d;
			x = -d * q + r;
			CHECK(q == -(a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD neg") {
			q = -n / -d;
			r = -n % -d;
			x = -d * q + r;
			CHECK(q == ((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
	}
	SECTION("part 3") {
		a = 456546;
		b = 123;
		n = a;
		d = b;
		SECTION("pos DIVMOD pos") {
			q = n / d;
			r = n % d;
			x = d * q + r;
			CHECK(q == (a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD pos") {
			q = -n / d;
			r = -n % d;
			x = d * q + r;
			CHECK(q == -((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
		SECTION("pos DIVMOD neg") {
			q = n / -d;
			r = n % -d;
			x = -d * q + r;
			CHECK(q == -(a / b));
			CHECK(r == (a % b));
			REQUIRE(n == x);
		}
		SECTION("neg DIVMOD neg") {
			q = -n / -d;
			r = -n % -d;
			x = -d * q + r;
			CHECK(q == ((a / b) + 1));
			CHECK(r == (b - (a % b)));
			REQUIRE(-n == x);
		}
	}
	SECTION("zero DIVMOD pos") {
		q = r = 0;
		q /= 20;
		r %= 20;
		CHECK(q == 0);
		REQUIRE(r == 0);
	}
	SECTION("zero DIVMOD neg") {
		q = r = 0;
		q /= -20;
		r %= -20;
		CHECK(q == 0);
		REQUIRE(r == 0);
	}
}

TEST_CASE("str", BIGINT_LABEL) {
	BigInt x;
	SECTION("positive") {
		x = 1234;
		REQUIRE(x.str() == std::string("1234"));
	}
	SECTION("negative") {
		x = -3211234;
		REQUIRE(x.str() == std::string("-3211234"));
	}
	SECTION("really big") {
		x = 10;
		int p = 30;
		x = x.pow(p);
		CHECK(x.size() > sizeof(int64_t));
		std::string s = "1";
		for (int i = 0; i < p; i++) {
			s += "0";
		}
		REQUIRE(x.str() == s);
	}
}

TEST_CASE("swap", BIGINT_LABEL) {
	BigInt a = 12345;
	BigInt b = -54321;
	a.swap(b);
	CHECK(a.str() == "-54321");
	REQUIRE(b.str() == "12345");
}

TEST_CASE("getSign", BIGINT_LABEL) {
	BigInt x;
	SECTION("positive") {
		x = 123456789;
		REQUIRE(x.getSign() == 1);
	}
	SECTION("positive") {
		x = -123456789;
		REQUIRE(x.getSign() == -1);
	}
	SECTION("zero") {
		x = 0;
		REQUIRE(x.getSign() == 0);
	}
}

TEST_CASE("friend operators", FRIEND_LABEL) {
	SECTION("operator<<") {
		BigInt x = 12345678;
		std::stringstream ss;
		ss << x;
		REQUIRE(ss.str() == x.str());
	}
	SECTION("math") {
		int a = 1234;
		int c = 5678;
		BigInt b = c;
		SECTION("operator+") {
			REQUIRE((a + b) == (a + c));
		}
		SECTION("operator-") {
			REQUIRE((a - b) == (a - c));
		}
		SECTION("operator*") {
			REQUIRE((a * b) == (a * c));
		}
	}
}

TEST_CASE("string constructor/assignment", BIGINT_LABEL) {
	SECTION("constructor") {
		BigInt a("12345");
		BigInt b("-67890");
		BigInt c("0");
		BigInt d("-0");
		CHECK(a == BigInt(12345));
		CHECK(b == BigInt(-67890));
		CHECK(c == 0);
		REQUIRE(d == 0);
	}
	SECTION("assignment") {
		BigInt a, b, c, d;
		a = b = c = d = 0;
		a = "12345";
		b = "-67890";
		c = "0";
		d = "-0";
		CHECK(a == BigInt(12345));
		CHECK(b == BigInt(-67890));
		CHECK(c == 0);
		REQUIRE(d == 0);
	} 
}

TEST_CASE("pos", BIGINT_LABEL) {
	BigInt x, y;
	SECTION("positive") {
		x = y = 1234;
		REQUIRE(x.pos() == y);
	}
	SECTION("negative") {
		x = -1234;
		y = 1234;
		REQUIRE(x.pos() == y);
	}
	SECTION("zero") {
		x = y = 0;
		REQUIRE(x.pos() == y);
	}
}
TEST_CASE("neg", BIGINT_LABEL) {
	BigInt x, y;
	SECTION("positive") {
		x = 1234;
		y = -1234;
		REQUIRE(x.neg() == y);
	}
	SECTION("negative") {
		x = y = -1234;
		REQUIRE(x.neg() == y);
	}
	SECTION("zero") {
		x = y = 0;
		REQUIRE(x.neg() == y);
	}
}