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
#include <exception>


class DataMap
{
public:
  typedef std::map<std::string, std::string> ValueMap;

private:
  ValueMap dataMap_;
  void parse(const char *data, size_t length);

public:
  static const char *EMPTY;
  static const char *OUT_OF_BOUNDS;

  DataMap(const std::string &data);
  DataMap(const char *data, size_t length);
  DataMap();

  ~DataMap();


  static void appendData(std::string       &data,
                         const std::string &attr,
                         const std::string &value);

  static void appendData(std::string       &data,
                         const std::string &attr,
                         const char        *value, 
                         size_t             length);

  static void appendData(std::string       &data,
                         const char        *attr, 
                         const std::string &value);
  
  static void appendData(std::string       &data,
                         const char        *attr, 
                         const char        *value, 
                         size_t             length);

  std::string getData();

  ValueMap::iterator begin();
  ValueMap::iterator end();
  ValueMap::iterator find(const std::string &attr);
  size_t size();
  bool empty();

  bool erase(const std::string &attr);

  bool hasAttribute(const std::string &att);
  bool hasAttribute(const char *att);

  std::string operator[](const std::string &att);
  std::string operator[](const char *att);

  std::string at(const std::string &att);
  std::string at(const char *att);

  std::string toString();
};

inline
DataMap::ValueMap::iterator
DataMap::begin()
{
  return dataMap_.begin();
}

inline
DataMap::ValueMap::iterator
DataMap::end()
{
  return dataMap_.end();
}

inline
DataMap::ValueMap::iterator
DataMap::find(const std::string &attr)
{
  return dataMap_.find(attr);
}

inline
size_t
DataMap::size()
{
  return dataMap_.size();
}

inline
bool
DataMap::empty()
{
  return dataMap_.empty();
}


class DataMapException : public std::exception
{
public:
  DataMapException(const char *what) : what_(what)
  {
  }

  virtual const char* what() const throw()
  {
    return what_;
  }

private:
  const char *what_;
};


#endif
