/*!
 * Copyright Moduleus
 * \file uff_version.h
 * \brief
 */

#ifndef UFF_VERSION_H
#define UFF_VERSION_H

// UFF
#include "uff_object.h"

namespace uff
{
    
/**
 * @brief The UFF Version class
 */
class Version : public uff::Object
{
    UFF_TYPE_MACRO(Version, uff::Object);

public:

    explicit Version(uint32_t major = UFF_VERSION_MAJOR, uint32_t minor = UFF_VERSION_MINOR, uint32_t patch = UFF_VERSION_PATCH)
    {
        m_major = major;
        m_minor = minor;
        m_patch = patch;
    }

    void printSelf(std::ostream& os, std::string indent) const override;

    uint32_t major() const { return m_major; }
    uint32_t minor() const { return m_minor; }
    uint32_t patch() const { return m_patch; }

    inline bool operator ==(const Version& other) const
    {
        return ((m_major == other.m_major) &&
            (m_minor == other.m_minor) &&
            (m_patch == other.m_patch));
    }

    inline bool operator !=(const Version& other) const
    {
        return !(*this == other);
    }

private:
    uint32_t m_major;
    uint32_t m_minor;
    uint32_t m_patch;
};

} // namespace uff

#endif // UFF_VERSION_H