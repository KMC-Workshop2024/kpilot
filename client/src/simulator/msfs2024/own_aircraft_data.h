#pragma once

#include <array>
#include <string>

namespace xpilot::msfs2024
{
    struct OwnAircraftData
    {
        double latitudeDeg, longitudeDeg, altitudeFt, altitudeAglFt, pressureAltitudeM;
        double cgToGroundFt, trueHeadingDeg, pitchDeg, bankDeg, groundVelocityKnots;
        double elevationFt, simOnGround;
        double lightStrobe, lightLanding, lightTaxi, lightBeacon, lightNav, lightLogo;
        double transponderCode;
        double com1ActiveMHz, com2ActiveMHz, com1StandbyMHz, com2StandbyMHz;
        double comTransmit1, comTransmit2, comReceiveAll, comTest1, comTest2, comStatus1, comStatus2;
        double flapsHandlePosition, spoilersHandlePosition, gearHandlePosition, numberOfEngines;
        double engine1Combustion, engine2Combustion, engine3Combustion, engine4Combustion;
        double engine1RpmPct, engine2RpmPct, engine3RpmPct, engine4RpmPct;
        double velocityWorldX, velocityWorldY, velocityWorldZ;
        double rotationVelocityBodyX, rotationVelocityBodyY, rotationVelocityBodyZ;
        double altitudeCalibratedFt;
    };

    struct SimVariableDefinition
    {
        std::string name;
        std::string unit;
    };

    const std::array<SimVariableDefinition, 49>& ownAircraftDefinitions();
}
