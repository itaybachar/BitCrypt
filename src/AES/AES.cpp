#include <AES.hh>
#include <cstring>

AES::AES(uint8_t key_len):
  _Nb(4)
{
  this->_Nk = key_len/32;
  switch(this->_Nk){
    case 4:
      this->_Nr = 10;
      break;
    case 6:
      this->_Nr = 12;
      break;
    case 8:
      this->_Nr = 14;
      break;
  }
  this->block_byte_size = this->_Nb * 4;
}

//Encrypt a single block
void AES::_encryptBlock(uint8_t in[], uint8_t out[], uint8_t key[]){
  uint8_t* w = new uint8_t[4 * this->_Nb*(this->_Nr+1)];
  _keyExpansion(key,w);

  uint8_t** state = new uint8_t*[4];
  state[0] = new uint8_t[4*this->_Nb];
  for(uint8_t i = 0; i<this->_Nb;i++){
    state[i] = state[0] + i*this->_Nb;
  }

  //Map input to a 2D state
  for(int i = 0; i<4;i++){
    for(int j = 0; j<this->_Nb; j++){
      state[i][j] = in[i+ 4*j];
    }
  }

  _addRoundKey(state,w);
  for(int i = 1; i<this->_Nr;i++){
    _subBytes(state);
    _shiftRows(state);
    _mixCols(state);
    _addRoundKey(state,w+(i*4*this->_Nb));
  }
  _subBytes(state);
  _shiftRows(state);
  _addRoundKey(state,w+(4*this->_Nb*(this->_Nr)));

  for(uint8_t r = 0 ; r<4;r++){
    for(uint8_t c = 0; c<this->_Nb;c++){
      out[r+c*4] = state[r][c];
    }
  }
}

//Calculate extra padding to make input divisible by a block
uint32_t AES::_calcPadding(uint32_t in_len){
  uint32_t blockCount = in_len/this->block_byte_size; 
  if(in_len%this->block_byte_size)
    blockCount++;

  return blockCount*this->block_byte_size;
}

//Copy the input into a padded location
uint8_t* AES::_mapPaddedInput(uint8_t in[], uint32_t in_len, uint32_t padded_len){
  uint8_t* padded = new uint8_t[padded_len];
  memcpy(padded,in,in_len);
  memset(padded+in_len,0x0,padded_len-in_len);
  return padded;
}

uint8_t* AES::encrypt(uint8_t in[], uint32_t in_len, uint8_t key[], uint32_t* out_len){
  *out_len = _calcPadding(in_len);
  uint8_t* paddedInput = _mapPaddedInput(in,in_len,*out_len);
  uint8_t* out = new uint8_t[*out_len];  

  for(uint32_t i = 0; i<*out_len;i+= this->block_byte_size){
    _encryptBlock(paddedInput+i,out+i,key);
  }

  delete[] paddedInput;
  return out;
}


void AES::_keyExpansion(uint8_t key[], uint8_t w[]){

  //Temp Word
  uint8_t* temp = new uint8_t[4];
  uint8_t* rcon = new uint8_t[4];

  uint32_t i = 0; 
  //Copy original key to the first round slot
  while(i < 4 * this->_Nk){
    w[i] = key[i];
    i++;
  }

  i = 4*this->_Nk;
  while(i < 4 * this->_Nb*(1+this->_Nr)){
    //Copy Last Key into temp
    temp[0] = w[i-4];
    temp[1] = w[i-3];
    temp[2] = w[i-2];
    temp[3] = w[i-1];

    if(i/4%this->_Nk == 0){
      _rotWord(temp);
      _subWord(temp);
      _Rcon(rcon,i/(4*this->_Nk));
      _xorWord(temp,rcon,temp);
    }else if(this->_Nk>6 && i/4 % this->_Nk == 4){
      _subWord(temp);
    }

    //Xor previous word with temp to create new word
    _xorWord(w+i-4*this->_Nb,temp,w+i);

    i+=4;
  }

  delete[] temp;
  delete[] rcon;
}
void AES::printState(uint8_t** w){
  std::cout<<std::hex<<std::showbase<<std::internal<<std::setfill('0')<<std::setw(2);
  for(int i =0;i<4;i++){
    for(int j = 0; j<4;j++){
      std::cout<<unsigned(w[i][j])<<" ";
    }
    std::cout<<'\n';
  }
  std::cout<<'\n';
}
void AES::printWord(uint8_t* w){
  std::cout<<std::hex<<std::showbase<<std::internal<<std::setfill('0')<<std::setw(2);
  for(int i = 0; i<4;i++)
    std::cout<<unsigned(w[i])<<" ";
  std::cout<<'\n';
}

void AES::_xorWord(uint8_t left[], uint8_t right[], uint8_t out[]){
  out[0] = left[0]^right[0];
  out[1] = left[1]^right[1];
  out[2] = left[2]^right[2];
  out[3] = left[3]^right[3];
}

void AES::_Rcon(uint8_t arr[], uint8_t n){
  uint8_t rval = 1;

  for(uint8_t i = 0; i<n-1;i++){
    rval = _xtime(rval);
  }
  arr[0] = rval;
  arr[1] = arr[2] = arr[3] = 0;
}

uint8_t AES::_xtime(uint8_t x){
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
void AES::_rotWord(uint8_t word[]){
  uint32_t* q = (uint32_t*)word;

  *q = *q<<24 | *q>>8;

  word = (uint8_t*)q;
}

void AES::_subWord(uint8_t word[]){
  for(uint8_t i = 0; i<4; i++){
    word[i] = sbox[word[i]/16][word[i]%16];
  }
}

void AES::_subBytes(uint8_t** state){
  for(uint8_t i = 0; i<4; i++){
    for(uint8_t j = 0; j<this->_Nb;j++){
      state[i][j] = sbox[state[i][j]/16][state[i][j]%16];
    }
  }
}

void AES::_shiftRows(uint8_t** state){
  for(uint8_t i = 1; i<4;i++){
    for(int8_t count = 0; count<i;count++)
      _rotWord(state[i]);
  }
}

uint8_t AES::_mulBytes(uint8_t a, uint8_t b){
  uint8_t p = 0x0;
  uint8_t bit = 0x0,low_bit = 0x01, high_bit = 0x80;
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

void AES::_mixCols(uint8_t** state){
  uint8_t res[4];
  for(uint8_t c = 0;c<this->_Nb;c++){
    //Multiply and Add
    res[0] = _mulBytes(0x02,state[0][c]) ^ _mulBytes(0x03,state[1][c]) ^ state[2][c] ^ state[3][c];
    res[1] = state[0][c] ^ _mulBytes(0x02,state[1][c]) ^ _mulBytes(0x03,state[2][c]) ^ state[3][c];
    res[2] = state[0][c] ^ state[1][c] ^ _mulBytes(0x02,state[2][c]) ^ _mulBytes(0x03,state[3][c]);
    res[3] = _mulBytes(0x03,state[0][c]) ^ state[1][c] ^ state[2][c] ^ _mulBytes(0x02,state[3][c]);

    //Update state
    state[0][c] = res[0];
    state[1][c] = res[1];
    state[2][c] = res[2];
    state[3][c] = res[3];
  }
}

void AES::_addRoundKey(uint8_t** state,uint8_t*w){
  for(uint8_t r = 0; r<4;r++){
    for(uint8_t c = 0; c< this->_Nb;c++){
      state[r][c] = state[r][c] ^ w[r+c*4];
    }
  }
}
