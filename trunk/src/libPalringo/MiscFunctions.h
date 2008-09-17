#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H
#include <string>
#include <inttypes.h>

#define DUMP_ROW_SIZE 16

std::string hexDump(std::string &data);

std::string hexDump(const char *data, size_t size);

std::string zlibCompress(const std::string &data, int level = -1);
std::string zlibDecompress(const std::string &data);

std::string toString(char num);
std::string toString(unsigned char num);

std::string toString(short num);
std::string toString(unsigned short num);


std::string toString(int num);
std::string toString(unsigned int num);

std::string toString(long num);
std::string toString(unsigned long num);

std::string toString(long long num);
std::string toString(unsigned long long num);

std::string toString(double);
#endif
