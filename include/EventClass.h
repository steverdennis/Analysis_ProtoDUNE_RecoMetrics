/**
 *  @file   include/EventClass.h
 *
 *  @brief  Header file for the event class.
 *
 *  $Log: $
 */

#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H 1

#include <string>
#include <vector>

class TChain;

namespace analysis
{

class EventClass
{
public: 
    /**
     *  @brief  Constructor
     */
    EventClass(const std::string &rootFilePath, const std::string &verboseString, const int momentum);

    /**
     *   @brief  Destructor
     */
    ~EventClass();

    /**
     *   @brief  Copy constructor
     */
    EventClass(const EventClass &rhs);

    /**
     *   @brief  Assignment operator
     */
    EventClass &operator=(const EventClass &rhs);

    /**
     *  @brief  Return a TChain to root files 
     */
    TChain *GetTChain();

    /**
     *  @brief  Return a descriptive string
     */
    std::string GetDescription();

private:
    std::string m_rootFilePath;  ///< Location of root files
    std::string m_verboseString; ///< Verbose string describing reconstruction
    int         m_momentum;      ///< Momentum of beam particles in sample
    TChain     *m_pTChain;       ///< TChain of root files
};

typedef std::vector<EventClass> EventClassVector;

//------------------------------------------------------------------------------------------------------------------------------------------

inline TChain* EventClass::GetTChain()
{
    return m_pTChain;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string EventClass::GetDescription()
{
    return m_verboseString;
}

} // namespace analysis

#endif // #ifndef EVENT_CLASS_H
