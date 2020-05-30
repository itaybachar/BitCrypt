#include "FileEditor2.hh"

//String
#include <string>

//File Reading
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdexcept>

//Memcpy
#include <cstring>

//Check for O_BINARY, needed for Windows writing
#ifndef O_BINARY
#define O_BINARY 0
#endif

FileEditor2::FileEditor2(const char* filepath, bool write) :
  m_avail(0),
  m_filepath(filepath)
{
  //Open File
  if(write){
    m_flags = O_WRONLY | O_BINARY | O_CREAT;
    m_fd = open(m_filepath,m_flags,S_IRUSR | S_IWUSR);

    if(m_fd<0)
      throw std::runtime_error("Could not open file for writing!");

  }else{
    m_flags = O_RDONLY | O_BINARY;
    m_fd = open(m_filepath,m_flags);

    if(m_fd<0)
      throw std::runtime_error("Could not open file for reading!");

    //Prepare read buffer
    m_prebuf = new uint8_t[s_SIZE + s_PREBUFSIZE];
    m_buf = m_prebuf + s_PREBUFSIZE;
    m_cur = m_buf;
  }
}

FileEditor2::~FileEditor2(){
  //Close Current File
  if(m_fd>=0)
    close(m_fd);

  //Delete Buffers
  delete[] m_prebuf;
}

//Takes in the input file(to read), renames it to ***.tmp and creates 
//another FileEditor2 creating a new file with the original name for wiritng
FileEditor2* FileEditor2::prepareFile(){
  //Rename file
  std::string temp = m_filepath;
  temp += ".tmp";
  rename(m_filepath, temp.c_str());

  //Create new File Editor for writing
  FileEditor2* writeFile = new FileEditor2(m_filepath,true);

  //Update member variable
  m_filepath = temp.c_str();

  return writeFile;
}

//Reads the next chunk of data to the buffer
void FileEditor2::readChunk(){
  //Copy excess bytes into prebuf
  for(uint32_t i = 0; i<m_avail;i++){
    m_prebuf[(s_PREBUFSIZE-m_avail)+i] = m_cur[i];
  }

  //load the next chunk into memory
  ssize_t bytesRead = read(m_fd,m_buf,s_SIZE);
  m_cur = m_buf-m_avail;
  m_avail += bytesRead;
}

//User read method using readChunk
//numBytes must be <= s_SIZE
ssize_t FileEditor2::readBytes(uint8_t* buf, uint32_t numBytes){
  if(m_flags  == (O_RDONLY | O_BINARY) && numBytes <= s_SIZE){

    //If another chunk of data is needed
    if(numBytes>m_avail)
      readChunk();

    //If still missing bytes(if eof) then return what is left
    if(numBytes>m_avail){
      memcpy(buf,m_cur,m_avail);
      m_cur += m_avail;

      ssize_t totalRead = m_avail;
      m_avail = 0;
      return totalRead;
    }

    //Finally, if there is enough data, copy
    memcpy(buf,m_cur,numBytes);
    m_cur += numBytes;
    m_avail -= numBytes;

    return numBytes;
  }
}

//User write method
ssize_t FileEditor2::writeBytes(uint8_t* buf, uint32_t numBytes){
  if(m_flags & O_WRONLY){
    return write(m_fd,buf,numBytes);
  }
}

void FileEditor2::skip(uint32_t num){
  if(m_flags  == (O_RDONLY | O_BINARY) && num <= s_SIZE){

    //If another chunk of data is needed
    if(num>m_avail)
      readChunk();
    
    if(num>m_avail){
      m_cur += m_avail;
      m_avail = 0;
      return;
    }
    m_cur += num;
    m_avail -= num;
  }
}
