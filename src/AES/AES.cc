#include <AES.hh>

#include <cstring>
#include <stdexcept>

AES::AES(uint32_t key_len):
  m_Nb(4)
{
  setType(key_len);
  this->m_blockSize = this->m_Nb * 4;
}

void AES::setType(uint32_t key_len){
  this->m_Nk = key_len/32;
  switch(this->m_Nk){
    case 4:
      this->m_Nr = 10;
      break;
    case 6:
      this->m_Nr = 12;
      break;
    case 8:
      this->m_Nr = 14;
      break;
    default:
      throw std::runtime_error("Bad Key Length");
  }
}

//Encrypt a single block
void AES::encryptBlock(uint8_t in[], uint8_t out[], uint8_t key[]){
  uint8_t* w = new uint8_t[4 * this->m_Nb*(this->m_Nr+1)];
  keyExpansion(key,w);

  uint8_t** state = new uint8_t*[4];
  state[0] = new uint8_t[4*this->m_Nb];
  for(uint8_t i = 0; i<this->m_Nb;i++){
    state[i] = state[0] + i*this->m_Nb;
  }

  //Map input to a 2D state
  for(int i = 0; i<4;i++){
    for(int j = 0; j<this->m_Nb; j++){
      state[i][j] = in[i+ 4*j];
    }
  }

  addRoundKey(state,w);
  for(int i = 1; i<this->m_Nr;i++){
    subBytes(state);
    shiftRows(state);
    mixCols(state);
    addRoundKey(state,w+(i*4*this->m_Nb));
  }
  subBytes(state);
  shiftRows(state);
  addRoundKey(state,w+(4*this->m_Nb*(this->m_Nr)));

  for(uint8_t r = 0 ; r<4;r++){
    for(uint8_t c = 0; c<this->m_Nb;c++){
      out[r+c*4] = state[r][c];
    }
  }
  delete[] w;

  delete[] *state;
  delete[] state;
}

//Calculate extra padding to make input divisible by a block
uint32_t AES::calcPadding(uint32_t in_len){
  uint32_t blockCount = in_len/this->m_blockSize; 
  if(in_len%this->m_blockSize)
    blockCount++;

  return blockCount*this->m_blockSize;
}

//Copy the input into a padded location
uint8_t* AES::mapPaddedInput(uint8_t in[], uint32_t in_len, uint32_t padded_len){
  uint8_t* padded = new uint8_t[padded_len];
  memcpy(padded,in,in_len);
  memset(padded+in_len,0x0,padded_len-in_len);
  return padded;
}

uint8_t* AES::encrypt(uint8_t in[], uint32_t in_len, uint8_t key[], uint8_t key_len, uint32_t* out_len){

  keyCheck(key_len);

  *out_len = calcPadding(in_len);
  uint8_t* paddedInput = mapPaddedInput(in,in_len,*out_len);
  uint8_t* out = new uint8_t[*out_len];  

  for(uint32_t i = 0; i<*out_len;i+= this->m_blockSize){
    encryptBlock(paddedInput+i,out+i,key);
  }

  delete[] paddedInput;
  return out;
}

uint8_t* AES::decrypt(uint8_t in[], uint32_t in_len, uint8_t key[], uint8_t key_len, uint32_t* out_len){

  keyCheck(key_len);

  *out_len = calcPadding(in_len);
  if(*out_len != in_len)
    throw std::runtime_error("ERR: INVALID_INPUT_FILE");


  uint8_t* out = new uint8_t[*out_len];  

  for(uint32_t i = 0; i<*out_len;i+= this->m_blockSize){
    decryptBlock(in+i,out+i,key);
  }

  return out;
}

