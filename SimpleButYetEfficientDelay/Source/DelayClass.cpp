/*
  ==============================================================================

    DelayClass.cpp
    Created: 1 May 2020 4:53:01pm
    Author:  lomn

  ==============================================================================
*/

#include <math.h>
#include "DelayClass.h"
#include <JuceHeader.h>

Delay::Delay(int sampleRate, double delay, double feedback)
{
    /** Initiate the size of the array knowing that by design the delay does not exceed 5s
    * I'm unsure wether the +1 is really needed or not when the delay time is exactly 5s
    * but I'll keep it here as it can't cause any problem...
    */ 

    this->m_sampleRate = sampleRate;
    this->m_circularBufferLen = sampleRate * 5 + 1;
    this->m_circularBuffer = (float*) malloc(sizeof(float) * this->m_circularBufferLen);

    if (!this->m_circularBuffer)
    {
        exit(0);
    }

    memset(this->m_circularBuffer, 0.f, sizeof(float) * this->m_circularBufferLen);

    // Negative feedback, why not ? Could be usefull for other effects...
    if (feedback <= 1 || feedback >= -1)
    {
        this->setFeedback(feedback);
    }
    else
    {
        this->setFeedback(0.);
    }

    if (delay >= 0. || delay <= 5.)
    {
        this->setDelay(delay);
    }
    else
    {
        this->setDelay(1.);
    }
    this->m_readIndex = 0;
    this->m_writeIndex = 0;
    this->m_reminderDelaySample = 0.f;
}

Delay::~Delay()
{
    free(this->m_circularBuffer);
}

double Delay::getDelay()
{
    return this->m_delaySec;
}

double Delay::getFeedback()
{
    return this->m_feedback;
}


void Delay::setFeedback(double feedback)
{
    if (feedback == this->m_feedback)
    {
        return;
    }
    this->m_feedback = feedback;
}


/**
* SetDelay takes a delay in seconds in argument. if this delay is negative (which isn't possible) it throws an error.
* 
*/
void Delay::setDelay(double delay)
{
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
    double sDelay = this->m_delaySec * (double) this->m_sampleRate;

    this->m_delaySample = (int) floor(sDelay);
    this->m_reminderDelaySample = sDelay - ((double)this->m_delaySample); //the reminder will be usefull in the next function.

    /** update the writing index with the newly aquired value 
    * (N.B you should always update the writeIndex and not the read index as it will mess up when you modify the delay length live
    */
    this->m_writeIndex = (this->m_readIndex + this->m_delaySample) % this->m_circularBufferLen;
}


//clear buffer before playing
void Delay::prepareToPlay()
{
    for (int i = 0; i < this->m_circularBufferLen; i++)
    {
        this->m_circularBuffer[i] = 0;
    }
}

float Delay::clamp(float val)
{
    if (val >= 1)
    {
        return 1;
    }
    if (val <= -1)
    {
        return -1;
    }
    return val;
}

float Delay::next(float sample)
{
    /** Formula commonly used that fakes continuity for delay times that are not integer
    * it's a first order aproximation  i.e linear
    */
    float outVal = this->m_circularBuffer[m_readIndex] * (1. - (float)this->m_reminderDelaySample)
        + m_circularBuffer[(m_readIndex + 1) % this->m_circularBufferLen] * (float)this->m_reminderDelaySample;

    this->m_circularBuffer[m_writeIndex] = this->clamp(sample + this->m_circularBuffer[m_readIndex] * (float) this->m_feedback);

    this->m_writeIndex = (this->m_writeIndex + 1) % this->m_circularBufferLen;
    this->m_readIndex = (this->m_readIndex + 1) % this->m_circularBufferLen;

    return outVal;
}

void Delay::clearBuffer()
{
    memset(this->m_circularBuffer, 0.f, sizeof(float) * this->m_circularBufferLen);
}