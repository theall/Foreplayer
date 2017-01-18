
#include "samplesfilter.h"

#include <math.h>

 TSamplesFilter::TSamplesFilter(int samplerate)
{
    int i;

    for(i=0 ; i < FILT_TAPS ; i++)
    {
        mFilterWork[0].dt[i] = mFilterWork[1].dt[i] = 0;
    }

    mFilterWork[0].rd=0;
    mFilterWork[1].rd=0;
    mFilterWork[0].wt=FILT_TAPS-1;
    mFilterWork[1].wt=FILT_TAPS-1;

    ftq(&mFilterWork[0],samplerate,samplerate/2,1.0);
    ftq(&mFilterWork[1],samplerate,samplerate/2,1.0);
}

void TSamplesFilter::ftq(FilterWork *p, long rate, long cutoff, double boost)
{
	double	total=(1<<FILT_BASE_BITS);
	double	r,fs,fc;
#ifndef IIR_FILTER
	long	i;
#endif

	fs=(double)rate;
	fc=(double)cutoff;
#ifndef IIR_FILTER
	//(CUTOFF*2)<=rate

    if((fc*2)>fs)	fc=fs/2;		//cutoff freq imitting
	total *= boost;
	for(i=0;i<FILT_TAPS;i++)
	{
		r=(2*PI*fc/fs)*((double)i-(double)(FILT_TAPS/2));
		if(r==0)
		{
			p->q[i]=(long)(2.0*total*fc/fs);
		}
		else
		{
			p->q[i]=(long)((2.0*total*fc/fs)*(sin(r)/r));
		}
	}
#else
	r=(2.0 * fc) / fs;
	if(r > 1) r=1;
	p->q[0]=(long)(r * total);			//coef
	p->q[1]=(long)((1.0-r) * total);	//feedback
#endif
}

#ifndef IIR_FILTER
#if 0
static long TSamplesFilter::FT_DT(FILTWK *p,long dt)
{
	long	i, d, idx;

	p->dt[p->wt++]=dt;	
	if(p->wt >= FILT_TAPS)	p->wt=0;
	idx=p->rd;			
	for(i=0,d=0;i<FILT_TAPS;i++)		
	{
		d += p->dt[idx++] * p->q[i];		
		if(idx >= FILT_TAPS)	idx=0;		
	}						
	p->rd++;					
	if(p->rd >= FILT_TAPS)	p->rd=0;		
	return (d>>FILT_BASE_BITS);			
}
#endif
long TSamplesFilter::do0(long dt)
{
	long	i, d, idx;

    mFilterWork[0].dt[mFilterWork[0].wt++]=dt;
    if(mFilterWork[0].wt >= FILT_TAPS)	mFilterWork[0].wt=0;
    idx=mFilterWork[0].rd;
	for(i=0,d=0;i<FILT_TAPS;i++)		
	{
        d += mFilterWork[0].dt[idx++] * mFilterWork[0].q[i];
		if(idx >= FILT_TAPS)	idx=0;		
	}						
    mFilterWork[0].rd++;
    if(mFilterWork[0].rd >= FILT_TAPS)	mFilterWork[0].rd=0;
	return (d>>FILT_BASE_BITS);			
}

long TSamplesFilter::do1(long dt)
{
	long	i, d, idx;

    mFilterWork[1].dt[mFilterWork[1].wt++]=dt;
    if(mFilterWork[1].wt >= FILT_TAPS)	mFilterWork[1].wt=0;
    idx=mFilterWork[1].rd;
	for(i=0,d=0;i<FILT_TAPS;i++)		
	{
        d += mFilterWork[1].dt[idx++] * mFilterWork[1].q[i];
		if(idx >= FILT_TAPS)	idx=0;		
	}						
    mFilterWork[1].rd++;
    if(mFilterWork[1].rd >= FILT_TAPS)	mFilterWork[1].rd=0;
    return (d>>FILT_BASE_BITS);
}

void TSamplesFilter::filter(int dwSamples, short *out)
{
    int i;
    int max = 0;
    int min = 0x7fff;
    int silent = -1;
    // and lowpass filter the results
    for (i = 0; i < dwSamples*2; i += 2)
    {
        // apply the "headphone mix" too
        if (mHeadMix > 0.0f)
        {
            signed long ot1, ot2;

            ot1 = (long)((float)out[i+1] * mHeadMix);
            ot2 = (long)((float)out[i] * mHeadMix);

            out[i] = (ot1 + out[i])>>1;
            out[i+1] = (ot2 + out[i+1])>>1;
        }

#if SHOW_CLIPPING
        if ((out[i] == SHRT_MAX) || (out[i] == SHRT_MIN)) printf("CLIP!\n");
        if ((out[i+1] == SHRT_MAX) || (out[i+1] == SHRT_MIN)) printf("CLIP!\n");
#endif
        if (out[i] > max) max = out[i];
        if (out[i] < min) min = out[i];
        if (out[i+1] > max) max = out[i+1];
        if (out[i+1] < min) min = out[i+1];
        out[i] = do0((long)out[i]);
        out[i+1] = do1((long)out[i+1]);
        if ((out[i] < 16 && out[i] > -16) && (out[i+1] < 16 && out[i+1] > -16))
        {	// if we've not yet been marked "noisy" this frame, mark silent
            if (silent == -1)
            {
                silent = 1;
            }
        }
        else
        {
            silent = 0;
        }
    }

    // Normalise
    if ((mNormalize) && (mVolume == 1.0f))
    {
        max = (abs(min) > max) ? abs(min) : max;
        int calc = 32000 / (double)max;
        if (calc < 1.0) calc = 1.0;
        for (i = 0; i < dwSamples*2; i += 2)
        {
            out[i] *= (short)calc;
            out[i+1] *= (short)calc;
        }

        int cur_norm = (int)(calc*100.0);
    }
    else
    {
        for (i = 0; i < dwSamples*2; i+=2)
        {
            out[i] = (short)((float)out[i] * mVolume);
            out[i+1] = (short)((float)out[i+1] * mVolume);
        }
    }

    // now apply the post volume
    if (mPostVolume != 1.0f)
    {
        for (i = 0; i < dwSamples*2; i+=2)
        {
            out[i] = (short)((float)out[i] * mPostVolume);
            out[i+1] = (short)((float)out[i+1] * mPostVolume);
        }
    }
}

#else
static long TSamplesFilter::FT_DT(FILTWK *p,long dt)
{
	p->dt[0]=(((dt * p->q[0]) + (p->dt[0] * p->q[1]))>>FILT_BASE_BITS);
	return p->dt[0];
}
static long TSamplesFilter::FT_DT0(long dt)
{
    mFilterWork[0].dt[0]=(((dt * mFilterWork[0].q[0]) + (mFilterWork[0].dt[0] * mFilterWork[0].q[1]))>>FILT_BASE_BITS);
    return mFilterWork[0].dt[0];
}
static long TSamplesFilter::FT_DT1(long dt)
{
    mFilterWork[1].dt[0]=(((dt * mFilterWork[1].q[0]) + (mFilterWork[1].dt[0] * mFilterWork[1].q[1]))>>FILT_BASE_BITS);
    return mFilterWork[1].dt[0];
}
#endif



