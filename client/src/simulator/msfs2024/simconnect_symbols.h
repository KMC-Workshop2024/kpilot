#pragma once

#include <functional>
#include <vector>

namespace xpilot::msfs2024
{
    using SymbolResolver = std::function<void*(const char*)>;

    const std::vector<const char*>& requiredSimConnectSymbols();
    bool allRequiredSymbolsResolved(const SymbolResolver& resolver);
}