void AES::keyExpansion(uint8_t key[], uint8_t w[]){

  //Temp Word
  uint8_t* temp = new uint8_t[4];
  uint8_t* rcon = new uint8_t[4];

  uint32_t i = 0; 
  //Copy original key to the first round slot
  while(i < 4 * this->m_Nk){
    w[i] = key[i];
    i++;
  }

  i = 4*this->m_Nk;
  while(i < 4 * this->m_Nb*(1u+this->m_Nr)){
    //Copy Last Key into temp
    temp[0] = w[i-4];
    temp[1] = w[i-3];
    temp[2] = w[i-2];
    temp[3] = w[i-1];

    if(i/4%this->m_Nk == 0){
      rotWord(temp);
      subWord(temp);
      Rcon(rcon,i/(4*this->m_Nk));
      xorWord(temp,rcon,temp);
    }else if(this->m_Nk>6 && i/4 % this->m_Nk == 4){
      subWord(temp);
    }

    //Xor previous word with temp to create new word
    xorWord(w+i-4*this->m_Nk,temp,w+i);

    i+=4;
  }

  delete[] temp;
  delete[] rcon;
}

void AES::xorWord(uint8_t left[], uint8_t right[], uint8_t out[]){
  out[0] = left[0]^right[0];
  out[1] = left[1]^right[1];
  out[2] = left[2]^right[2];
  out[3] = left[3]^right[3];
}

void AES::Rcon(uint8_t arr[], uint8_t n){
  uint8_t rval = 1;

  for(uint8_t i = 0; i<n-1;i++){
    rval = xtime(rval);
  }
  arr[0] = rval;
  arr[1] = arr[2] = arr[3] = 0;
}

uint8_t AES::xtime(uint8_t x){
  uint8_t mask = 0x80, mod = 0x1b;
  uint8_t high_bit = x&mask;

  //Shift to raise to a power of 2
  x = x<<1;

  //Check for overflow and apply modulus
  if(high_bit){
    x = x^mod; 
  }

  return x;
}

//Rotates left(in big endian) | word ->32bits
void AES::rotWord(uint8_t word[]){
  uint32_t* q = (uint32_t*)word;

  *q = *q<<24 | *q>>8;

  word = (uint8_t*)q;
}

void AES::subWord(uint8_t word[]){
  for(uint8_t i = 0; i<4; i++){
    word[i] = sbox[word[i]/16][word[i]%16];
  }
}

void AES::subBytes(uint8_t** state){
  for(uint8_t i = 0; i<4; i++){
    for(uint8_t j = 0; j<this->m_Nb;j++){
      state[i][j] = sbox[state[i][j]/16][state[i][j]%16];
    }
  }
}

void AES::shiftRows(uint8_t** state){
  for(uint8_t i = 1; i<4;i++){
    for(int8_t count = 0; count<i;count++)
      rotWord(state[i]);
  }
}

uint8_t AES::mulBytes(uint8_t a, uint8_t b){
  uint8_t p = 0x0;
  uint8_t low_bit = 0x01, high_bit = 0x80;
  uint8_t carry = 0x0;

  for(uint8_t i = 0; i < 8;i++){
    if(b&low_bit)
      p= p^ a;

    b= b >> 1;
    carry = a&high_bit;
    a<<=1;
    if(carry)
      a^=0x1b;
  }

  return p;
}

void AES::mixCols(uint8_t** state){
  uint8_t res[4];
  for(uint8_t c = 0;c<this->m_Nb;c++){
    //Multiply and Add
    res[0] = mulBytes(0x02,state[0][c]) ^ mulBytes(0x03,state[1][c]) ^ state[2][c] ^ state[3][c];
    res[1] = state[0][c] ^ mulBytes(0x02,state[1][c]) ^ mulBytes(0x03,state[2][c]) ^ state[3][c];
    res[2] = state[0][c] ^ state[1][c] ^ mulBytes(0x02,state[2][c]) ^ mulBytes(0x03,state[3][c]);
    res[3] = mulBytes(0x03,state[0][c]) ^ state[1][c] ^ state[2][c] ^ mulBytes(0x02,state[3][c]);

    //Update state
    state[0][c] = res[0];
    state[1][c] = res[1];
    state[2][c] = res[2];
    state[3][c] = res[3];
  }
}

