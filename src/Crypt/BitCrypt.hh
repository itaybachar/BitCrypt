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

/*
  fe.loadFile();
  fe.readBytes(,1)
  //Code for key len
  fe.readBytes(,32)//max
  //If found key size
  aes.decrypt(buf)
  checkHash(pass,buf)
    outlen = readBytes(,fe.fileSize())
    clearFile()
    written = writeBytes(dec,outSize);

  //Could not find key len, assume is decrypted
  readNum = de.read(,fe.size())
  out[] = new [1 + hashSize + readNum]
  encsize = aes.enc(&buf[1],hashedpass) //Skip first byte of header
  clearFile();
  fe.write(out,encsize);
   */
};
