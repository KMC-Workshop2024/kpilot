/*
 * K-Pilot: private FSD pilot client
 * Copyright (C) 2026 K-Pilot contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SERVER_ENDPOINT_H
#define SERVER_ENDPOINT_H

#include <QString>

namespace xpilot
{
    class ServerEndpoint
    {
    public:
        static QString normalizeAddress(const QString &address);
        static bool isValidAddress(const QString &address);
        static bool isValidPort(int port);
    };
}

#endif
