#include "simconnect_symbols.h"

namespace xpilot::msfs2024
{
    const std::vector<const char*>& requiredSimConnectSymbols()
    {
        static const std::vector<const char*> symbols{
            "SimConnect_Open",
            "SimConnect_Close",
            "SimConnect_AddToDataDefinition",
            "SimConnect_RequestDataOnSimObject",
            "SimConnect_SetDataOnSimObject",
            "SimConnect_AICreateNonATCAircraft",
            "SimConnect_AIRemoveObject",
            "SimConnect_CallDispatch",
            "SimConnect_SubscribeToSystemEvent",
            "SimConnect_MapClientEventToSimEvent",
            "SimConnect_TransmitClientEvent",
        };
        return symbols;
    }

    bool allRequiredSymbolsResolved(const SymbolResolver& resolver)
    {
        for(const char* symbol : requiredSimConnectSymbols()) {
            if(resolver(symbol) == nullptr) {
                return false;
            }
        }
        return true;
    }
}
