#pragma once

#include "own_aircraft_data.h"

namespace xpilot::msfs2024
{
    class ISimConnectApi
    {
    public:
        virtual ~ISimConnectApi() = default;
        virtual bool open(const char* clientName) = 0;
        virtual bool addOwnAircraftDefinition(const SimVariableDefinition& definition) = 0;
    };

    class Msfs2024Backend
    {
    public:
        explicit Msfs2024Backend(ISimConnectApi& api);
        bool start();

    private:
        ISimConnectApi& m_api;
    };
}
