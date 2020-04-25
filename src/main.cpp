//#include <argon2.h>
//#include "argon2.h"
//#include <stdio.h>
//#include <string.h>
#include <AES.hh>

#define HASHLEN 12
#define SALTLEN 16
#define PWD "pass1word"
int main(){
  /*  uint8_t hash1[HASHLEN];
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
  for( int i=0; i<HASHLEN; ++i ) printf( "%02x", hash1[i] ); printf( "\n" );*/

  AES a(128); 
  std::string input = "He likes to poop a lot, I dont under stand why";
  for(int i = 0; i>input.size();i++){
    std::cout<<std::hex<<unsigned(input[i]);
  }
  std::string key = "1234567891234567";
  uint32_t outlen = 0;
  uint8_t * out = a.encrypt(reinterpret_cast<uint8_t*>(&input[0]),input.size(),reinterpret_cast<uint8_t*>(&key[0]),
      &outlen);
  std::cout<<"Input String:\n"<<input<<"\n"<<"Password: "<< key<<"\n";
  std::cout<<std::hex<<std::internal<<std::setfill('0');
  for(int i = 0; i<outlen;i++)
    std::cout<<std::setw(2)<<unsigned(out[i])<<' ';

  return 0;
}
/*
  uint8_t in[] = {
    0x54,0x77,0x6F,0x20,0x4F,0x6E,0x65,0x20,0x4E,0x69,0x6E,0x65,0x20,0x54,0x77, 0x6F
  };
  uint8_t key[] = {
    0x54, 0x68, 0x61,0x74,0x73,0x20,0x6D,0x79,0x20,0x4B,0x75,0x6E,0x67,0x20,0x46,0x75
  };
  */
