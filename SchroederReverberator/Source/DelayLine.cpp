/*
  ==============================================================================

    DelayLine.cpp
    Created: 21 Jul 2020 3:46:29pm
    Author:  lomn

  ==============================================================================
*/

#include "DelayLine.h"
#include <math.h>
#include <JuceHeader.h>

DelayLine::DelayLine(int sampleRate, double delay, double delayMax)
{
    this->m_sampleRate = sampleRate;
    this->m_circularBufferLen = (int)((double)sampleRate * delayMax) + 1;
    this->m_circularBuffer = (float*)malloc(sizeof(float) * this->m_circularBufferLen);

    if (!this->m_circularBuffer)
    {
        exit(0);
    }

    memset(this->m_circularBuffer, 0.f, sizeof(float) * this->m_circularBufferLen);
    this->m_readIndex = 0;
    this->m_writeIndex = 0;

    this->setDelay(0.);
    if (delay >= 0. || delay <= delayMax)
    {
        this->setDelay(delay);
    }
}

DelayLine::~DelayLine()
{
    free(this->m_circularBuffer);
}

double DelayLine::getDelay()
{
    return this->m_delaySec;
}

/**
* SetDelay takes a delay in seconds in argument. if this delay is negative (which isn't possible) it throws an error.
*
*/
void DelayLine::setDelay(double delay)
{
//    assert(delay < 0 && delay == this->m_delaySec);
    if (delay < 0)
    {
        throw 20;
    }
    if (delay == this->m_delaySec)
    {
        return;
    }

    this->m_delaySec = delay;

    /** sDelay is the delay in samples. It is calculated as a float as it might not be a round number,
    * if so it is taken into account when the buffer is read (cf the next(flaot) function)
    */
    double sDelay = this->m_delaySec * (double)this->m_sampleRate;

    this->m_delaySample = (int)floor(sDelay);
    this->m_reminderDelaySample = sDelay - ((double)this->m_delaySample); //the reminder will be usefull in the next function.

    /** update the writing index with the newly aquired value
    * (N.B you should always update the writeIndex and not the read index as it will mess up when you modify the delay length live
    */
    //this->m_writeIndex = (this->m_readIndex + this->m_delaySample) % this->m_circularBufferLen;
    m_readIndex = m_writeIndex - m_delaySample;

    if (m_readIndex < 0)
    {
        m_readIndex += m_circularBufferLen;
    }
}


//clear buffer before playing
void DelayLine::prepareToPlay()
{
    for (int i = 0; i < this->m_circularBufferLen; i++)
    {
        this->m_circularBuffer[i] = 0;
    }
}

float DelayLine::next(float sample)
{
    /** Formula commonly used that fakes continuity for delay times that are not integer
    * it's a first order aproximation  i.e linear
    */
    float outVal = this->m_circularBuffer[m_readIndex] * (1. - (float)this->m_reminderDelaySample)
        + m_circularBuffer[(m_readIndex + 1) % this->m_circularBufferLen] * (float)this->m_reminderDelaySample;

    this->m_circularBuffer[m_writeIndex] = sample;

    this->m_writeIndex = (this->m_writeIndex + 1) % this->m_circularBufferLen;
    this->m_readIndex = (this->m_readIndex + 1) % this->m_circularBufferLen;

    return outVal;
}

void DelayLine::clearBuffer()
{
    memset(this->m_circularBuffer, 0.f, sizeof(float) * this->m_circularBufferLen);
}

float DelayLine::readSample()
{
    float sample = m_circularBuffer[m_readIndex] * (1. - (float)m_reminderDelaySample)
        + m_circularBuffer[(m_readIndex + 1) % m_circularBufferLen] * (float)m_reminderDelaySample;

    this->m_readIndex = (this->m_readIndex + 1) % this->m_circularBufferLen;

    return sample;
}

void DelayLine::writeSample(float sample)
{
    m_circularBuffer[m_writeIndex] = sample;
    m_writeIndex = (this->m_writeIndex + 1) % this->m_circularBufferLen;
}