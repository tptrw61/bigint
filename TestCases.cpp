#include "catch.hpp"
#include "BigInt.h"
#include "Helper.h"
#include <vector>
#include <stdint.h>

#define MACRO_LABEL "[.][macro]"
#define HELPER_LABEL "[helper]"
#define BIGINT_LABEL "[bigint]"

//helper test cases
TEST_CASE("GET_BYTE macro", MACRO_LABEL) {
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
TEST_CASE("MAX macro", MACRO_LABEL) {
	int a = 10;
	int b = 15;

	REQUIRE((MAX(a, b) == 15));
	REQUIRE((MAX(b, a) == 15));
	REQUIRE((MAX(a, a) == 10));
	REQUIRE((MAX(b, b) == 15));
}
TEST_CASE("IS_ZERO macro", MACRO_LABEL) {
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
TEST_CASE("IS_ONE macro", MACRO_LABEL) {
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