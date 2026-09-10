#pragma once

namespace xpilot
{
    struct KinematicState
    {
        double latitude;
        double longitude;
        double altitudeFeet;
        double pitch;
        double heading;
        double bank;
    };

    struct Velocity3
    {
        double x;
        double y;
        double z;
    };

    class VelocityPredictor
    {
    public:
        static KinematicState extrapolate(const KinematicState& state,
                                          const Velocity3& positionalVelocity,
                                          const Velocity3& rotationalVelocity,
                                          double intervalSeconds);
        static Velocity3 positionCorrection(const KinematicState& predicted,
                                            const KinematicState& reported,
                                            double correctionWindowSeconds);
        static bool velocityIsStale(long long ageMilliseconds, long long staleAfterMilliseconds);
    };
}
