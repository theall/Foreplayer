/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

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
    memset(mBuf, 0, mBufSize);
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
    if(!mBuf)
        mBuf = (byte*)malloc(size);
    else
        mBuf = (byte*)realloc(mBuf, size);

    mBufSize = size;
    mBufEnd = mBuf + mBufSize - 1;
    reset();
}
