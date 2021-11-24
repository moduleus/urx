/*!
 * Copyright Moduleus 2019
 * \file uff_translation.h
 * \brief
 */

#ifndef UFF_TRANSLATION_H
#define UFF_TRANSLATION_H

// UFF
#include "uff_object.h"

namespace uff
{
    
/**
 * @brief The UFF Translation class
 */
class Translation : public uff::Object
{
    UFF_TYPE_MACRO(Translation, uff::Object);

public:

    explicit Translation(double tx = 0.0, double ty = 0.0, double tz = 0.0)
    {
        setX(tx);
        setY(ty);
        setZ(tz);
    }

    void printSelf(std::ostream& os, std::string indent) const override;
    
    double x() const { return m_tx; }
    void setX(double x) { m_tx = x; }

    double y() const { return m_ty; }
    void setY(double y) { m_ty = y; }

    double z() const { return m_tz; }
    void setZ(double z) { m_tz = z; }

    bool operator ==(const Translation& other) const
    {
        return ((m_tx == other.m_tx) &&
            (m_ty == other.m_ty) &&
            (m_tz == other.m_tz));
    }

    inline bool operator !=(const Translation& other) const
    {
        return !(*this == other);
    }

private:
    double m_tx;
    double m_ty;
    double m_tz;
};

} // namespace uff

#endif // UFF_TRANSLATION_H