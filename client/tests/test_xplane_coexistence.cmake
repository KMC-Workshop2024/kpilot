set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
set(RUNTIME_FILES
    "client/src/simulator/xplane_adapter.cpp"
    "client/src/config/appconfig.h"
    "client/src/config/appconfig.cpp"
    "plugin/src/xpilot.cpp"
    "plugin/src/plugin.cpp"
    "plugin/src/notification_panel.cpp"
    "plugin/include/config.h"
)

foreach(relative_path IN LISTS RUNTIME_FILES)
    file(READ "${PROJECT_ROOT}/${relative_path}" contents)
    if(contents MATCHES "xpilot\\.ipc|\"xpilot/|DEFAULT_PLUGIN_PORT 53100|m_tcpPort = 53100|XplanePluginPort.*, 53100")
        message(FATAL_ERROR "xPilot runtime identifier collision remains in ${relative_path}")
    endif()
endforeach()

file(READ "${PROJECT_ROOT}/installer/kpilot.iss" installer)
if(NOT installer MATCHES "Resources\\\\plugins\\\\K-Pilot")
    message(FATAL_ERROR "K-Pilot is not installed into an isolated plugin directory")
endif()

if(NOT installer MATCHES "MigrateExistingCslConfig")
    message(FATAL_ERROR "Installer does not migrate existing CSL configuration into isolated storage")
endif()
