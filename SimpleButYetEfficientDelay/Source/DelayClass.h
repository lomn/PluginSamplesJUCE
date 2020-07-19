/*
  ==============================================================================

    DelayClass.h
    Created: 1 May 2020 4:53:01pm
    Author:  lomn

  ==============================================================================
*/

#pragma once

class Delay
{
public:

    Delay(int sampleRate, double delay, double feedback);
    ~Delay();

    double getDelay();
    double getFeedback();
    void setDelay(double delay);
    void setFeedback(double feedback);
    float next(float sample);
    void prepareToPlay();
    void clearBuffer();

private:
    double m_delaySec;
    int m_delaySample;
    double m_reminderDelaySample;
    double m_feedback;
    float *m_circularBuffer;
    int m_sampleRate;
    int m_circularBufferLen;

    int m_readIndex;
    int m_writeIndex;

    float clamp(float val);
};