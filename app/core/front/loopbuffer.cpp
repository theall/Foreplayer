#include "loopbuffer.h"

#include <memory.h>
#include <stdlib.h>

TLoopBuffer::TLoopBuffer(int bufSize) :
    mBuf(NULL)
  , mBufEnd(NULL)
  , mDataStart(NULL)
  , mDataEnd(NULL)
  , mBufSize(0)
  , mPos(0)
  , mDataSize(0)
  , mSegmentSize(0)
  , mCallback(NULL)
{
    alloc(bufSize);
}

TLoopBuffer::~TLoopBuffer()
{
    if(mBuf)
    {
        delete mBuf;
        mBuf = NULL;
    }
}

void TLoopBuffer::resize(int newSize)
{
    alloc(newSize);
}

int TLoopBuffer::capcity()
{
    return mBufSize;
}

int TLoopBuffer::segmentGap(int segSize)
{
    return (mBufSize-mPos) / segSize;
}

void TLoopBuffer::reset()
{
    mDataStart = mBuf;
    mDataEnd = mBuf;
    mDataSize = 0;
    mPos = 0;
}

void TLoopBuffer::update()
{
    if(!mCallback || mSegmentSize<1)
        return;

    int segGap = (mBufEnd-mDataEnd) / mSegmentSize;
    if(segGap > 0)
    {
        for(int i=0;i<segGap;i++)
        {
            mCallback->read(mDataEnd, mSegmentSize);
            mDataEnd += mSegmentSize;
        }
    } else {
        int dataSize = mDataEnd - mDataStart;
        int gapSize = mDataStart - mBuf;
        if(gapSize > dataSize)
        {
            memcpy(mBuf, mDataStart, dataSize);
            mDataStart = mBuf;
            mDataEnd = mDataStart + dataSize;

            // Read more data
            update();
        } else {
            // Pending, wait data being read
        }
    }
}

void TLoopBuffer::read(byte *buf, int size)
{
    int dataSize = mDataEnd - mDataStart;
    if(dataSize < size)
    {
        update();
    }
    // Read buf
    memcpy(buf, mDataStart, size);
    mDataStart += size;
}

void TLoopBuffer::setCallback(IDataCallback *callback, int sampleSize)
{
    mCallback = callback;
    mSegmentSize = sampleSize * 4;
}

void TLoopBuffer::alloc(int size)
{
    if(mBuf)
        delete mBuf;

    mBuf = (byte*)malloc(size);
    mBufSize = size;
    mBufEnd = mBuf + mBufSize - 1;
    reset();
}
