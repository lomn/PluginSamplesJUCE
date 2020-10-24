/*
  ==============================================================================

    CombFilter.h
    Created: 21 Jul 2020 3:44:36pm
    Author:  lomn

  ==============================================================================
*/

#pragma once

#include "DelayLine.h"

class CombFilter
{
public:

    CombFilter(int sampleRate, double delay, double delayMax, float gain);
    ~CombFilter();

    //getters and setters
    float getGain();
    double getDelay();

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