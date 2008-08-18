#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "MiscFunctions.h"

std::string 
hexDump(std::string &data)
{
  return hexDump(data.data(), data.size());
}

std::string 
hexDump(const char *data, size_t size)
{
  /* The result shold look something like below.
0000: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52   .PNG........IHDR
0010: 00 00 04 02 00 00 02 E7 08 02 00 00 00 9C 1B 72   ...............r
0020: E9 00 00 00 09 70 48 59 73 00 00 0E C4 00 00 0E   .....pHYs.......
0030: C4 01 95 2B 0E 1B 00 00 00 07 74 49 4D 45 07 D6   ...+......tIME..
0040: 0B 06 0B 0A 09 D5 6E 84 90 00 00 20 00 49 44 41   ......n.... .IDA
0050: 54 78 DA EC DD 79 BC 24 69 55 27 FC DF 39 E7 79   Tx...y.$iU'..9.y
0060: 22 22 F3 DE 5B 5B 57 75 57 D3 48 B3 4A 23 34 B2   ""..[[WuW.H.J#4.
0070: 09 34 A0 5D 0A B4 E8 0C EF B8 8C 23 E8 C0 BC 23   .4.].......#...#
0080: AF 8E 23 B2 4D 3B 6C 32 74 81 8A E8 08 0A 2A 8E   ..#.M;l2t.....*.
0090: C2 E0 E8 0B EA A8 83 A3 30 0E 2F AF 42 15 DA 80   ........0./.B...
00A0: 28 28 82 20 9B CD 62 EF DD B5 DD BA 99 19 F1 3C   ((. ..b........<
00B0: E7 9C F9 23 6E DD BA B5 74 F5 0E DD F6 F9 7E B2   ...#n...t.....~.
00C0: 6E 65 46 46 E4 12 91 91 79 CE B3 D2 85 17 5E 88   neFF....y.....^.
00D0: 10 42 08 21 84 10 C2 3D 49 02 F0 BB BF FD 56 00   .B.!...=I.....V.
00E0: 0E 8F DD 71 A7 22 D0 DD EE 35 5F 39 C8 2D 5C F3   ...q."...5_9.-\.
00F0: 5D 7F 7D 4D 1C E2 70 6B C4 17 4E 08 21 84 F0 55   ].}M..pk..N.!..U
  */
  const char *charData(reinterpret_cast<const char*>(data));

  std::ostringstream ost;

  for (size_t i = 0; i < size; ++i)
  {
    if ((i % DUMP_ROW_SIZE) == 0)
    {
      ost << std::setw(4) << std::setfill('0') << i;
    }

	ost << " " << std::hex << std::setw(2) << std::setfill('0')
        << (0x000000FF & static_cast<int>(charData[i]));

    // Right is it time to print a new line?
    if ((i == (size - 1)) // last one
        ||
        (((i + 1) % DUMP_ROW_SIZE) == 0))
    {
      
      ost << std::setw((std::streamsize)
	    (3 * (DUMP_ROW_SIZE - (i % DUMP_ROW_SIZE)))) 
          << std::setfill(' ') << "";

     size_t j(0);

      if (i == (size - 1))
      {
        j = DUMP_ROW_SIZE * (i / DUMP_ROW_SIZE);
      }
      else
      {
        j = i + 1 - DUMP_ROW_SIZE;
      }

      // But don't forget to print the ascii data.
      for (; j <= i; ++j)
      {
        if (isprint(charData[j]))
        {
          ost << charData[j];
        }
        else
        {
          ost << ".";
        }
      }

      ost << "\n";
    }
  }

  return ost.str();
}


std::string toString(char num)
{
  char tmp[8];
  sprintf(tmp, "%hhd", num);
  return std::string(tmp);
}

std::string toString(unsigned char num)
{
  char tmp[8];
  sprintf(tmp, "%hhu", num);
  return std::string(tmp);
}

std::string toString(short num)
{
  char tmp[8];
  sprintf(tmp, "%hd", num);
  return std::string(tmp);
}

std::string toString(unsigned short num)
{
  char tmp[8];
  sprintf(tmp, "%hu", num);
  return std::string(tmp);
}

std::string toString(int num)
{
  char tmp[16];
  sprintf(tmp, "%d", num);
  return std::string(tmp);
}

std::string toString(unsigned int num)
{
  char tmp[16];
  sprintf(tmp, "%u", num);
  return std::string(tmp);
}

std::string toString(long num)
{
  char tmp[32];
  sprintf(tmp, "%ld", num);
  return std::string(tmp);
}

std::string toString(unsigned long num)
{
  char tmp[32];
  sprintf(tmp, "%lu", num);
  return std::string(tmp);
}

std::string toString(long long num)
{
  char tmp[64];
  sprintf(tmp, "%lld", num);
  return std::string(tmp);
}

std::string toString(unsigned long long num)
{
  char tmp[64];
  sprintf(tmp, "%llu", num);
  return std::string(tmp);
}