void AES::addRoundKey(uint8_t** state,uint8_t*w){
  for(uint8_t r = 0; r<4;r++){
    for(uint8_t c = 0; c< this->m_Nb;c++){
      state[r][c] = state[r][c] ^ w[r+c*4];
    }
  }
}

bool AES::keyCheck(uint8_t len){
  if(this->m_Nk*4 != len)
    throw std::runtime_error("ERR: INVALID_KEY_LEN");
  return true;
}

void AES::decryptBlock(uint8_t in[], uint8_t out[], uint8_t key[]){
  uint8_t* w = new uint8_t[4 * this->m_Nb*(this->m_Nr+1)];
  keyExpansion(key,w);

  uint8_t** state = new uint8_t*[4];
  state[0] = new uint8_t[4*this->m_Nb];
  for(uint8_t i = 0; i<this->m_Nb;i++){
    state[i] = state[0] + i*this->m_Nb;
  }

  //Map input to a 2D state
  for(int i = 0; i<4;i++){
    for(int j = 0; j<this->m_Nb; j++){
      state[i][j] = in[i+ 4*j];
    }
  }

  addRoundKey(state,w+(4*this->m_Nb*(this->m_Nr)));
  for(int i = this->m_Nr-1; i>0;i--){
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state,w+(i*4*this->m_Nb));
    invMixCols(state);
  }
  invShiftRows(state);
  invSubBytes(state);
  addRoundKey(state,w);

  for(uint8_t r = 0 ; r<4;r++){
    for(uint8_t c = 0; c<this->m_Nb;c++){
      out[r+c*4] = state[r][c];
    }
  }

  delete[] w;
  delete[] *state;
  delete[] state;
}

void AES::invShiftRows(uint8_t** state){
  for(uint8_t i = 1; i<4;i++){
    for(int8_t count = 4-i; count>0;count--)
      rotWord(state[i]);
  }
}

void AES::invSubBytes(uint8_t** state){
  for(uint8_t i = 0; i<4; i++){
    for(uint8_t j = 0; j<this->m_Nb;j++){
      state[i][j] = inv_sbox[state[i][j]/16][state[i][j]%16];
    }
  }
}

void AES::invMixCols(uint8_t** state){
  uint8_t res[4];
  for(uint8_t c = 0;c<this->m_Nb;c++){
    //Multiply and Add
    res[0] = mulBytes(0x0e,state[0][c]) ^ mulBytes(0x0b,state[1][c]) ^ mulBytes(0x0d,state[2][c]) ^ mulBytes(0x09,state[3][c]);
    res[1] = mulBytes(0x09,state[0][c]) ^ mulBytes(0x0e,state[1][c]) ^ mulBytes(0x0b,state[2][c]) ^ mulBytes(0x0d,state[3][c]);
    res[2] = mulBytes(0x0d,state[0][c]) ^ mulBytes(0x09,state[1][c]) ^ mulBytes(0x0e,state[2][c]) ^ mulBytes(0x0b,state[3][c]);
    res[3] = mulBytes(0x0b,state[0][c]) ^ mulBytes(0x0d,state[1][c]) ^ mulBytes(0x09,state[2][c]) ^ mulBytes(0x0e,state[3][c]);

    //Update state
    state[0][c] = res[0];
    state[1][c] = res[1];
    state[2][c] = res[2];
    state[3][c] = res[3];
  }
}

//sbox tables
const unsigned char AES::sbox[16][16] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
  0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
  0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
  0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
  0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
  0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
  0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
  0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
  0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
  0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
  0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
  0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
  0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
  0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
  0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
  0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
  0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const unsigned char AES::inv_sbox[16][16] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
  0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
  0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
  0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
  0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
  0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
  0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
  0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
  0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
  0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
  0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
  0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
  0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
  0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
  0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
  0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
  0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};
