#include "msfs2024_backend.h"

namespace xpilot::msfs2024
{
    Msfs2024Backend::Msfs2024Backend(ISimConnectApi& api) : m_api(api) {}

    bool Msfs2024Backend::start()
    {
        if(!m_api.open("K-Pilot")) {
            return false;
        }

        for(const auto& definition : ownAircraftDefinitions()) {
            if(!m_api.addOwnAircraftDefinition(definition)) {
                return false;
            }
        }
        return true;
    }
}
