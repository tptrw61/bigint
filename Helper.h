#pragma once
#include <vector>
#include "BigInt.h"

using byte = BigInt::byte;
#define GET_BYTE(n, b) ((byte)((n) >> ((b) * 8)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_ZERO(v) (v.size() == 1 && v[0] == 0)
#define IS_ONE(v) (v.size() == 1 && v[0] == 1)

void invert(std::vector<byte>& bytes);
void addX(std::vector<byte>& bytes, byte x);
void addOne(std::vector<byte>& bytes);
void twosCompliment(std::vector<byte>& bytes);

std::vector<byte> ones(int size);

void resizeBy(std::vector<byte>& bytes, int n);
void resizeTo(std::vector<byte>& bytes, int n);
void removeLeadingZeros(std::vector<byte>& bytes);

std::vector<byte> numToBytes(uint64_t x);
void addVectors(std::vector<byte>& a, const std::vector<byte>& b);

int compareVectors(const std::vector<byte>& l, const std::vector<byte>& r);
