#include "network_log_sanitizer.h"

#include <QStringList>

namespace xpilot
{
    QString NetworkLogSanitizer::sanitize(const QString& data)
    {
        QStringList packets = data.split("\r\n", Qt::KeepEmptyParts);
        for(QString& packet : packets) {
            QStringList fields = packet.split(':', Qt::KeepEmptyParts);
            int passwordField = -1;
            if(packet.startsWith("#AP", Qt::CaseInsensitive)) {
                passwordField = 3;
            } else if(packet.startsWith("#AA", Qt::CaseInsensitive)) {
                passwordField = 4;
            }

            if(passwordField >= 0 && fields.size() > passwordField) {
                fields[passwordField] = "<redacted>";
                packet = fields.join(':');
            }
        }
        return packets.join("\r\n");
    }
}
