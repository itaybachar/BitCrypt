#pragma once

#include <stdint.h>

#define AES_128 0b0001
#define AES_192 0b0010
#define AES_256 0b0100

class BitCrypt{
private:
  uint8_t _keyLen;

private:
  //Argon2 Variables
  static constexpr uint32_t _tCost = 2;
  static constexpr uint32_t _mCost = (1<<16);
  static constexpr uint32_t _parallel = 1; 
  static constexpr uint8_t _saltLen = 16;
  const uint8_t salt[_saltLen] = {0x6b, 0x42, 0x33, 0x3b, 0x70, 0x4b, 0x31, 0x23, 0x62, 0x47, 0x38, 0x7d, 0x6c, 0x58, 0x33, 0x29};
  uint8_t _hashLen;

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
