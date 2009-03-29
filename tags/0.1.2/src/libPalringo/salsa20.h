/* salsa20.h */

/* 
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 * 
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef SALSA20_SYNC
#define SALSA20_SYNC

#include "ecrypt-portable.h"
#include "inttypes.h"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; SALSA20_KEYSIZE(i) <= SALSA20_MAXKEYSIZE; ++i)
 *   {
 *     keysize = SALSA20_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define SALSA20_MAXKEYSIZE 256                 /* [edit] */
#define SALSA20_KEYSIZE(i) (128 + (i)*128)     /* [edit] */

#define SALSA20_MAXIVSIZE 64                   /* [edit] */
#define SALSA20_IVSIZE(i) (64 + (i)*64)        /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/* 
 * salsa20_ctx is the structure containing the representation of the
 * internal state of your cipher. 
 */

typedef struct
{
  uint32_t input[16]; /* could be compressed */
  /* 
   * [edit]
   *
   * Put here all state variable needed during the encryption process.
   */
} salsa20_ctx;

/* ------------------------------------------------------------------------- */

/* Mandatory functions */

/*
 * Key and message independent initialization. This function will be
 * called once when the program starts (e.g., to build expanded S-box
 * tables).
 */
void salsa20_init();

/*
 * Key setup. It is the user's responsibility to select the values of
 * keysize and ivsize from the set of supported values specified
 * above.
 */
void salsa20_keysetup(
  salsa20_ctx* ctx, 
  const uint8_t* key, 
  uint32_t keysize,                /* Key size in bits. */ 
  uint32_t ivsize);                /* IV size in bits. */ 

/*
 * IV setup. After having called salsa20_keysetup(), the user is
 * allowed to call salsa20_ivsetup() different times in order to
 * encrypt/decrypt different messages with the same key but different
 * IV's.
 */
void salsa20_ivsetup(
  salsa20_ctx* ctx, 
  const uint8_t* iv);

/*
 * Encryption/decryption of arbitrary length messages.
 *
 * For efficiency reasons, the API provides two types of
 * encrypt/decrypt functions. The salsa20_encrypt_bytes() function
 * (declared here) encrypts byte strings of arbitrary length, while
 * the salsa20_encrypt_blocks() function (defined later) only accepts
 * lengths which are multiples of SALSA20_BLOCKLENGTH.
 * 
 * The user is allowed to make multiple calls to
 * salsa20_encrypt_blocks() to incrementally encrypt a long message,
 * but he is NOT allowed to make additional encryption calls once he
 * has called salsa20_encrypt_bytes() (unless he starts a new message
 * of course). For example, this sequence of calls is acceptable:
 *
 * salsa20_keysetup();
 *
 * salsa20_ivsetup();
 * salsa20_encrypt_blocks();
 * salsa20_encrypt_blocks();
 * salsa20_encrypt_bytes();
 *
 * salsa20_ivsetup();
 * salsa20_encrypt_blocks();
 * salsa20_encrypt_blocks();
 *
 * salsa20_ivsetup();
 * salsa20_encrypt_bytes();
 * 
 * The following sequence is not:
 *
 * salsa20_keysetup();
 * salsa20_ivsetup();
 * salsa20_encrypt_blocks();
 * salsa20_encrypt_bytes();
 * salsa20_encrypt_blocks();
 */

void salsa20_encrypt_bytes(
  salsa20_ctx* ctx, 
  const uint8_t* plaintext, 
  uint8_t* ciphertext, 
  uint32_t msglen);                /* Message length in bytes. */ 

void salsa20_decrypt_bytes(
  salsa20_ctx* ctx, 
  const uint8_t* ciphertext, 
  uint8_t* plaintext, 
  uint32_t msglen);                /* Message length in bytes. */ 

/* ------------------------------------------------------------------------- */

/* Optional features */

/* 
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the SALSA20_GENERATES_KEYSTREAM flag.
 */

#define SALSA20_GENERATES_KEYSTREAM
#ifdef SALSA20_GENERATES_KEYSTREAM

void salsa20_keystream_bytes(
  salsa20_ctx* ctx,
  uint8_t* keystream,
  uint32_t length);                /* Length of keystream in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional optimizations */

/* 
 * By default, the functions in this section are implemented using
 * calls to functions declared above. However, you might want to
 * implement them differently for performance reasons.
 */

/*
 * All-in-one encryption/decryption of (short) packets.
 *
 * The default definitions of these functions can be found in
 * "ecrypt-sync.c". If you want to implement them differently, please
 * undef the SALSA20_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define SALSA20_USES_DEFAULT_ALL_IN_ONE        /* [edit] */

void salsa20_encrypt_packet(
  salsa20_ctx* ctx, 
  const uint8_t* iv,
  const uint8_t* plaintext, 
  uint8_t* ciphertext, 
  uint32_t msglen);

void salsa20_decrypt_packet(
  salsa20_ctx* ctx, 
  const uint8_t* iv,
  const uint8_t* ciphertext, 
  uint8_t* plaintext, 
  uint32_t msglen);

/*
 * Encryption/decryption of blocks.
 * 
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * SALSA20_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define SALSA20_BLOCKLENGTH 64                  /* [edit] */

#define SALSA20_USES_DEFAULT_BLOCK_MACROS      /* [edit] */
#ifdef SALSA20_USES_DEFAULT_BLOCK_MACROS

#define salsa20_encrypt_blocks(ctx, plaintext, ciphertext, blocks)  \
  salsa20_encrypt_bytes(ctx, plaintext, ciphertext,                 \
    (blocks) * SALSA20_BLOCKLENGTH)

#define salsa20_decrypt_blocks(ctx, ciphertext, plaintext, blocks)  \
  salsa20_decrypt_bytes(ctx, ciphertext, plaintext,                 \
    (blocks) * SALSA20_BLOCKLENGTH)

#ifdef SALSA20_GENERATES_KEYSTREAM

#define salsa20_keystream_blocks(ctx, keystream, blocks)            \
  salsa20_keystream_bytes(ctx, keystream,                        \
    (blocks) * SALSA20_BLOCKLENGTH)

#endif

#else

void salsa20_encrypt_blocks(
  salsa20_ctx* ctx, 
  const uint8_t* plaintext, 
  uint8_t* ciphertext, 
  uint32_t blocks);                /* Message length in blocks. */ 

void salsa20_decrypt_blocks(
  salsa20_ctx* ctx, 
  const uint8_t* ciphertext, 
  uint8_t* plaintext, 
  uint32_t blocks);                /* Message length in blocks. */ 

#ifdef SALSA20_GENERATES_KEYSTREAM

void salsa20_keystream_blocks(
  salsa20_ctx* ctx,
  const uint8_t* keystream,
  uint32_t blocks);                /* Keystream length in blocks. */ 

#endif

#endif

/*
 * If your cipher can be implemented in different ways, you can use
 * the SALSA20_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DSALSA20_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (salsa20_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same SALSA20_BLOCKLENGTH, etc.). 
 */
#define SALSA20_MAXVARIANT 1                   /* [edit] */

#ifndef SALSA20_VARIANT
#define SALSA20_VARIANT 1
#endif

#if (SALSA20_VARIANT > SALSA20_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
