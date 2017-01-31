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

#ifndef SPECTRUMANALYSER_H
#define SPECTRUMANALYSER_H

#include <QByteArray>
#include <QObject>
#include <QVector>
#include <QThread>

#include "spectrum.h"
#include "frequencyspectrum.h"
#include <fftreal/fftreal_wrapper.h>
#include <fftreal/FFTRealFixLenParam.h>

class SpectrumAnalyserThreadPrivate;

/**
 * Implementation of the spectrum analysis which can be run in a
 * separate thread.
 */
class TSpectrumAnalyserThread : public QObject
{
    Q_OBJECT

public:
    TSpectrumAnalyserThread(QObject *parent);
    ~TSpectrumAnalyserThread();

public slots:
    void setWindowFunction(WindowFunction type);
    void calculateSpectrum(const QByteArray &buffer,
                           int inputFrequency,
                           int bytesPerSample);

signals:
    void calculationComplete(const TFrequencySpectrum &spectrum);

private:
    void calculateWindow();

private:
    FFTRealWrapper* mFFT;
    const int mNumSamples;
    WindowFunction mWindowFunction;
    typedef FFTRealFixLenParam::DataType DataType;
    QVector<DataType> mWindow;
    QVector<DataType> mInput;
    QVector<DataType> mOutput;
    TFrequencySpectrum mSpectrum;
    QThread* mThread;
};

/**
 * Class which performs frequency spectrum analysis on a window of
 * audio samples, provided to it by the Engine.
 */
class TSpectrumAnalyser : public QObject
{
    Q_OBJECT

public:
    TSpectrumAnalyser(QObject *parent = 0);
    ~TSpectrumAnalyser();

#ifdef DUMP_SPECTRUMANALYSER
    void setOutputPath(const QString &outputPath);
#endif

public:
    /*
     * Set the windowing function which is applied before calculating the FFT
     */
    void setWindowFunction(WindowFunction type);

    /*
     * Calculate a frequency spectrum
     *
     * \param buffer       Audio data
     * \param format       Format of audio data
     *
     * Frequency spectrum is calculated asynchronously.  The result is returned
     * via the spectrumChanged signal.
     *
     * An ongoing calculation can be cancelled by calling cancelCalculation().
     *
     */
    void calculate(const QByteArray &buffer);

    /*
     * Check whether the object is ready to perform another calculation
     */
    bool isReady() const;

    /*
     * Cancel an ongoing calculation
     *
     * Note that cancelling is asynchronous.
     */
    void cancelCalculation();

signals:
    void spectrumChanged(const TFrequencySpectrum &spectrum);

private slots:
    void calculationComplete(const TFrequencySpectrum &spectrum);

private:
    void calculateWindow();

private:

    TSpectrumAnalyserThread*    mThread;

    enum State {
        Idle,
        Busy,
        Cancelled
    };

    State              mState;

#ifdef DUMP_SPECTRUMANALYSER
    QDir                m_outputDir;
    int                 m_count;
    QFile               m_textFile;
    QTextStream         m_textStream;
#endif
};

#endif // SPECTRUMANALYSER_H

