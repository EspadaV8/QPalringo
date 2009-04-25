/*
 * libPalringo/DataMap.cc
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <iostream>

#include "DataMap.h"

const char *DataMap::OUT_OF_BOUNDS("Value length out of bounds");

DataMap::DataMap(const std::string &data)
{
  parse(data.data(), data.size());
}

DataMap::DataMap(const char* data, size_t length)
{
  parse(data, length);
}

DataMap::DataMap()
{
}

DataMap::~DataMap()
{
  dataMap_.clear();
}

void
DataMap::parse(const char *data, size_t length)
{
  if (!length)
  {
    return; 
  }

  const char *p(data);
  const char *base(data);
  const char *end(data + length);

  while(p < end)
  {
    // p will point to the end of the attribute string
    if(!(p = index(base, '\0')))
    {
      return;
    }

    std::string attr(base, p - base);

    // p points to the length short
    p++;

    // extract the length of the data
    unsigned short valueLength(static_cast<unsigned int>(
	ntohs(*(reinterpret_cast<const uint16_t*>(p)))));

    // point to the first byte of the data
    p += 2;

    // out of bounds, parse error!
    if ((p + valueLength) > end)
    {
      throw DataMapException(OUT_OF_BOUNDS);
    }

    std::string value(p, valueLength);

    std::pair<ValueMap::iterator, bool> res(
	dataMap_.insert(std::pair<std::string, std::string>(attr, value)));

    // if there is already an attribute in the map append the data to it
    if(!res.second)
    {
      res.first->second.append(value);
    }

    p += valueLength;
    base = p;
  }
}

void
DataMap::appendData(std::string &data,
                    const std::string &attr, 
                    const std::string &value)
{
  appendData(data, attr.c_str(), value.data(), value.size());
}

void
DataMap::appendData(std::string &data,
                    const std::string &attr, 
                    const char *value, 
		    size_t length)
{
  appendData(data, attr.c_str(), value, length);
}

void
DataMap::appendData(std::string &data,
                    const char *attr, 
                    const std::string &value) 
{
  appendData(data, attr, value.data(), value.size());
}

void
DataMap::appendData(std::string &data,
                    const char *attr, 
                    const char *value, 
		    size_t length)
{
  const char *p = value;
  const char *end = value + length;

  while(p < end)
  {
    data.append(attr);

    uint16_t valueLength = (end - p <= 0xffff ) ? end - p : 0xffff;

    uint16_t len(htons(valueLength));

    data.append(reinterpret_cast<const char*>(&len), 2);

    data.append(p, valueLength);

    p += valueLength;
  }
}

std::string
DataMap::getData()
{
  std::string res;

  for (ValueMap::iterator it = dataMap_.begin();
       it != dataMap_.end();
       ++it)
  {
    appendData(res, it->first, it->second);
  }
  return res;
}


bool
DataMap::hasAttribute(const std::string &att)
{
  ValueMap::iterator it(dataMap_.find(att));

  if (it != dataMap_.end())
  {
    return true;
  }

  else
  {
    return false;
  }
}

bool
DataMap::hasAttribute(const char *att)
{
  return hasAttribute(std::string(att));
}

std::string
DataMap::operator[](const std::string &att)
{
  return at(att);
}

std::string
DataMap::operator[](const char *att)
{
  return at(att);
}

std::string
DataMap::at(const std::string &att)
{
  ValueMap::iterator it(dataMap_.find(att));

  if (it != dataMap_.end())
  {
    return std::string(it->second);
  }
  else
  {
    return std::string();
  }
}

std::string
DataMap::at(const char *att)
{
  return at(std::string(att));
}


std::string
DataMap::toString()
{
  std::string res("{\n");

  for(ValueMap::iterator it = dataMap_.begin(); it != dataMap_.end(); ++it)
  {
    res.append("\t");
    res.append(it->first);
    res.append(": ");
    res.append(it->second);
    res.append("\n");
  }

  res.append("}");

  return res;
}

