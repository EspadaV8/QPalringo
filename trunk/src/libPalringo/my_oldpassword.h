#include <inttypes.h>

void hash_password(uint32_t *result, const char *password, uint32_t password_len);
void make_scrambled_password_323(char *to, const char *password);
