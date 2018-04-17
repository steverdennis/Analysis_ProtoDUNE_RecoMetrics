/**
 *  @file   src/EventClass.cc
 *
 *  @brief  Implementation of the event class.
 *
 *  $Log: $
 */

#include "EventClass.h"

#include "TChain.h"

namespace analysis
{ 

EventClass::EventClass(const std::string &rootFilePath, const std::string &verboseString, const int momentum) : 
    m_rootFilePath(rootFilePath),
    m_verboseString(verboseString),
    m_momentum(momentum)
{
    m_pTChain = new TChain("Validation");
    m_pTChain->Add(m_rootFilePath.c_str());
}

//------------------------------------------------------------------------------------------------------------------------------------------

EventClass::~EventClass()
{
    delete m_pTChain;
}

//------------------------------------------------------------------------------------------------------------------------------------------

EventClass::EventClass(const EventClass &rhs) : 
    m_rootFilePath(rhs.m_rootFilePath),
    m_verboseString(rhs.m_verboseString),
    m_momentum(rhs.m_momentum)
{
    m_pTChain = new TChain("Validation");
    m_pTChain->Add(m_rootFilePath.c_str());
}

//------------------------------------------------------------------------------------------------------------------------------------------

EventClass &EventClass::operator=(const EventClass &rhs)
{
    if (this != &rhs)
    {
        m_rootFilePath = rhs.m_rootFilePath;
        m_verboseString = rhs.m_verboseString;
        m_momentum = rhs.m_momentum;
        m_pTChain = new TChain("Validation");
        m_pTChain->Add(m_rootFilePath.c_str());
    }

    return *this;
}

} // namespace analysis
