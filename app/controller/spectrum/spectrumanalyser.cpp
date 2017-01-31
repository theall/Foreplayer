/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "spectrumanalyser.h"
#include "spectrumutils.h"
#include "spectrum.h"

#include <qmath.h>
#include <qmetatype.h>
#include <QThread>

#include <fftreal/fftreal_wrapper.h>

TSpectrumAnalyserThread::TSpectrumAnalyserThread(QObject *parent)
    :   QObject(parent)
    ,   mFFT(new FFTRealWrapper)
    ,   mNumSamples(SpectrumLengthSamples)
    ,   mWindowFunction(DefaultWindowFunction)
    ,   mWindow(SpectrumLengthSamples, 0.0)
    ,   mInput(SpectrumLengthSamples, 0.0)
    ,   mOutput(SpectrumLengthSamples, 0.0)
    ,   mSpectrum(SpectrumLengthSamples)
    ,   mThread(new QThread(this))
{
    // moveToThread() cannot be called on a QObject with a parent
    setParent(0);
    moveToThread(mThread);
    mThread->start();

    calculateWindow();
}

TSpectrumAnalyserThread::~TSpectrumAnalyserThread()
{
#ifndef DISABLE_FFT
    delete mFFT;
#endif
}

void TSpectrumAnalyserThread::setWindowFunction(WindowFunction type)
{
    mWindowFunction = type;
    calculateWindow();
}

void TSpectrumAnalyserThread::calculateWindow()
{
    for (int i=0; i<mNumSamples; ++i) {
        DataType x = 0.0;

        switch (mWindowFunction) {
        case NoWindow:
            x = 1.0;
            break;
        case HannWindow:
            x = 0.5 * (1 - qCos((2 * M_PI * i) / (mNumSamples - 1)));
            break;
        default:
            Q_ASSERT(false);
        }

        mWindow[i] = x;
    }
}

void TSpectrumAnalyserThread::calculateSpectrum(const QByteArray &buffer,
                                                int inputFrequency,
                                                int bytesPerSample)
{
#ifndef DISABLE_FFT
    Q_ASSERT(buffer.size() == mNumSamples * bytesPerSample);

    // Initialize data array
    const char *ptr = buffer.constData();
    for (int i=0; i<mNumSamples; ++i) {
        const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
        // Scale down to range [-1.0, 1.0]
        const DataType realSample = pcmToReal(pcmSample);
        const DataType windowedSample = realSample * mWindow[i];
        mInput[i] = windowedSample;
        ptr += bytesPerSample;
    }

    // Calculate the FFT
    mFFT->calculateFFT(mOutput.data(), mInput.data());

    // Analyze output to obtain amplitude and phase for each frequency
    for (int i=2; i<=mNumSamples/2; ++i) {
        // Calculate frequency of this complex sample
        mSpectrum[i].frequency = qreal(i * inputFrequency) / (mNumSamples);

        const qreal real = mOutput[i];
        qreal imag = 0.0;
        if (i>0 && i<mNumSamples/2)
            imag = mOutput[mNumSamples/2 + i];

        const qreal magnitude = qSqrt(real*real + imag*imag);
        qreal amplitude = SpectrumAnalyserMultiplier * qLn(magnitude);

        // Bound amplitude to [0.0, 1.0]
        mSpectrum[i].clipped = (amplitude > 1.0);
        amplitude = qMax(qreal(0.0), amplitude);
        amplitude = qMin(qreal(1.0), amplitude);
        mSpectrum[i].amplitude = amplitude;
    }
#endif

    emit calculationComplete(mSpectrum);
}


//=============================================================================
// TSpectrumAnalyser
//=============================================================================

TSpectrumAnalyser::TSpectrumAnalyser(QObject *parent)
    :   QObject(parent)
    ,   mThread(new TSpectrumAnalyserThread(this))
    ,   mState(Idle)
#ifdef DUMP_SPECTRUMANALYSER
    ,   m_count(0)
#endif
{
    connect(mThread, SIGNAL(calculationComplete(TFrequencySpectrum)),
                    this, SLOT(calculationComplete(TFrequencySpectrum)));
}

TSpectrumAnalyser::~TSpectrumAnalyser()
{

}

#ifdef DUMP_SPECTRUMANALYSER
void TSpectrumAnalyser::setOutputPath(const QString &outputDir)
{
    m_outputDir.setPath(outputDir);
    m_textFile.setFileName(m_outputDir.filePath("spectrum.txt"));
    m_textFile.open(QIODevice::WriteOnly | QIODevice::Text);
    m_textStream.setDevice(&m_textFile);
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void TSpectrumAnalyser::setWindowFunction(WindowFunction type)
{
    const bool b = QMetaObject::invokeMethod(mThread, "setWindowFunction",
                              Qt::AutoConnection,
                              Q_ARG(WindowFunction, type));
    Q_ASSERT(b);
    Q_UNUSED(b) // suppress warnings in release builds
}

void TSpectrumAnalyser::calculate(const QByteArray &buffer)
{
    if (isReady()) {
        const int bytesPerSample = 2;

#ifdef DUMP_SPECTRUMANALYSER
        m_count++;
        const QString pcmFileName = m_outputDir.filePath(QString("spectrum_%1.pcm").arg(m_count, 4, 10, QChar('0')));
        QFile pcmFile(pcmFileName);
        pcmFile.open(QIODevice::WriteOnly);
        const int bufferLength = m_numSamples * bytesPerSample;
        pcmFile.write(buffer, bufferLength);

        m_textStream << "TimeDomain " << m_count << "\n";
        const qint16* input = reinterpret_cast<const qint16*>(buffer);
        for (int i=0; i<m_numSamples; ++i) {
            m_textStream << i << "\t" << *input << "\n";
            input += format.channels();
        }
#endif

        mState = Busy;

        // Invoke TSpectrumAnalyserThread::calculateSpectrum using QMetaObject.  If
        // m_thread is in a different thread from the current thread, the
        // calculation will be done in the child thread.
        // Once the calculation is finished, a calculationChanged signal will be
        // emitted by m_thread.
        const bool b = QMetaObject::invokeMethod(mThread, "calculateSpectrum",
                                  Qt::AutoConnection,
                                  Q_ARG(QByteArray, buffer),
                                  Q_ARG(int, 44100),
                                  Q_ARG(int, bytesPerSample));
        Q_ASSERT(b);
        Q_UNUSED(b) // suppress warnings in release builds

#ifdef DUMP_SPECTRUMANALYSER
        m_textStream << "FrequencySpectrum " << m_count << "\n";
        FrequencySpectrum::const_iterator x = m_spectrum.begin();
        for (int i=0; i<m_numSamples; ++i, ++x)
            m_textStream << i << "\t"
                         << x->frequency << "\t"
                         << x->amplitude<< "\t"
                         << x->phase << "\n";
#endif
    }
}

bool TSpectrumAnalyser::isReady() const
{
    return (Idle == mState);
}

void TSpectrumAnalyser::cancelCalculation()
{
    if (Busy == mState)
        mState = Cancelled;
}


//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void TSpectrumAnalyser::calculationComplete(const TFrequencySpectrum &spectrum)
{
    Q_ASSERT(Idle != mState);
    if (Busy == mState)
        emit spectrumChanged(spectrum);
    mState = Idle;
}
