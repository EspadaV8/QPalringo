#include <iostream>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include "crypto.h"

extern "C" {
#include "md5.h"
#include "my_oldpassword.h"
}

namespace crypto
{

std::string
md5(const std::string &input)
{
  char buff[16] = "";
  md5_buffer(input.data(), input.size(), (void *)buff);
  return std::string(buff, sizeof(buff));
}

std::string
oldPassword(const std::string &source)
{
  char result[1024];

  make_scrambled_password_323(result, source.c_str());

  return std::string(result);
}

const unsigned int SalsaCipher::BLOCK_SIZE(512);
const unsigned int SalsaCipher::IV_SIZE(64);

const int DiffieHellman::PRIME_LEN(1024);
const int DiffieHellman::GENERATOR(2);
const char *DiffieHellman::GENERATOR_STR("2");

SalsaCipher::SalsaCipher(const std::string &initVector, const std::string &key)
 :initVector_(initVector),
  key_(key)
{
  keySetup();
  ivSetup();
}

void
SalsaCipher::encrypt(const std::string &input, std::string &output)
{
  const uint8_t *inbuf(reinterpret_cast<const uint8_t*>(input.c_str()));
  unsigned int msgLen(input.size());
  unsigned int blockLen(0);
  uint8_t outbuf[BLOCK_SIZE];

  output.clear();

  for (unsigned int i = 0; i < msgLen; i += BLOCK_SIZE)
  {
    blockLen = (msgLen - i) < BLOCK_SIZE ? msgLen - i : BLOCK_SIZE;

    salsa20_encrypt_bytes(&context_, inbuf + i, outbuf, blockLen);

    output.append(reinterpret_cast<char*>(outbuf), blockLen);
  }
}

void
SalsaCipher::setIV(std::string &initVector)
{
  initVector_ = initVector;
  ivSetup();
}

void
SalsaCipher::setKey(std::string &key)
{
  key_ = key;
  keySetup();
}

DiffieHellman::DiffieHellman():
  dh_(NULL)
{
  generateParameters();
}

DiffieHellman::DiffieHellman(const std::string &primeNumber):
  dh_(NULL)
{
  BIGNUM *p = BN_new();
  BIGNUM *g = BN_new();
  BN_dec2bn(&p, primeNumber.c_str());
  BN_dec2bn(&g, GENERATOR_STR);

  do
  {
    dh_ = DH_new();
  } while (!dh_);

  dh_->p = p;
  dh_->g = g;
}

DiffieHellman::~DiffieHellman()
{
  if(dh_)
  {
    DH_free(dh_);
  }
}

void
DiffieHellman::setPrime(const std::string &primeNumber)
{
  if (dh_->p)
  {
    BN_free(dh_->p);
  }

  BIGNUM *p = BN_new();
  BN_dec2bn(&p, primeNumber.c_str());
  dh_->p = p;
}

void
DiffieHellman::generateParameters()
{
  if(dh_)
  {
    DH_free(dh_);
  }

  dh_ = NULL;
  do
  {
    dh_ = DH_generate_parameters(PRIME_LEN, GENERATOR, NULL, NULL);
  } while(!dh_);
}

std::string
DiffieHellman::getPrime()
{
  return std::string(BN_bn2dec(dh_->p));
}

std::string
DiffieHellman::getPublic()
{
  DH_generate_key(dh_);
  return std::string(BN_bn2dec(dh_->pub_key));
}

std::string
DiffieHellman::getSecret(const std::string &pub_key)
{
  BIGNUM *bnpub_key = BN_new();
  BN_dec2bn(&bnpub_key, pub_key.c_str());
  unsigned int keylen = DH_size(dh_);

  unsigned char *key = new unsigned char[keylen];

  int result(0);
  if ((result = DH_compute_key(key, bnpub_key, dh_)) < 0)
  {
    std::cout << "Error: " << ERR_error_string(ERR_get_error(), NULL) << std::endl;
    return std::string();
  }
  unsigned int keysize = static_cast<unsigned int>(result);

  char *hexkey = new char[keysize * 3];

  for(unsigned int i = 0; i < keysize; i++)
  {
    sprintf(hexkey + i * 2, "%02x", key[i]);
  }

  BIGNUM *bnkey = BN_new();
  BN_hex2bn(&bnkey, hexkey);
  std::string res(BN_bn2dec(bnkey));

  BN_free(bnpub_key);
  BN_free(bnkey);
  delete[] key;
  delete[] hexkey;
  return res;
}

}
