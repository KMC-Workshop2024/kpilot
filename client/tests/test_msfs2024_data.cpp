#include <QtTest>

#include "simulator/msfs2024/own_aircraft_data.h"
#include "simulator/msfs2024/msfs2024_backend.h"
#include "simulator/msfs2024/simconnect_symbols.h"

#include <cstddef>
#include <algorithm>

using namespace xpilot::msfs2024;

namespace
{
    class FakeSimConnectApi final : public ISimConnectApi
    {
    public:
        bool openCalled = false;
        std::vector<SimVariableDefinition> registered;

        bool open(const char*) override
        {
            openCalled = true;
            return true;
        }

        bool addOwnAircraftDefinition(const SimVariableDefinition& definition) override
        {
            registered.push_back(definition);
            return true;
        }
    };
}

class TestMsfs2024Data : public QObject
{
    Q_OBJECT

private slots:
    void ownAircraftAbiMatchesSwift()
    {
        QCOMPARE(sizeof(OwnAircraftData), std::size_t(49 * sizeof(double)));
        QCOMPARE(offsetof(OwnAircraftData, velocityWorldX), std::size_t(42 * sizeof(double)));
        QCOMPARE(offsetof(OwnAircraftData, rotationVelocityBodyX), std::size_t(45 * sizeof(double)));
        QCOMPARE(offsetof(OwnAircraftData, altitudeCalibratedFt), std::size_t(48 * sizeof(double)));
    }

    void definitionOrderMatchesBinaryLayout()
    {
        const auto definitions = ownAircraftDefinitions();
        QCOMPARE(definitions.size(), std::size_t(49));
        QCOMPARE(definitions.at(0).name, std::string("PLANE LATITUDE"));
        QCOMPARE(definitions.at(18).name, std::string("TRANSPONDER CODE:1"));
        QCOMPARE(definitions.at(42).name, std::string("VELOCITY WORLD X"));
        QCOMPARE(definitions.at(48).name, std::string("INDICATED ALTITUDE CALIBRATED"));
        QCOMPARE(definitions.at(42).unit, std::string("Feet per second"));
        QCOMPARE(definitions.at(45).unit, std::string("Radians per second"));
    }

    void backendDoesNothingUntilExplicitlyStarted()
    {
        FakeSimConnectApi api;
        Msfs2024Backend backend(api);

        QVERIFY(!api.openCalled);
        QVERIFY(api.registered.empty());
        QVERIFY(backend.start());
        QVERIFY(api.openCalled);
        QCOMPARE(api.registered.size(), std::size_t(49));
    }

    void requiredSimConnectSymbolsRejectIncompleteDll()
    {
        const auto symbols = requiredSimConnectSymbols();
        QVERIFY(symbols.size() >= std::size_t(8));
        QVERIFY(std::find(symbols.begin(), symbols.end(), "SimConnect_Open") != symbols.end());
        QVERIFY(std::find(symbols.begin(), symbols.end(), "SimConnect_AICreateNonATCAircraft") != symbols.end());

        QVERIFY(allRequiredSymbolsResolved([&](const char*) { return reinterpret_cast<void*>(1); }));
        QVERIFY(!allRequiredSymbolsResolved([&](const char* name) {
            return std::string(name) == "SimConnect_Open" ? nullptr : reinterpret_cast<void*>(1);
        }));
    }
};

QTEST_APPLESS_MAIN(TestMsfs2024Data)

#include "test_msfs2024_data.moc"
