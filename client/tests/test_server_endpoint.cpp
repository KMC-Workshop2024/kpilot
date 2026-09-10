#include <QtTest>

#include "network/server_endpoint.h"
#include "network/network_log_sanitizer.h"
#include "fsd/pdu/pdu_add_pilot.h"

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

    void preservesLeadingZeroCidAndFastProtocolRevision()
    {
        const PDUAddPilot login("KMA001", "0921", "secret", NetworkRating::OBS,
                                ProtocolRevision::Vatsim2022, SimulatorType::XPlane,
                                "Test Pilot");
        const QStringList fields = login.toTokens();

        QCOMPARE(fields.at(2), QString("0921"));
        QCOMPARE(fields.at(5), QString("101"));
    }

    void redactsPilotAndAtcLoginPasswords()
    {
        QCOMPARE(NetworkLogSanitizer::sanitize(
                     "#APKMA001:SERVER:0921:secret:1:101:11:Test Pilot\r\n"),
                 QString("#APKMA001:SERVER:0921:<redacted>:1:101:11:Test Pilot\r\n"));
        QCOMPARE(NetworkLogSanitizer::sanitize(
                     "#AAKMA_TWR:SERVER:Real Name:0921:secret:1:4:40.0:145.0:0\r\n"),
                 QString("#AAKMA_TWR:SERVER:Real Name:0921:<redacted>:1:4:40.0:145.0:0\r\n"));
        QCOMPARE(NetworkLogSanitizer::sanitize("^KMA001:1:2:3\r\n"),
                 QString("^KMA001:1:2:3\r\n"));
    }
};

QTEST_APPLESS_MAIN(TestServerEndpoint)

#include "test_server_endpoint.moc"
