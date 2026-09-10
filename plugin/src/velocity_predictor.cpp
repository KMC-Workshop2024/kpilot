#include "velocity_predictor.h"

#include "geo_calc.hpp"

#include <cmath>

namespace
{
    double normalizedDelta(double start, double end, double lowerBound, double upperBound)
    {
        const double range = upperBound - lowerBound;
        if(std::abs(end - start) > range / 2.0) {
            end += end > start ? -range : range;
        }
        return end - start;
    }

    double normalizedCoordinate(double value, double lowerBound, double upperBound)
    {
        const double range = upperBound - lowerBound;
        if(value < lowerBound) return value + range;
        if(value > upperBound) return value - range;
        return value;
    }
}

namespace xpilot
{
    KinematicState VelocityPredictor::extrapolate(const KinematicState& state,
                                                   const Velocity3& positionalVelocity,
                                                   const Velocity3&,
                                                   double intervalSeconds)
    {
        KinematicState result = state;
        result.latitude = normalizedCoordinate(
            state.latitude + MetersToDegrees(positionalVelocity.z * intervalSeconds), -90.0, 90.0);
        result.longitude = normalizedCoordinate(
            state.longitude + MetersToDegrees(positionalVelocity.x * intervalSeconds /
                                               LongitudeScalingFactor(state.latitude)),
            -180.0, 180.0);
        result.altitudeFeet += positionalVelocity.y * intervalSeconds * 3.28084;
        return result;
    }

    Velocity3 VelocityPredictor::positionCorrection(const KinematicState& predicted,
                                                     const KinematicState& reported,
                                                     double correctionWindowSeconds)
    {
        const double latitudeDelta = DegreesToMeters(normalizedDelta(
            predicted.latitude, reported.latitude, -90.0, 90.0));
        double longitudeDelta = DegreesToMeters(normalizedDelta(
            predicted.longitude, reported.longitude, -180.0, 180.0));
        longitudeDelta *= LongitudeScalingFactor(reported.latitude);
        const double altitudeDelta = (reported.altitudeFeet - predicted.altitudeFeet) * 0.3048;

        return {longitudeDelta / correctionWindowSeconds,
                altitudeDelta / correctionWindowSeconds,
                latitudeDelta / correctionWindowSeconds};
    }

    bool VelocityPredictor::velocityIsStale(long long ageMilliseconds, long long staleAfterMilliseconds)
    {
        return ageMilliseconds > staleAfterMilliseconds;
    }
}
