#include "velocity_predictor.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <deque>

namespace
{
    void expectNear(double actual, double expected, double tolerance, const char* label)
    {
        if(std::abs(actual - expected) > tolerance) {
            std::cerr << label << ": expected " << expected << ", got " << actual << '\n';
            std::exit(EXIT_FAILURE);
        }
    }
}

namespace
{
    struct Packet
    {
        double arrival;
        xpilot::KinematicState reported;
    };

    double replayMaximumError(double rttSeconds)
    {
        using namespace xpilot;
        constexpr double frame = 0.02;
        constexpr double packetInterval = 0.2;
        constexpr double speed = 100.0;
        constexpr double duration = 30.0;
        const Velocity3 velocity{speed, 0.0, 0.0};
        const Velocity3 stopped{0.0, 0.0, 0.0};
        KinematicState predicted{0.0, 0.0, 1000.0, 0.0, 90.0, 0.0};
        Velocity3 correction{0.0, 0.0, 0.0};
        double correctionUntil = 0.0;
        double maximumError = 0.0;
        std::deque<Packet> packets;
        int packetNumber = 0;

        for(double now = 0.0; now <= duration; now += frame) {
            if(std::fmod(now + frame / 2.0, packetInterval) < frame) {
                ++packetNumber;
                if(packetNumber % 10 != 0) {
                    const double jitter = (packetNumber % 5 - 2) * 0.01;
                    const auto reported = VelocityPredictor::extrapolate(
                        KinematicState{0.0, 0.0, 1000.0, 0.0, 90.0, 0.0},
                        velocity, stopped, now);
                    packets.push_back({now + rttSeconds / 2.0 + jitter, reported});
                }
            }

            while(!packets.empty() && packets.front().arrival <= now) {
                correction = VelocityPredictor::positionCorrection(predicted, packets.front().reported, 2.0);
                correctionUntil = now + 2.0;
                packets.pop_front();
            }

            Velocity3 displayVelocity = velocity;
            if(now <= correctionUntil) {
                displayVelocity.x += correction.x;
                displayVelocity.y += correction.y;
                displayVelocity.z += correction.z;
            }
            predicted = VelocityPredictor::extrapolate(predicted, displayVelocity, stopped, frame);

            if(now >= 5.0) {
                const auto truth = VelocityPredictor::extrapolate(
                    KinematicState{0.0, 0.0, 1000.0, 0.0, 90.0, 0.0},
                    velocity, stopped, now + frame);
                const auto error = VelocityPredictor::positionCorrection(predicted, truth, 1.0);
                maximumError = std::max(maximumError, std::abs(error.x));
            }

            if(packets.size() > 2) {
                std::cerr << "replay packet queue grew without bound\n";
                std::exit(EXIT_FAILURE);
            }
        }
        return maximumError;
    }
}

int main()
{
    using namespace xpilot;

    const KinematicState origin{0.0, 0.0, 1000.0, 0.0, 90.0, 0.0};
    const Velocity3 eastAt100MetersPerSecond{100.0, 0.0, 0.0};
    const Velocity3 stopped{0.0, 0.0, 0.0};

    const auto afterOneSecond = VelocityPredictor::extrapolate(
        origin, eastAt100MetersPerSecond, stopped, 1.0);
    expectNear(afterOneSecond.longitude, 0.00089932, 0.000001, "eastward longitude");
    expectNear(afterOneSecond.latitude, 0.0, 0.0000001, "unchanged latitude");

    const auto correction = VelocityPredictor::positionCorrection(
        KinematicState{0.0, 0.001, 1000.0, 0.0, 0.0, 0.0}, origin, 2.0);
    expectNear(correction.x, -55.5975, 0.1, "two-second correction velocity");

    if(!VelocityPredictor::velocityIsStale(501, 500)) {
        std::cerr << "velocity older than 500 ms must be stale\n";
        return EXIT_FAILURE;
    }
    if(VelocityPredictor::velocityIsStale(500, 500)) {
        std::cerr << "velocity at 500 ms must remain usable\n";
        return EXIT_FAILURE;
    }

    expectNear(replayMaximumError(0.06), 0.0, 15.0, "60 ms RTT replay error");
    expectNear(replayMaximumError(0.16), 0.0, 25.0, "160 ms RTT replay error");
    expectNear(replayMaximumError(0.25), 0.0, 35.0, "250 ms RTT replay error");

    return EXIT_SUCCESS;
}
