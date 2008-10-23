/*
 * libPalringo/crypto.h
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
