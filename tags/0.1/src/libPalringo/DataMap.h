/*
 * libPalringo/DataMap.h
 * Copyright (c) 2008, Palringo Ltd
 *
 * Original author: Eric Sellin
 * Extended version author: Xabier Eizmendi
 *
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
#ifndef DATAMAP_H
#define DATAMAP_H

#include <string>
#include <map>
#include <vector>

class DataMap
{
public:
  typedef struct
  {
    unsigned int pos_;
    unsigned int length_;
  } Value;

  typedef std::map<std::string, Value> ValueMap;

private:
  std::vector<char> data_;
  ValueMap dataMap_;

public:
  DataMap(const std::string &data);
  DataMap(const char *data, unsigned long length);
  DataMap();

  ~DataMap();

  bool parse();

  bool appendData(const std::string &attr,
                  const std::string &value);

  bool appendData(const std::string &attr,
                  const char *value, 
		  unsigned int length);

  bool appendData(const char *attr, 
                  const std::string &value);
  
  bool appendData(const char *attr, 
                  const char *value, 
		  unsigned int length);

  unsigned long getData(char *data);

  std::string getData();

  std::vector<std::string> getAttributes();

  ValueMap::iterator begin();
  ValueMap::iterator end();

  bool hasAttribute(const std::string &att);
  bool hasAttribute(const char *att);

  std::string operator[](const std::string &att);
  std::string operator[](const char *att);
  std::string operator[](ValueMap::iterator &it);

  std::string at(const std::string &att);
  std::string at(const char *att);
  std::string at(ValueMap::iterator &it);

  std::string toString();
};


#endif
