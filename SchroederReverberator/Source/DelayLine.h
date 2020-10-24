/*
  ==============================================================================

    DelayLine.h
    Created: 21 Jul 2020 3:46:29pm
    Author:  lomn

  ==============================================================================
*/

#pragma once

class DelayLine
{
public:

    DelayLine(int sampleRate, double delay, double delayMax);
    ~DelayLine();

    double getDelay();
    void setDelay(double delay);
    float next(float sample);
    void prepareToPlay();
    void clearBuffer();

    float readSample();
    void writeSample(float sample);


private:
    double m_delaySec;
    int m_delaySample;
    double m_reminderDelaySample;
    float* m_circularBuffer;
    int m_sampleRate;
    int m_circularBufferLen;

    int m_readIndex;
    int m_writeIndex;
};