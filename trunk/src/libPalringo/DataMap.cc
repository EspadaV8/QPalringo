#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <iostream>

#include "DataMap.h"

DataMap::DataMap(const std::string &data)
{
  const char* dataptr =  data.data();
  data_.reserve(data.size());
  data_.insert(data_.end(), dataptr, dataptr + data.size());
}

DataMap::DataMap(const char* data, unsigned long length)
{
  data_.reserve(length);
  data_.insert(data_.end(), data, data + length);
}

DataMap::DataMap()
{
}

DataMap::~DataMap()
{
  data_.clear();
  dataMap_.clear();
}

bool
DataMap::parse()
{
  const char *dataptr =  &data_[0];
  if (!data_.size())
  {
    return false;
  }

  const char *p(dataptr);
  const char *base(p);
  const char *end(dataptr + data_.size());

  while(p < end)
  {
    // p will point to the end of the attribute string
    if(!(p = index(base, '\0')))
    {
      break;
    }

    std::string key(base, p - base);

    Value value;

    // p points to the length short
    p++;

    // extract the length of the data
    value.length_ = static_cast<unsigned int>(
	ntohs(*(reinterpret_cast<const uint16_t*>(p))));

    // point to the first byte of the data
    p += 2;

    // out of bounds, parse error!
    if ((p + value.length_) > end)
    {
      std::cout << "p out of bounds, length: " 
	        << value.length_ 
	        << " but end:" << end - p << std::endl;
      return false;
    }

    value.pos_ = p - dataptr;

    std::pair<ValueMap::iterator, bool> res(
	dataMap_.insert(std::pair<std::string, Value>(key, value)));

    // the attribute isn't unique, parse error!
    if(!res.second)
    {
      std::cout << "Attribute isn't unique" << std::endl;
      return false;
    }

    p += value.length_;
    base = p;
  }

  return true;
}

bool
DataMap::appendData(const std::string &attr, const std::string &value)
{

  if (dataMap_.find(attr) != dataMap_.end())
  {
    return false;
  }

  else
  {
    unsigned long newLength(data_.size() + attr.size() + 3 + value.size());

    data_.reserve(newLength);

    const char *attrptr = attr.c_str();
    const char *valueptr = value.data();

    data_.insert(data_.end(), attrptr, attrptr + attr.size() + 1);

    uint16_t len(htons(static_cast<uint16_t>(value.size())));
    const char *lenptr =  reinterpret_cast<const char*>(&len);

    data_.insert(data_.end(), lenptr, lenptr + 2);

    data_.insert(data_.end(), valueptr, valueptr + value.size());

    Value dataValue;

    dataValue.pos_ = data_.size() - value.size();
    dataValue.length_ = value.size();

    dataMap_.insert(std::pair<std::string, Value>(
	  attr, dataValue));

    return true;
  }
}

bool
DataMap::appendData(const std::string &attr, 
                    const char *value, 
		    unsigned int length)
{
  return appendData(attr, std::string(value, length));
}

bool
DataMap::appendData(const char *attr, 
                    const std::string &value) 
{
  return appendData(std::string(attr), value);
}

bool
DataMap::appendData(const char *attr, 
                    const char *value, 
		    unsigned int length)
{
  return appendData(std::string(attr), std::string(value, length));
}

unsigned long
DataMap::getData(char *data)
{
  if(data_.size())
  {
    memcpy((void*)data, (const void*)&data_[0], data_.size());

    return data_.size();
  }

  else
  {
    return 0;
  }
}

std::string
DataMap::getData()
{
  if(data_.size())
  {
    return std::string(&data_[0], data_.size());
  }

  else
  {
    return std::string();
  }
}

std::vector<std::string>
DataMap::getAttributes()
{
  std::vector<std::string> res;

  for (ValueMap::iterator it = dataMap_.begin(); it != dataMap_.end(); it++)
  {
    res.push_back(it->first);
  }

  return res;
}

DataMap::ValueMap::iterator
DataMap::begin()
{
  return dataMap_.begin();
}

DataMap::ValueMap::iterator
DataMap::end()
{
  return dataMap_.end();
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
  ValueMap::iterator it(dataMap_.find(att));

  if (it != dataMap_.end())
  {
    return std::string(&data_[0] + it->second.pos_, it->second.length_);
  }
  else
  {
    return std::string();
  }
}

std::string
DataMap::operator[](const char *att)
{
  return operator[](std::string(att));
}

std::string
DataMap::operator[](ValueMap::iterator &it)
{
  return std::string(&data_[0] + it->second.pos_, it->second.length_);
}

std::string
DataMap::at(const std::string &att)
{
  ValueMap::iterator it(dataMap_.find(att));

  if (it != dataMap_.end())
  {
    return std::string(&data_[0] + it->second.pos_, it->second.length_);
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
DataMap::at(ValueMap::iterator &it)
{
  return std::string(&data_[0] + it->second.pos_, it->second.length_);
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
    res.append(&data_[0] + it->second.pos_, it->second.length_);
    res.append("\n");
  }

  res.append("}");

  return res;
}
