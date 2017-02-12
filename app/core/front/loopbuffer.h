#ifndef TLOOPBUFFER_H
#define TLOOPBUFFER_H

#define DEFAULT_BUF_SIZE        65536
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
