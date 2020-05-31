#include <BitCrypt.hh>

#include <argon2.h>
#include <FileEditor.hh>

#include <stdexcept>
#include <cstring>

BitCrypt::BitCrypt(uint8_t keyLen) :
  m_aes(128),
  m_keyLen(keyLen)
{
  updateKeyLen();
}

void BitCrypt::updateKeyLen(){
  switch(m_keyLen){
    case AES_128:
      this->m_hashLen = 16; 
      this->m_headerSize = 16;
      this->m_aes.setType(128);
      break;
    case AES_192:
      this->m_hashLen = 24;
      this->m_headerSize = 32;
      this->m_aes.setType(192);
      break;
    case AES_256:
      this->m_hashLen = 32;
      this->m_headerSize = 32;
      this->m_aes.setType(256);
      break;
    default:
      throw std::runtime_error("Bad AES Key Length");
  }
}

bool BitCrypt::encryptFile(const char* filepath,const char* key, uint32_t keyLen){
  //Declaring variables
  uint8_t* in = new uint8_t[FileEditor::s_SIZE];
  uint8_t* out;
  uint32_t outLen = 0;

  //Hash the input key
  uint8_t* hash = hashKey(key,keyLen);

  //Load file and prepare it
  FileEditor inFile(filepath);
  FileEditor* outFile = inFile.prepareFile();

  //Write Header to file
  outFile->writeBytes(&m_keyLen,1);

  //Encrypt hash and write it to out
  out = m_aes.encrypt(hash, m_hashLen, hash, m_hashLen, &outLen);
  outFile->writeBytes(out,outLen);
  delete[] out;

  //Encrypt and Write Size to file
  uint8_t sizePtr[8];
  std::memcpy(sizePtr, inFile.getSize(), 8);

  out = m_aes.encrypt(sizePtr, 8, hash, m_hashLen, &outLen);
  outFile->writeBytes(out, outLen);
  delete[] out;

  //Encrypt rest of file
  ssize_t read = 0;
  while((read = inFile.readBytes(in,FileEditor::s_SIZE)) > 0){
    out = m_aes.encrypt(in, read, hash, m_hashLen, &outLen);
    outFile->writeBytes(out,outLen);
    delete[] out;
  }

  //Delete Tmp file
  inFile.deleteFile();
  outFile->~FileEditor();

  delete[] in;
  delete[] hash;
  return true;
}

uint8_t* BitCrypt::hashKey(const char* key, uint32_t keyLen){
  uint8_t* hash = new uint8_t[m_hashLen];
  argon2i_hash_raw(m_tCost, m_mCost, m_parallel,
      key, keyLen, m_salt, m_saltLen, hash, m_hashLen);
  return hash;
}

//Checks if a file is encrypted, and if the key is correct
//Return
//  -1: no header found
//   0: Header Found, passwords do not match
//   1: Header Found, passwords match
int8_t BitCrypt::checkFile(const char* filepath, const char* key, uint32_t keyLen){
  //Load File
  FileEditor inFile(filepath);

  //Read AES Type
  uint8_t encType = 0;
  inFile.readBytes(&encType,1);

  //Check if file size exists
  if(encType == AES_128 || encType == AES_192 || encType == AES_256){
    //Update Key Length variables
    m_keyLen = encType;
    updateKeyLen();

    //Hashed Key
    uint8_t* hashedKey = hashKey(key,keyLen);

    //Read the encrypted Hash
    uint8_t inputHash[m_headerSize];
    inFile.readBytes(inputHash,m_headerSize);

    //Decrypt the hash
    uint32_t outLen = 0;
    uint8_t* decryptedHash = m_aes.decrypt(inputHash,m_headerSize,hashedKey,m_hashLen, &outLen);

    bool ret = compareHash(hashedKey,decryptedHash,m_hashLen);
    delete[] hashedKey;
    delete[] decryptedHash;
    return ret;
  }
  return -1;
}

bool BitCrypt::compareHash(uint8_t* h1, uint8_t* h2, uint32_t size){
  for(uint32_t i = 0; i<size;i++){
    if(h1[i] != h2[i]){
      return false;
    }
  }
  return true;
}


bool BitCrypt::decryptFile(const char* filepath, const char* key, uint32_t keyLen){
  //Declaring variables
  uint8_t* in = new uint8_t[FileEditor::s_SIZE];
  uint8_t* out;
  uint32_t outLen = 0; //size of current decrypted block
  uint64_t outSize = 0; //size of original file
  ssize_t read = 0;

  //Hash the input key
  uint8_t* hash = hashKey(key,keyLen);

  //Load file and prepare it
  FileEditor inFile(filepath);
  FileEditor* outFile = inFile.prepareFile();

  //Skip header and hash
  inFile.skip(1);
  inFile.skip(m_headerSize);

  //Read file size
  read = inFile.readBytes(in, 16); //Read the next State block containing size
  out = m_aes.decrypt(in, read, hash, m_hashLen, &outLen);
  
  memcpy(&outSize, out, 8);
  delete[] out;

  //Encrypt rest of file
  while((read = inFile.readBytes(in,FileEditor::s_SIZE)) > 0){
    out = m_aes.decrypt(in, read, hash, m_hashLen, &outLen);
	if(read != FileEditor::s_SIZE)
		//cleanDecryption(out, &outLen);

	if (outSize < outLen)
		outLen = outSize;

    outFile->writeBytes(out,outLen);
	outSize -= outLen;

    delete[] out;
  }

  //Delete tmp File
  inFile.deleteFile();
  outFile->~FileEditor();

  delete[] in;
  delete[] hash;
  return true;
}

void BitCrypt::cleanDecryption(uint8_t* in, uint32_t* size){
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
    m_keyLen = encType;
    updateKeyLen();
  }
}
