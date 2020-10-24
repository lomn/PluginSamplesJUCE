/*
  ==============================================================================

    CombFilter.cpp
    Created: 21 Jul 2020 3:44:36pm
    Author:  lomn

  ==============================================================================
*/

#include "CombFilter.h"

CombFilter::CombFilter(int sampleRate, double delay, double delayMax, float gain)
{
    this->m_sampleRate = sampleRate;
    this->m_delay = delay;
    this->m_gain = gain;

    this->delayLine = new DelayLine(sampleRate, delay, delayMax);

    return;
}

CombFilter::~CombFilter()
{
    delete delayLine;
}

float CombFilter::getGain()
{
    return this->m_gain;
}

double CombFilter::getDelay()
{
    return this->m_delay;
}

void CombFilter::setGain(float gain)
{
    this->m_gain = gain;
}

void CombFilter::setDelay(double delay)
{
    this->delayLine->setDelay(delay);
    this->m_delay = delay;
}

void CombFilter::clearBuffer()
{
    this->delayLine->clearBuffer();
}

float CombFilter::next(float sample)
{
    float delayed = this->delayLine->readSample();
    float out = delayed * m_gain + sample;
    this->delayLine->writeSample(out);

    return delayed * this->m_gain + sample;
}