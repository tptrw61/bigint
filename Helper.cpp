#include "Helper.h"
#include <ctype.h>

void invert(std::vector<byte>& bytes) {
	for (int i = 0; i < (int)bytes.size(); i++) {
		bytes[i] = ~bytes[i];
	}
}
void addX(std::vector<byte>& bytes, byte x) {
	byte carry = x;
	uint16_t sum;
	for (int i = 0; i < (int)bytes.size(); i++) {
		sum = bytes[i] + carry;
		carry = GET_BYTE(sum, 1);
		bytes[i] = GET_BYTE(sum, 0);
		if (carry == 0)
			break;
	}
}
void addOne(std::vector<byte>& bytes) { addX(bytes, 1); }
void twosCompliment(std::vector<byte>& bytes) {
	invert(bytes);
	addOne(bytes);
}
std::vector<byte> ones(int size) {
	std::vector<byte> bytes;
	for (int i = 0; i < size; i++)
		bytes.push_back(0xff);
	return bytes;
}

void resizeBy(std::vector<byte>& bytes, int n) {
	for (int i = 0; i < n; i++)
		bytes.push_back(0);
}
void resizeTo(std::vector<byte>& bytes, int n) {
	while ((int)bytes.size() < n)
		bytes.push_back(0);
}
void removeLeadingZeros(std::vector<byte>& bytes) {
	while (*(bytes.end() - 1) == 0 && bytes.size() > 1)
		bytes.erase(bytes.end() - 1);
}

std::vector<byte> numToBytes(uint64_t x) {
	std::vector<byte> bytes;
	for (int i = 0; i < (int)sizeof(uint64_t); i++) {
		bytes.push_back(GET_BYTE(x, i));
	}
	return bytes;
}

void addVectors(std::vector<byte>& a, const std::vector<byte>& b) {
	byte carry = 0;
	uint16_t sum;
	for (int i = 0; i < (int)a.size(); i++) {
		sum = a[i] + b[i] + carry;
		carry = GET_BYTE(sum, 1);
		a[i] = GET_BYTE(sum, 0);
	}
}

int compareVectors(const std::vector<byte>& l, const std::vector<byte>& r) {
	if (l.size() > r.size())
		return 1;
	if (l.size() < r.size())
		return -1;
	for (int i = l.size() - 1; i >= 0; i--) {
		if (l[i] > r[i])
			return 1;
		if (l[i] < r[i])
			return -1;
	}
	return 0;
}

bool isInt(const std::string& s) {
	if (s.empty()) return false;
	if (!s[0] == '-' && !isdigit(s[0])) return false;
	if (s[0] == '-' && s.size() == 1) return false;
	for (int i = 1; i < (int)s.size(); i++) {
		if (!isdigit(s[i])) {
			return false;
		}
	}
	return true;
}