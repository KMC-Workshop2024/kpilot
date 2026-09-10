#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>

#include "common/build_config.h"
#include "fsd/fsd_client.h"

using namespace xpilot;

const ushort BuildConfig::TowerviewClientId() { return 0; }
const ushort BuildConfig::VatsimClientId() { return 0; }
const QString BuildConfig::VatsimClientKey() { return {}; }

class TestPrivateFsd : public QObject
{
    Q_OBJECT

private slots:
    void privateConnectionDoesNotAnswerVatsimChallenge()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        FsdClient client;
        client.SetClientProperties({"K-Pilot", 3, 0, 0, {}});

        bool identified = false;
        connect(&client, &FsdClient::RaiseServerIdentificationReceived,
                this, [&](const PDUServerIdentification&) { identified = true; });

        client.Connect("127.0.0.1", server.serverPort(), false);
        QTRY_VERIFY_WITH_TIMEOUT(server.hasPendingConnections(), 1000);
        QTcpSocket* peer = server.nextPendingConnection();
        QVERIFY(peer != nullptr);

        peer->write("$DISERVER:KMA001:1.0:private-key\r\n");
        peer->flush();
        QTRY_VERIFY_WITH_TIMEOUT(identified, 1000);

        peer->write("$ZCSERVER:KMA001:challenge\r\n");
        peer->flush();
        QTest::qWait(100);
        QCOMPARE(peer->bytesAvailable(), 0);
    }

    void fragmentedVelocityPacketIsParsedOnce()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        FsdClient client;
        int velocityPackets = 0;
        QString callsign;
        connect(&client, &FsdClient::RaiseFastPilotPositionReceived,
                this, [&](const PDUFastPilotPosition& pdu) {
                    ++velocityPackets;
                    callsign = pdu.From;
                });

        client.Connect("127.0.0.1", server.serverPort(), false);
        QTRY_VERIFY_WITH_TIMEOUT(server.hasPendingConnections(), 1000);
        QTcpSocket* peer = server.nextPendingConnection();

        peer->write("^KMA002:-37.1:145.2:10000:9000:12582828:100.0:0.0");
        peer->flush();
        QTest::qWait(50);
        QCOMPARE(velocityPackets, 0);

        peer->write(":0.0:0.0:0.0:0.0:0.0:0.0\r\n");
        peer->flush();
        QTRY_COMPARE_WITH_TIMEOUT(velocityPackets, 1, 1000);
        QCOMPARE(callsign, QString("KMA002"));
    }

    void privateConnectionCanReconnectCleanly()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost));

        FsdClient client;
        client.Connect("127.0.0.1", server.serverPort(), false);
        QTRY_VERIFY_WITH_TIMEOUT(server.hasPendingConnections(), 1000);
        QTcpSocket* firstPeer = server.nextPendingConnection();
        QVERIFY(firstPeer != nullptr);

        firstPeer->write("^INCOMPLETE");
        firstPeer->flush();
        QTest::qWait(20);
        client.Disconnect();
        QTRY_VERIFY_WITH_TIMEOUT(!client.IsConnected(), 1000);

        client.Connect("127.0.0.1", server.serverPort(), false);
        QTRY_VERIFY_WITH_TIMEOUT(server.hasPendingConnections(), 1000);
        QTcpSocket* secondPeer = server.nextPendingConnection();
        QVERIFY(secondPeer != nullptr);
        QTRY_VERIFY_WITH_TIMEOUT(client.IsConnected(), 1000);
    }
};

QTEST_MAIN(TestPrivateFsd)

#include "test_fsd_private.moc"
