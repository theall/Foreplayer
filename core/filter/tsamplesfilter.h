#ifndef TSAMPLESFILTER_H
#define TSAMPLESFILTER_H

#define FILT_TAPS		5
#define FILT_BASE_BITS	8

#define	PI	3.14159265358979

//--Filter work struct
typedef struct
{
    long	q[FILT_TAPS];
    long	dt[FILT_TAPS];
    long	rd;	//read point
    long	wt;	//write point
} FilterWork;

class TSamplesFilter
{
public:
    TSamplesFilter(int sampleRate);

    long do0(long dt);
    long do1(long dt);

    void filter();

private:
    FilterWork mFilterWork[2];
    void ftq(FilterWork *p, long rate, long cutoff, double boost);
};

#endif
