#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <openssl/dh.h>

extern "C" {
#include "salsa20.h"
}

namespace crypto
{
std::string md5(const std::string &input);

std::string oldPassword(const std::string &source);

class SalsaCipher
{
private:
  std::string initVector_;
  std::string key_;
  salsa20_ctx context_;

  SalsaCipher();

  void keySetup();

  void ivSetup();

public:

  SalsaCipher(const std::string &initVector, const std::string &key);

  void encrypt(const std::string &input, std::string &output);
  
  void setIV(std::string &initVector);
  void setKey(std::string &key);

  const std::string& getIV();

  const std::string& getKey();

  static const unsigned int BLOCK_SIZE;
  static const unsigned int IV_SIZE;
};

class DiffieHellman
{
private:
  DH *dh_;
  static const int PRIME_LEN;
  static const int GENERATOR;
  static const char *GENERATOR_STR;
public:
  DiffieHellman();
  DiffieHellman(const std::string &primeNumber);
  ~DiffieHellman();

  void setPrime(const std::string &primeNumber);

  void generateParameters();

  std::string getPrime();

  std::string getPublic();

  std::string getSecret(const std::string &pub_key);
};

}

inline void
crypto::SalsaCipher::keySetup()
{
  salsa20_keysetup(&context_,
                   reinterpret_cast<const uint8_t*>(key_.c_str()), 
                   key_.size() * 8,
                   IV_SIZE);
}

inline void
crypto::SalsaCipher::ivSetup()
{
  salsa20_ivsetup(&context_, 
                  reinterpret_cast<const uint8_t*>(initVector_.c_str()));
}

inline const std::string&
crypto::SalsaCipher::getIV()
{
  return initVector_;
}

inline const std::string&
crypto::SalsaCipher::getKey()
{
  return key_;
}
#endif
