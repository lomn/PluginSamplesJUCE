/*
  ==============================================================================

    SchroederReverberator.h
    Created: 21 Jul 2020 5:03:27pm
    Author:  melcm

  ==============================================================================
*/

#pragma once

#include "AllPassFilter.h"
#include "CombFilter.h"

#define NB_COMBS 4
#define NB_ALLPASS 2

class SchroederReverberator
{
public:
    /*SchroederReverberator(int sampleRate, float rt60,
        double cDelay1, double cDelay2, double cDelay3, double cDelay4,
            double aDelay1, double aDelay2, float aGain1, float aGain2);*/

    SchroederReverberator(int sampleRate);
    ~SchroederReverberator();

    float getRt60();
    void setRt60(double rt60);

    void updateCombDelays(double delays[NB_COMBS]);

    float next(float sample);

private:

    void updateGain();

    AllPassFilter* m_allpasses[NB_ALLPASS];
    CombFilter* m_combfilters[NB_COMBS];
    double m_rt60;
    double m_time_combs[NB_COMBS];

};