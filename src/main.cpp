#include <argon2.h>
#include "argon2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHLEN 12
#define SALTLEN 16
#define PWD "pass1word"
int main(){
  uint8_t hash1[HASHLEN];
  uint8_t hash2[HASHLEN];

  uint8_t salt[SALTLEN];
  memset( salt, 0x00, SALTLEN );

  uint8_t *pwd = (uint8_t *)strdup(PWD);
  uint32_t pwdlen = strlen((char *)pwd);

  uint32_t t_cost = 2;            // 1-pass computation
  uint32_t m_cost = (1<<16);      // 64 mebibytes memory usage
  uint32_t parallelism = 1;       // number of threads and lanes

  // high-level API
  argon2i_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, salt, SALTLEN, hash1, HASHLEN);
  for( int i=0; i<HASHLEN; ++i ) printf( "%02x", hash1[i] ); printf( "\n" );
  return 0;
}
