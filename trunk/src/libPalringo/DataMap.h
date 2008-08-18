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
