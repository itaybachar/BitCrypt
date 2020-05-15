#pragma once

#include <argon2.h>
#include "AES.hh"
#include "FileReader.hh"

#define AES_128 0b0001
#define AES_192 0b0010
#define AES_256 0b0100

class BitCrypt{
private:
  uint8_t _keyLen;

private:
  //Argon2 Variables
  const uint32_t _tCost = 2;
  const uint32_t _mCost = (1<<16);
  const uint32_t _parallel = 1; 
  const uint8_t _saltLen = 16;
  uint8_t* salt;
  uint32_t _hashLen;
    
private:
  bool checkHeader();
public:
  BitCrypt(uint8_t _keyLen = AES_128);
  
  bool checkFile(const char* filepath, const char* key);
  bool encryptFile(const char* filepath, const char* key);
  //Open File
  //


};
