#include "BitCrypt.hh"

#include "argon2.h"
#include "FileEditor2.hh"

#include <cstring>

BitCrypt::BitCrypt(uint8_t _keyLen) :
  _keyLen(_keyLen),
  _aes(128)
{
  _updateKeyLen();
}

void BitCrypt::_updateKeyLen(){
  switch(_keyLen){
    case AES_128:
      this->_hashLen = 16; 
      this->_headerSize = 16;
      this->_aes.setType(128);
      break;
    case AES_192:
      this->_hashLen = 24;
      this->_headerSize = 32;
      this->_aes.setType(192);
      break;
    case AES_256:
      this->_hashLen = 32;
      this->_headerSize = 32;
      this->_aes.setType(256);
      break;
    default:
      throw std::runtime_error("Bad AES Key Length");
  }
}

bool BitCrypt::encryptFile(const char* filepath,const char* key, uint32_t keyLen){
  //Declaring variables
  uint8_t* in = new uint8_t[FileEditor2::s_SIZE];
  uint8_t* out;
  uint32_t outLen = 0;

  //Hash the input key
  uint8_t* hash = _hashKey(key,keyLen);

  //Load file and prepare it
  FileEditor2 inFile(filepath);
  FileEditor2* outFile = inFile.prepareFile();

  //Write Header to file
  outFile->writeBytes(&this->_keyLen,1);

  //Encrypt hash and write it to out
  out = _aes.encrypt(hash, _hashLen, hash, _hashLen, &outLen);
  outFile->writeBytes(out,outLen);

  //Encrypt rest of file
  ssize_t read = 0;
  while((read = inFile.readBytes(in,FileEditor2::s_SIZE)) > 0){
    out = _aes.encrypt(in, read, hash, _hashLen, &outLen);
    outFile->writeBytes(out,outLen);
    delete[] out;
  }

  delete outFile;
  delete[] in;
  delete[] hash;
  return true;
}

uint8_t* BitCrypt::_hashKey(const char* key, uint32_t keyLen){
  uint8_t* hash = new uint8_t[this->_hashLen];
  argon2i_hash_raw(this->_tCost, this->_mCost, this->_parallel,
      key, keyLen, this->salt, this->_saltLen, hash, this->_hashLen);
  return hash;
}

//Checks if a file is encrypted, and if the key is correct
//Return
//  -1: no header found
//   0: Header Found, passwords do not match
//   1: Header Found, passwords match
int8_t BitCrypt::checkFile(const char* filepath, const char* key, uint32_t keyLen){
  //Load File
  FileEditor2 inFile(filepath);

  //Read AES Type
  uint8_t encType = 0;
  inFile.readBytes(&encType,1);

  //Check if file size exists
  if(encType == AES_128 || encType == AES_192 || encType == AES_256){
    //Update Key Length variables
    this->_keyLen = encType;
    _updateKeyLen();

    //Hashed Key
    uint8_t* hashedKey = _hashKey(key,keyLen);

    //Read the encrypted Hash
    uint8_t inputHash[this->_headerSize];
    inFile.readBytes(inputHash,_headerSize);

    //Decrypt the hash
    uint32_t outLen = 0;
    uint8_t* decryptedHash = _aes.decrypt(inputHash,_headerSize,hashedKey,_hashLen, &outLen);

    bool ret = _compareHash(hashedKey,decryptedHash,_hashLen);
    delete[] hashedKey;
    delete[] decryptedHash;
    return ret;
  }
  return -1;
}

bool BitCrypt::_compareHash(uint8_t* h1, uint8_t* h2, uint32_t size){

  for(uint32_t i = 0; i<size;i++){
    if(h1[i] != h2[i]){
      return false;
    }
  }
  return true;
}


bool BitCrypt::decryptFile(const char* filepath, const char* key, uint32_t keyLen){
  //Declaring variables
  uint8_t* in = new uint8_t[FileEditor2::s_SIZE];
  uint8_t* out;
  uint32_t outLen = 0;

  //Hash the input key
  uint8_t* hash = _hashKey(key,keyLen);

  //Load file and prepare it
  FileEditor2 inFile(filepath);
  FileEditor2*  outFile = inFile.prepareFile();

  //Skip header and hash
  inFile.skip(1);
  inFile.skip(_headerSize);

  //Encrypt rest of file
  ssize_t read = 0;
  while((read = inFile.readBytes(in,FileEditor2::s_SIZE)) > 0){
    out = _aes.decrypt(in, read, hash, _hashLen, &outLen);
    _cleanDecryption(out, &outLen);
    outFile->writeBytes(out,outLen);
    delete[] out;
  }

  delete outFile;
  delete[] in;
  delete[] hash;
  return true;
}

void BitCrypt::_cleanDecryption(uint8_t* in, uint32_t* size){
  if(!*size)
    return;
  uint32_t count = 0;
  while(in[*size-1-count] == 0x0){
    count++;
    if(*size == count)
      break;
  }

  *size = *size-count;
}

void BitCrypt::setEncryptionType(uint8_t encType){
  if(encType == AES_128 || encType == AES_192 || encType == AES_256){
    //Update Key Length variables
    this->_keyLen = encType;
    _updateKeyLen();
  }
}
