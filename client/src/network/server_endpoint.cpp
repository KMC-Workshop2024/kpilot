/*
 * K-Pilot: private FSD pilot client
 * Copyright (C) 2026 K-Pilot contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "server_endpoint.h"

#include <QHostAddress>
#include <QRegularExpression>

namespace xpilot
{
    QString ServerEndpoint::normalizeAddress(const QString &address)
    {
        QString normalized = address.trimmed();
        if(normalized.size() >= 2 && normalized.front() == '[' && normalized.back() == ']') {
            normalized = normalized.mid(1, normalized.size() - 2);
        }
        return normalized;
    }

    bool ServerEndpoint::isValidAddress(const QString &address)
    {
        const QString normalized = normalizeAddress(address);
        if(normalized.isEmpty() || normalized.size() > 253) {
            return false;
        }

        QHostAddress ipAddress;
        if(ipAddress.setAddress(normalized)) {
            return true;
        }

        static const QRegularExpression dnsLabel(
            "^[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?$");
        const QStringList labels = normalized.split('.', Qt::KeepEmptyParts);
        for(const QString &label : labels) {
            if(!dnsLabel.match(label).hasMatch()) {
                return false;
            }
        }
        return true;
    }

    bool ServerEndpoint::isValidPort(int port)
    {
        return port >= 1 && port <= 65535;
    }
}
