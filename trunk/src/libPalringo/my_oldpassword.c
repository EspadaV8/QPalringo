#include <stdio.h>
#include <string.h>
#include "my_oldpassword.h"


void hash_password(uint32_t *result, const char *password, uint32_t password_len)
{
  register uint32_t nr=1345345333L, add=7, nr2=0x12345671L;
  uint32_t tmp;
  const char *password_end= password + password_len;
  for (; password < password_end; password++)
  {
    if (*password == ' ' || *password == '\t')
      continue;                                 /* skip space in password */
    tmp= (uint32_t) (unsigned char) *password;
    nr^= (((nr & 63)+add)*tmp)+ (nr << 8); 
    nr2+=(nr2 << 8) ^ nr; 
    add+=tmp;
  }
  result[0]=nr & (((uint32_t) 1L << 31) -1L); /* Don't use sign bit (str2int) */;
  result[1]=nr2 & (((uint32_t) 1L << 31) -1L);
}


/*
 *     Create password to be stored in user database from raw string
 *     Used for pre-4.1 password handling
 *     SYNOPSIS
 *     make_scrambled_password_323()
 *     to        OUT store scrambled password here
 *     password  IN  user-supplied password
 **/

void make_scrambled_password_323(char *to, const char *password)
{
  uint32_t hash_res[2];
  hash_password(hash_res, password, (uint32_t) strlen(password));
  sprintf(to, "%08x%08x", hash_res[0], hash_res[1]);
}

