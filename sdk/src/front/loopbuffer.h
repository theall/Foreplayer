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
#ifndef TLOOPBUFFER_H
#define TLOOPBUFFER_H

#define DEFAULT_BUF_SIZE        32768
typedef unsigned char byte;

class IDataCallback
{
public:
    virtual void read(byte *buf, int size) = 0;
};

class TLoopBuffer
{
public:
    TLoopBuffer(int bufSize = DEFAULT_BUF_SIZE);
    ~TLoopBuffer();

    void resize(int newSize);
    int capcity();
    int segmentGap(int segSize);
    void reset();
    void update();
    void read(byte *buf, int size);
    void setCallback(IDataCallback *callback, int sampleSize);

private:
    byte *mBuf;
    byte *mBufEnd;
    byte *mDataStart;
    byte *mDataEnd;
    int mBufSize;
    int mPos;
    int mDataSize;
    int mSegmentSize;
    IDataCallback *mCallback;
    void alloc(int size);
};

#endif // TLOOPBUFFER_H
