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
    EventClass(const std::string &rootFilePath, const std::string &verboseString, const std::string treeName = "Validation");

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
    TChain *GetTChain() const;

    /**
     *  @brief  Return a descriptive string
     */
    std::string GetDescription() const;

    /**
     *  @brief  Return name of root tree
     */
    std::string GetTreeName() const;

    typedef std::vector<std::string> StringVector;
    /*
     *  @brief  Convert deliminated string to vector of strings
     *
     *  @param  inputString input string
     *  @param  tokens output vector of strings
     *  @param  delimiter
     */
    void TokenizeString(const std::string &inputString, StringVector &tokens, const std::string &delimiter);

private:
    std::string  m_rootFilePath;               ///< Location of root files
    StringVector m_rootFilePaths;              ///< Location of root file paths
    std::string  m_verboseString;              ///< Verbose string describing reconstruction
    std::string  m_treeName;                   ///< Name of root tree
    TChain      *m_pTChain;                    ///< TChain of root files
};

typedef std::vector<EventClass> EventClassVector;

//------------------------------------------------------------------------------------------------------------------------------------------

inline TChain* EventClass::GetTChain() const
{
    return m_pTChain;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string EventClass::GetDescription() const
{
    return m_verboseString;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string EventClass::GetTreeName() const
{
    return m_treeName;
}

} // namespace analysis

#endif // #ifndef EVENT_CLASS_H
