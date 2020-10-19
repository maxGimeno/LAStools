/*
===============================================================================

  FILE:  bytestreamout_array.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com
  
  COPYRIGHT:
  
    (c) 2007-2016, martin isenburg, rapidlasso - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    22 June 2016 -- access to current size for "native LAS 1.4 compressor"
    19 July 2015 -- moved from LASlib to LASzip for "compatibility mode" in DLL
     9 April 2012 -- created after cooking Zuccini/Onion/Potatoe dinner for Mara
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_ARRAY_HPP
#define BYTE_STREAM_OUT_ARRAY_HPP

#include "bytestreamout.hpp"

#include <stdlib.h>
#include <string.h>

class ByteStreamOutArray : public ByteStreamOut
{
public:
  ByteStreamOutArray(I64 alloc=1024);
/* write a single byte                                       */
  las_BOOL putByte(U8 byte);
/* write an array of bytes                                   */
  las_BOOL putBytes(const U8* bytes, U32 num_bytes);
/* is the stream seekable (e.g. standard out is not)         */
  las_BOOL isSeekable() const;
/* get current position of stream                            */
  I64 tell() const;
/* seek to this position in the stream                       */
  las_BOOL seek(const I64 position);
/* seek to the end of the file                               */
  las_BOOL seekEnd();
/* destructor                                                */
  ~ByteStreamOutArray(){};
/* get access to data                                        */
  inline I64 getSize() const { return size; };
  inline I64 getCurr() const { return curr; };
  inline const U8* getData() const { return data; };
  inline U8* takeData() { U8* d = data; data = 0; alloc = 0; size = 0; curr = 0; return d; };
protected:
  U8* data;
  I64 alloc;
  I64 size;
  I64 curr;
};

class ByteStreamOutArrayLE : public ByteStreamOutArray
{
public:
  ByteStreamOutArrayLE(I64 alloc=1024);
/* write 16 bit low-endian field                             */
  las_BOOL put16bitsLE(const U8* bytes);
/* write 32 bit low-endian field                             */
  las_BOOL put32bitsLE(const U8* bytes);
/* write 64 bit low-endian field                             */
  las_BOOL put64bitsLE(const U8* bytes);
/* write 16 bit big-endian field                             */
  las_BOOL put16bitsBE(const U8* bytes);
/* write 32 bit big-endian field                             */
  las_BOOL put32bitsBE(const U8* bytes);
/* write 64 bit big-endian field                             */
  las_BOOL put64bitsBE(const U8* bytes);
private:
  U8 swapped[8];
};

class ByteStreamOutArrayBE : public ByteStreamOutArray
{
public:
  ByteStreamOutArrayBE(I64 alloc=1024);
/* write 16 bit low-endian field                             */
  las_BOOL put16bitsLE(const U8* bytes);
/* write 32 bit low-endian field                             */
  las_BOOL put32bitsLE(const U8* bytes);
/* write 64 bit low-endian field                             */
  las_BOOL put64bitsLE(const U8* bytes);
/* write 16 bit big-endian field                             */
  las_BOOL put16bitsBE(const U8* bytes);
/* write 32 bit big-endian field                             */
  las_BOOL put32bitsBE(const U8* bytes);
/* write 64 bit big-endian field                             */
  las_BOOL put64bitsBE(const U8* bytes);
private:
  U8 swapped[8];
};

inline ByteStreamOutArray::ByteStreamOutArray(I64 alloc)
{
  this->data = (U8*)malloc((U32)alloc);
  this->alloc = 1024;
  this->size = 0;
  this->curr = 0;
}

inline las_BOOL ByteStreamOutArray::putByte(U8 byte)
{
  if (curr == alloc)
  {
    alloc += 1024;
    data = (U8*)realloc(data, (U32)alloc);
    if (data == 0)
    {
      return FALSE;
    }
  }
  data[curr] = byte;
  if (curr == size) size++;
  curr++;
  return TRUE;
}

inline las_BOOL ByteStreamOutArray::putBytes(const U8* bytes, U32 num_bytes)
{
  if ((curr+num_bytes) > alloc)
  {
    alloc += (1024+num_bytes);
    data = (U8*)realloc(data, (U32)alloc);
    if (data == 0)
    {
      return FALSE;
    }
  }
  memcpy((void*)(data+curr), bytes, num_bytes);
  curr += num_bytes;
  if (curr > size) size = curr;
  return TRUE;
}

inline las_BOOL ByteStreamOutArray::isSeekable() const
{
  return TRUE;
}

inline I64 ByteStreamOutArray::tell() const
{
  return curr;
}

inline las_BOOL ByteStreamOutArray::seek(I64 position)
{
  if ((0 <= position) && (position <= size))
  {
    curr = position;
    return TRUE;
  }
  return FALSE;
}

inline las_BOOL ByteStreamOutArray::seekEnd()
{
  curr = size;
  return TRUE;

/*
  if ((0 <= distance) && (distance <= size))
  {
    curr = size - distance;
    return TRUE;
  }
  return FALSE;
*/
}

inline ByteStreamOutArrayLE::ByteStreamOutArrayLE(I64 alloc) : ByteStreamOutArray(alloc)
{
}

inline las_BOOL ByteStreamOutArrayLE::put16bitsLE(const U8* bytes)
{
  return putBytes(bytes, 2);
}

inline las_BOOL ByteStreamOutArrayLE::put32bitsLE(const U8* bytes)
{
  return putBytes(bytes, 4);
}

inline las_BOOL ByteStreamOutArrayLE::put64bitsLE(const U8* bytes)
{
  return putBytes(bytes, 8);
}

inline las_BOOL ByteStreamOutArrayLE::put16bitsBE(const U8* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline las_BOOL ByteStreamOutArrayLE::put32bitsBE(const U8* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline las_BOOL ByteStreamOutArrayLE::put64bitsBE(const U8* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline ByteStreamOutArrayBE::ByteStreamOutArrayBE(I64 alloc) : ByteStreamOutArray(alloc)
{
}

inline las_BOOL ByteStreamOutArrayBE::put16bitsLE(const U8* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline las_BOOL ByteStreamOutArrayBE::put32bitsLE(const U8* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline las_BOOL ByteStreamOutArrayBE::put64bitsLE(const U8* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline las_BOOL ByteStreamOutArrayBE::put16bitsBE(const U8* bytes)
{
  return putBytes(bytes, 2);
}

inline las_BOOL ByteStreamOutArrayBE::put32bitsBE(const U8* bytes)
{
  return putBytes(bytes, 4);
}

inline las_BOOL ByteStreamOutArrayBE::put64bitsBE(const U8* bytes)
{
  return putBytes(bytes, 8);
}

#endif
