/*
* libPalringo/MiscFunctions.cc
* Copyright (c) 2008, Palringo Ltd
*
* Original author: Eric Sellin
* Extended version author: Xabier Eizmendi
*
*
* This program is free software (with dual license);
* You can redistribute it and/or modify it under the terms of
* the GNU General Public License version 2 as published by the
* Free Software Foundation and in terms of Palringo Closed Source License
* If you want to use it in a closed source program you must contact
* Palringo Ltd.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Library General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
*/
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
