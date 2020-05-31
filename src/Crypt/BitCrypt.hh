#pragma once

#include <AES.hh>

#define AES_128 0b0001
#define AES_192 0b0010
#define AES_256 0b0100

class AES;

class BitCrypt{
private:
  //Argon2 Variables
  static constexpr uint32_t m_tCost = 2;
  static constexpr uint32_t m_mCost = (1<<16);
  static constexpr uint32_t m_parallel = 1; 
  static constexpr uint8_t m_saltLen = 16;

  const uint8_t m_salt[m_saltLen] = {0x6b, 0x42, 0x33, 0x3b, 0x70, 0x4b, 0x31, 0x23, 0x62, 0x47, 0x38, 0x7d, 0x6c, 0x58, 0x33, 0x29}; 
  uint8_t m_hashLen;

  //This contains the type of AES and the encrypted hash. If AES 192 is used,
  //The size becomes 32+1, 16 bytes is a size of a state in AES so we round up.
  uint32_t m_headerSize;
  AES m_aes;
  uint8_t m_keyLen;

private:
  bool compareHash(uint8_t* h1, uint8_t* h2, uint32_t size);
  void updateKeyLen();
  void cleanDecryption(uint8_t* in, uint32_t* size);

  uint8_t* hashKey(const char* key, uint32_t keyLen);
public:
  BitCrypt(uint8_t keyLen = AES_128);

  int8_t checkFile(const char* filepath, const char* key, uint32_t keyLen);
  bool encryptFile(const char* filepath, const char* key, uint32_t keyLen);
  bool decryptFile(const char* filepath, const char* key, uint32_t keyLen);
  void setEncryptionType(uint8_t encType);

};
