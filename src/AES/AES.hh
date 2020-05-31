#pragma once
#include <stdint.h>

class AES{
private:
  uint8_t m_Nb;
  uint8_t m_Nk;
  uint8_t m_Nr;
  uint32_t m_blockSize;
  
  //Substitution Boxes
  static const unsigned char sbox[16][16];
  static const unsigned char inv_sbox[16][16];
private:
  bool keyCheck(uint8_t len);

  void addRoundKey(uint8_t** state, uint8_t* w);

  void subBytes(uint8_t** state);
  void invSubBytes(uint8_t** state);

  void shiftRows(uint8_t** state);
  void invShiftRows(uint8_t** state);

  void mixCols(uint8_t** state);
  void invMixCols(uint8_t** state);

  void encryptBlock(uint8_t in[], uint8_t out[], uint8_t key[]);
  void decryptBlock(uint8_t in[], uint8_t out[], uint8_t key[]);

  uint32_t calcPadding(uint32_t in_len);
  uint8_t* mapPaddedInput(uint8_t in[], uint32_t in_len, uint32_t padded_len);

  void keyExpansion(uint8_t key[], uint8_t w[]);
  void rotWord(uint8_t word[]);
  void subWord(uint8_t word[]);
  void xorWord(uint8_t left[], uint8_t right[], uint8_t out[]);

  uint8_t xtime(uint8_t x);
  uint8_t mulBytes(uint8_t a, uint8_t b);
  void Rcon(uint8_t arr[], uint8_t n);
public:
  AES(uint32_t key_len);
  uint8_t* encrypt(uint8_t in[], uint32_t in_len, uint8_t key[],uint8_t key_len, uint32_t* out_len);

  uint8_t* decrypt(uint8_t in[], uint32_t in_len, uint8_t key[],uint8_t key_len, uint32_t* out_len);

  void setType(uint32_t keyLen);
};

