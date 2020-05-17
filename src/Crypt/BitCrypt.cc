#include "BitCrypt.hh"

#include <argon2.h>
#include "AES.hh"
#include "FileEditor.hh"

BitCrypt::BitCrypt(uint8_t _keyLen) :
  _keyLen(_keyLen)
{
  //Set Salt to kB3;pK1#bG8}lX3)
//this->salt = uint8_t{0x6b, 0x42, 0x33, 0x3b, 0x70, 0x4b, 0x31, 0x23, 0x62, 0x47, 0x38, 0x7d, 0x6c, 0x58, 0x33, 0x29};

switch(_keyLen){
  case AES_128:
    _hashLen = 16; 
    break;
  case AES_192:
    _hashLen = 24;
    break;
  case AES_256:
    _hashLen = 32;
    break;
  default:
    throw std::runtime_error("Bad AES Key Length");
}

}

bool encryptFile(const char* filepath,const char* key){
  //Hash the input key
  //uint8_t * hash = hashKey(key);
  return false;
}
