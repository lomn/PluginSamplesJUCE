/*
  ==============================================================================

    AllPassFilter.h
    Created: 24 Jul 2020 1:44:28pm
    Author:  melcm

  ==============================================================================
*/

#pragma once

#include "DelayLine.h"

class AllPassFilter
{
public:

    AllPassFilter(int sampleRate, double delay, double delayMax, float gain);
    ~AllPassFilter();

    float getGain();
    float getDelay();

    void setGain(float gain);
    void setDelay(double delay);

    void prepareToPlay();
    void clearBuffer();
    float next(float sample);

private:

    float m_gain;
    double m_delay;
    int m_sampleRate;

    DelayLine* delayLine;
};