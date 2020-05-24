#include <AES.hh>
#include <FileEditor.hh>
#include <BitCrypt.hh>

#include "argon2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HASHLEN 32
#define SALTLEN 16
#define PWD "password"
void AESTests(), FileTest(), bcryptTest(), argonTest();

int main(){
  std::cout<<"HI";
  bcryptTest();
  return 0;
}

void bcryptTest(){
  BitCrypt b(AES_128);
  const char file[] = "C:\\Users\\itayb\\OneDrive\\Desktop\\Hi.txt";
  b.encryptFile(file,"HIE",3);
  if(b.checkFile(file,"HIE",3) == 1){
   b.decryptFile(file,"HIE",3);
  }
}

void FileTest(){
  uint8_t buf[131940];
  const char file[] = "/home/itay/Desktop/hi.txt";
  char text[] = "GET WIPED SON!";

  FileEditor fedit;
  if(fedit.loadFile(file,false)){
    int32_t readNum = fedit.readBytes(buf,fedit.fileSize()); 
    for(int32_t i = 0; i<readNum; i++){
      std::cout<<char(buf[i]);
    } 
    fedit.writeBytes((uint8_t*)text,14);
  } 
}

void argonTest(){

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
}

void AESTests(){
#if 0
  //Test 128 bit
  AES a(128); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
  };
  uint8_t keylen = 16;
#endif

#if 0
  //Test 196 bit
  AES a(196); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17
  };
  uint8_t keylen = 24;
#endif

#if 1
  //Test 196 bit
  AES a(256); 
  uint8_t key[] = {
    0x00, 0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f, 0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f
  };
  uint8_t keylen = 32;
#endif
  uint32_t outlen = 0;

  std::string text = "Hello, My Name Is SJ!";
  //Encryption
  //auto encrypted = a.encrypt(input,16,key,keylen,&outlen);
  auto encrypted = a.encrypt(reinterpret_cast<uint8_t*>(text.data()),text.size(),key,keylen,&outlen);
  std::cout<<"Input String: " << text.c_str()<<'\n'<<std::endl;
  std::cout<<"Ecrypted: \n";
  std::cout<<std::hex<<std::internal<<std::setfill('0');
  for(int i = 0; i<outlen;i++)
    std::cout<<std::setw(2)<<unsigned(encrypted[i])<<' ';

  std::cout<<"\n\n";
  //Decryption
  auto decrypted = a.decrypt(encrypted,outlen,key,keylen,&outlen);

  std::cout<<"Decrypted: \n";
  //std::cout<<std::hex<<std::internal<<std::setfill('0');
  for(int i = 0; i<outlen;i++)
    std::cout<<char(decrypted[i]);
  //std::cout<<std::setw(2)<<unsigned(decrypted[i])<<' ';

  std::cout<<'\n';

}
