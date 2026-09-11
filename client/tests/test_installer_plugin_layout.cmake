set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
file(READ "${PROJECT_ROOT}/installer/kpilot.iss" installer)

# X-Plane only discovers Windows plugins at <plugin>/win_x64/*.xpl.
if(NOT installer MATCHES "DestDir: \"\\{code:GetXPlanePluginDir\\}\\\\win_x64\"")
    message(FATAL_ERROR "Windows plugin files are not installed beneath win_x64")
endif()

if(NOT installer MATCHES "Resources\\\\plugins\\\\xPilot")
    message(FATAL_ERROR "Installer does not preserve the existing CSL/config data directory")
endif()

if(NOT installer MATCHES "Type: files; Name:.*xPilot\\.xpl")
    message(FATAL_ERROR "Installer does not remove the legacy plugin binary")
endif()
