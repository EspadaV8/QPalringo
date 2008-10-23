/*
* libPalringo/my_oldpassword.h
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
#include <inttypes.h>

void hash_password(uint32_t *result, const char *password, uint32_t password_len);
void make_scrambled_password_323(char *to, const char *password);
