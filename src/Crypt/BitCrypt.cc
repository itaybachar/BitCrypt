#include "BitCrypt.hh"

#include "argon2.h"
#include "AES.hh"
#include "FileEditor.hh"

#include <cstring>

BitCrypt::BitCrypt(uint8_t _keyLen) :
  _keyLen(_keyLen)
{
  _updateKeyLen();
  this->_f = new FileEditor();
}

void BitCrypt::_updateKeyLen(){
  switch(_keyLen){
    case AES_128:
      this->_hashLen = 16; 
      this->_headerSize = 16;
      this->_aes = new AES(128);
      break;
    case AES_192:
      this->_hashLen = 24;
      this->_headerSize = 32;
      this->_aes = new AES(192);
      break;
    case AES_256:
      this->_hashLen = 32;
      this->_headerSize = 32;
      this->_aes = new AES(256);
      break;
    default:
      throw std::runtime_error("Bad AES Key Length");
  }
}

bool BitCrypt::encryptFile(const char* filepath,const char* key, uint32_t keyLen){
  //Hash the input key
  uint8_t* hash = _hashKey(key,keyLen);

  //Create a file editor
  this->_f->loadFile(filepath);
  this->_f->makeTemp(); //Rename file as temp

  //Create new file for writing
  FileEditor outFile;
  outFile.loadFile(filepath,true);

  //Write Header to file
  outFile.writeBytes(&this->_keyLen,1);

  //Total Size of final Document (w/o 1 byte head)
  uint32_t expandedSize = this->_headerSize + this->_f->fileSize();

  uint32_t totalChunks = expandedSize/this->_f->SIZE;

  if(expandedSize%this->_f->SIZE)
    totalChunks++;

  //Size is 2^15 or 2048 AES iterations(states)
  uint8_t* encryptionChunk = new uint8_t[this->_f->SIZE];

  uint32_t curChunkSize = _headerSize;

  //Set Hash
  memcpy(&encryptionChunk[0],hash,this->_hashLen);
  //If 192, set the next 8 bytes to 0x0
  if(this->_keyLen == AES_192)
    memset(&encryptionChunk[24],0x0,8);

  //Read file into the rest of the space 
  curChunkSize += this->_f->readBytes(&encryptionChunk[this->_headerSize],this->_f->SIZE-this->_headerSize);

  //Encrypt first chunk
  uint32_t outLen = 0;
  uint8_t* encryptedInput = this->_aes->encrypt(encryptionChunk, curChunkSize, hash,this->_hashLen,&outLen);

  //Write first chunk
  outFile.writeBytes(encryptedInput,outLen);

  for(uint32_t i = 1; i<totalChunks; i++){
    //Read the next chunk from the file
    curChunkSize = this->_f->readBytes(encryptionChunk,this->_f->SIZE);

    //Encrypt first chunk
    encryptedInput = this->_aes->encrypt(encryptionChunk, curChunkSize, hash,this->_hashLen,&outLen);

    //Write first chunk
    outFile.writeBytes(encryptedInput,outLen);
  }

  //Write Encrypted Hash and file
  this->_f->closeFile();

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
  this->_f->loadFile(filepath);

  //Read AES Type
  uint8_t encType = 0;
  this->_f->readBytes(&encType,1);

  //Check if file size exists
  if(encType == AES_128 || encType == AES_192 || encType == AES_256){
    //Update Key Length variables
    this->_keyLen = encType;
    _updateKeyLen();

    //Hashed Key
    uint8_t* hashedKey = _hashKey(key,keyLen);


    //Read the encrypted Hash
    uint8_t inputHash[this->_headerSize];
    this->_f->readBytes(inputHash,this->_headerSize);

    //Decrypt the hash
    uint32_t outLen = 0;
    uint8_t* decryptedHash = this->_aes->decrypt(inputHash,this->_headerSize,hashedKey,this->_hashLen, &outLen);

    return _compareHash(hashedKey,decryptedHash,this->_hashLen);
  }
  return -1;
}

bool BitCrypt::_compareHash(uint8_t* h1, uint8_t* h2, uint32_t size){

  for(uint32_t i = 0; i<size;i++){
    if(h1[i] != h2[i])
      return false;
  }
  return true;
}


//CheckFile must be called before
bool BitCrypt::decryptFile(const char* filepath, const char* key, uint32_t keyLen){
  //Load File into memory
  uint8_t* hashedKey = _hashKey(key,keyLen);

  uint32_t bytesToRead = this->_f->fileSize() - this->_f->filePointerLoc(); 
  uint8_t encryptedIn[bytesToRead];
  this->_f->readBytes(encryptedIn,bytesToRead);

  //Decrypt Input
  uint32_t outLen = 0;
  uint8_t* decryptedIn = this->_aes->decrypt(encryptedIn,bytesToRead,hashedKey, this->_hashLen,&outLen);
  _cleanDecryption(decryptedIn, &outLen);

  //Write to file
  this->_f->writeBytes(decryptedIn,outLen);
  this-_f->closeFile();

  return true;
}

void BitCrypt::_cleanDecryption(uint8_t* in, uint32_t* size){
  uint32_t count = 0;
  for(uint32_t i = 0; i<*size;i++)
    if(i == *size-1 && in[i] == 0x0)
      count++;
    else if(in[i] == in[i+1] && in[i] == 0x0)
      count++;
  *size = *size-count;
}

void BitCrypt::setEncryptionType(uint8_t encType){
  if(encType == AES_128 || encType == AES_192 || encType == AES_256){
    //Update Key Length variables
    this->_keyLen = encType;
    _updateKeyLen();
  }
}
