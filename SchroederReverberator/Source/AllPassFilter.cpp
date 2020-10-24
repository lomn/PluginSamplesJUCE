/*
  ==============================================================================

    AllPassFilter.cpp
    Created: 24 Jul 2020 1:44:28pm
    Author:  melcm

  ==============================================================================
*/

#include "AllPassFilter.h"

AllPassFilter::AllPassFilter(int sampleRate, double delay, double delayMax, float gain)
{
    this->m_sampleRate = sampleRate;
    this->m_delay = delay;
    this->m_gain = gain;

    this->delayLine = new DelayLine(sampleRate, delay, delayMax);
}

AllPassFilter::~AllPassFilter()
{
    delete delayLine;
}

float AllPassFilter::getGain()
{
    return this->m_gain;
}

float AllPassFilter::getDelay()
{
    return this->m_delay;
}

void AllPassFilter::setGain(float gain)
{
    this->m_gain = gain;
}

void AllPassFilter::setDelay(double delay)
{
    this->m_delay = delay;
    this->delayLine->setDelay(delay);
}

void AllPassFilter::clearBuffer()
{
    this->delayLine->clearBuffer();
}

float AllPassFilter::next(float sample)
{
    float delayed = this->delayLine->readSample();
    float v = sample + delayed * m_gain;
    this->delayLine->writeSample(v);

    return -m_gain * v + delayed;//(-1 * m_gain) * sample + delayed * (1 - m_gain*m_gain);
}
