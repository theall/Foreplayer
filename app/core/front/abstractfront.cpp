#include "abstractfront.h"


TAbstractFront::TAbstractFront() :
    mSamplesSize(0),
    mSamples(NULL),
    mPlaying(false),
    mCallback(NULL)
{

}

TAbstractFront::~TAbstractFront()
{

}

void TAbstractFront::setCallback(TRequestSamples callback)
{
    mCallback = callback;
}

void TAbstractFront::requestNextSamples(int n, short *samples)
{
    if(mCallback)
    {
        mCallback(n, samples);
        if(!mSamples || n>mSamplesSize)
        {
            free(mSamples);
            mSamples = (short*)malloc(n*sizeof(short));
        }
        mSamplesSize = n;
        memcpy(mSamples, samples, n);
    }
}

void TAbstractFront::currentSamples(int *size, short **samples)
{
    *size = mSamplesSize;
    *samples = mSamples;
}
