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

EventClass::EventClass(const std::string &rootFilePath, const std::string &verboseString, const int momentum, const std::string treeName) :
    m_rootFilePath(rootFilePath),
    m_verboseString(verboseString),
    m_momentum(momentum),
    m_treeName(treeName)
{
    m_pTChain = new TChain(m_treeName.c_str());

    StringVector tokens;
    this->TokenizeString(m_rootFilePath, tokens, ":");
    for (const std::string token : tokens)
        m_pTChain->Add(token.c_str());
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
    m_momentum(rhs.m_momentum),
    m_treeName(rhs.m_treeName)
{
    m_pTChain = new TChain(m_treeName.c_str());

    StringVector tokens;
    this->TokenizeString(m_rootFilePath, tokens, ":");
    for (const std::string token : tokens)
        m_pTChain->Add(token.c_str());
}

//------------------------------------------------------------------------------------------------------------------------------------------

EventClass &EventClass::operator=(const EventClass &rhs)
{
    if (this != &rhs)
    {
        m_rootFilePath = rhs.m_rootFilePath;
        m_verboseString = rhs.m_verboseString;
        m_momentum = rhs.m_momentum;
        m_treeName = rhs.m_treeName;
        m_pTChain = new TChain(m_treeName.c_str());

        StringVector tokens;
        this->TokenizeString(m_rootFilePath, tokens, ":");
        for (const std::string token : tokens)
            m_pTChain->Add(token.c_str());
    }

    return *this;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void EventClass::TokenizeString(const std::string &inputString, StringVector &tokens, const std::string &delimiter)
{
    std::string::size_type lastPos = inputString.find_first_not_of(delimiter, 0);
    std::string::size_type pos     = inputString.find_first_of(delimiter, lastPos);

    while ((std::string::npos != pos) || (std::string::npos != lastPos))
    {
        tokens.push_back(inputString.substr(lastPos, pos - lastPos));
        lastPos = inputString.find_first_not_of(delimiter, pos);
        pos = inputString.find_first_of(delimiter, lastPos);
    }
}

} // namespace analysis
