/*
  ==============================================================================

    SchroederReverberator.cpp
    Created: 21 Jul 2020 5:03:27pm
    Author:  melcm

  ==============================================================================
*/

#include "SchroederReverberator.h"
#include <cmath>

SchroederReverberator::SchroederReverberator(int sampleRate)
{

    m_allpasses[0] = new AllPassFilter(sampleRate, 0.005, 0.6, 0.7);
    m_allpasses[1] = new AllPassFilter(sampleRate, 0.0017, 0.6, 0.7);

    this->m_rt60 = 0.5;

    m_time_combs[0] = 0.03;
    m_time_combs[1] = 0.035;
    m_time_combs[2] = 0.037;
    m_time_combs[3] = 0.04;
    
    for (int i = 0; i < NB_COMBS; i++)
    {
        m_combfilters[i] = new CombFilter(sampleRate, m_time_combs[i], 1, exp(-3 * m_time_combs[i] / m_rt60));
    }
}

SchroederReverberator::~SchroederReverberator()
{
    for (int i = 0; i < NB_ALLPASS; i++)
    {
        delete this->m_allpasses[i];
    }
}

float SchroederReverberator::getRt60()
{
    return this->m_rt60;
}

void SchroederReverberator::setRt60(double rt60)
{
    this->m_rt60 = rt60;

    /*for (int i = 0; i < NB_COMBS; i++)
    {
        m_combfilters[i]->setGain(exp(-3 * m_time_combs[i] / m_rt60));
    }*/

    updateGain();
}

// updates the delay of the comb filters, by default if you want the value unchanged you should provide a delay of -1
void SchroederReverberator::updateCombDelays(double delays[NB_COMBS])
{
    for (int i = 0; i < NB_COMBS; i++)
    {
        if (delays[i] < 0.045 && delays[i] > 0.03)
        {
            m_combfilters[i]->setDelay(delays[i]);
            m_time_combs[i] = delays[i];
        }
    }

    updateGain();
}

void SchroederReverberator::updateGain()
{
    //update gains to match RT60
    for (int i = 0; i < NB_COMBS; i++)
    {
        m_combfilters[i]->setGain(pow(10.0, -3 * m_combfilters[i]->getDelay() / m_rt60));
    }
}


float SchroederReverberator::next(float sample)
{
    /** float tmp = this->allpasses[0]->next(sample);
    tmp = this->allpasses[1]->next(tmp);
    tmp = this->allpasses[2]->next(tmp); */

    float tempVal = 0.;
    for (int i = 0; i < NB_COMBS; i++)
    {
        tempVal += m_combfilters[i]->next(sample);
    }

    float val0 = tempVal;
    float val1 = 0;
    for (int i = 0; i < NB_ALLPASS; i++)
    {
        val1 = m_allpasses[i]->next(val0);
        val0 = val1;
    }
    return val1;
}
