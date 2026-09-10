#include <QtTest>

#include "network/server_endpoint.h"

using namespace xpilot;

class TestServerEndpoint : public QObject
{
    Q_OBJECT

private slots:
    void acceptsPrivateHosts()
    {
        QVERIFY(ServerEndpoint::isValidAddress("100.86.230.111"));
        QVERIFY(ServerEndpoint::isValidAddress("fsd.example.com"));
        QVERIFY(ServerEndpoint::isValidAddress("2001:db8::1"));
    }

    void rejectsUnsafeOrAmbiguousHosts()
    {
        QVERIFY(!ServerEndpoint::isValidAddress(""));
        QVERIFY(!ServerEndpoint::isValidAddress("https://fsd.example.com"));
        QVERIFY(!ServerEndpoint::isValidAddress("fsd.example.com/path"));
        QVERIFY(!ServerEndpoint::isValidAddress("host name"));
    }

    void validatesPortRange()
    {
        QVERIFY(ServerEndpoint::isValidPort(1));
        QVERIFY(ServerEndpoint::isValidPort(6809));
        QVERIFY(ServerEndpoint::isValidPort(65535));
        QVERIFY(!ServerEndpoint::isValidPort(0));
        QVERIFY(!ServerEndpoint::isValidPort(65536));
    }

    void normalizesWhitespaceAndIpv6Brackets()
    {
        QCOMPARE(ServerEndpoint::normalizeAddress("  fsd.example.com  "), QString("fsd.example.com"));
        QCOMPARE(ServerEndpoint::normalizeAddress("[2001:db8::1]"), QString("2001:db8::1"));
    }
};

QTEST_APPLESS_MAIN(TestServerEndpoint)

#include "test_server_endpoint.moc"
