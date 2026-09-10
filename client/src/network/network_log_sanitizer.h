#pragma once

#include <QString>

namespace xpilot
{
    class NetworkLogSanitizer
    {
    public:
        static QString sanitize(const QString& data);
    };
}
