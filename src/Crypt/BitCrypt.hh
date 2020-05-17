#pragma once

#include <stdint.h>

#define AES_128 0b0001
#define AES_192 0b0010
#define AES_256 0b0100

class AES;
class FileEditor;

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
  //This contains the type of AES and the encrypted hash. If AES 192 is used,
  //The size becomes 32+1, 16 bytes is a size of a state in AES so we round up.
  uint32_t _headerSize;
  AES* _aes;
  FileEditor* _f;

private:

  bool _compareHash(uint8_t* h1, uint8_t* h2, uint32_t size);
  void _updateKeyLen();
  void _cleanDecryption(uint8_t* in, uint32_t* size);

  uint8_t* _hashKey(const char* key, uint32_t keyLen);
public:
  BitCrypt(uint8_t _keyLen = AES_128);

  bool checkFile(const char* filepath, const char* key, uint32_t keyLen);
  bool encryptFile(const char* filepath, const char* key, uint32_t keyLen);
  bool decryptFile(const char* filepath, const char* key, uint32_t keyLen);

};
