#include "FileEditor.hh"

//String
#include <string>

//File Reading
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Puts
#include <stdio.h>

//Memcpy
#include <cstring>

//Check for O_BINARY, needed for Windows writing
#ifndef O_BINARY
#define O_BINARY 0
#endif

FileEditor::FileEditor():
  fd(-1),
  size(0),
  loc(0),
  flags(0),
  avail(0)
{
  prebuf = new uint8_t[preBufSize + SIZE];
  buf = prebuf + preBufSize;
}

FileEditor::~FileEditor(){
  closeFile();

  delete[] prebuf;
}

void FileEditor::readChunk(){
  if(!isWrite){
    for(uint32_t i = 0; i<avail;i++){
      prebuf[(preBufSize-avail)+i] = cur[i];
    }
    ssize_t readBytes = read(fd,buf,SIZE);
    cur = buf-avail;
    avail += readBytes;
  }
}

//Loads a file and sets class variables
bool FileEditor::loadFile(const char filepath[], bool write){

  this->isWrite = write;

  if(isWrite){
    this->flags = O_WRONLY | O_TRUNC  | O_CREAT | O_BINARY;
  } else this->flags = O_RDONLY | O_BINARY;

  this->fd = open(filepath,flags);
  this->filepath = filepath;
  if(fd<0){
    puts("ERROR: Could not find file");
    return false;
  }
  //avail = 0;
  readChunk();

  struct stat fileStats;
  fstat(fd,&fileStats);
  this->size = fileStats.st_size;

  return true;
}

/*
   Reads the currently loaded file(if its size is more than 0)
   Returns number of bytes read of -1 if failed. 
 */
int32_t FileEditor::readBytes(uint8_t* buf,uint32_t numBytes){

  if(!isWrite){

    uint32_t numChunks = numBytes/SIZE;
    if(numBytes % SIZE)
      numChunks++;

    uint32_t read = 0;

    for(uint32_t i = 0; i < numChunks; i++){

      if(i+1 == numChunks){
        if(numBytes <= avail){
          memcpy(buf + read,cur,numBytes);
          return read + numBytes;
        }else{
          memcpy(buf + read,cur,avail);
          return read + avail;
        }
      } else {
        memcpy(buf + read,cur, avail);
        read += avail;
        numBytes -= avail;
        avail = 0;
      }
      readChunk();
    }
  }
  return -1;
}

//Getter Method for size, Also updates variable in case there was a size change
uint32_t FileEditor::fileSize(){
  //Update size
  struct stat buf;
  fstat(fd,&buf);
  this->size = buf.st_size;

  return this->size;
}

//Closes current file
bool FileEditor::closeFile(){
  return close(this->fd);
}

int32_t FileEditor::writeBytes(uint8_t* buf, uint32_t numBytes){

  if(isWrite){
    uint32_t numWrite = write(fd,buf,numBytes);
    return numWrite;
  }

  return -1;
}

uint32_t FileEditor::filePointerLoc(){
  return this->loc;
}

uint8_t* FileEditor::getBuffer(){
  return this->buf;
}

void FileEditor::makeTemp(){
  std::string temp = this->filepath;
  temp += ".tmp";
  if(fd>-1){
    rename(this->filepath,temp.c_str());

  }

}
