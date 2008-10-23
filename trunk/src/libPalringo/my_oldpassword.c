/*
* libPalringo/my_oldpassword.c
* Copyright (c) 2008, Palringo Ltd
*
* Original author: Eric Sellin
* Extended version author: Xabier Eizmendi
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

