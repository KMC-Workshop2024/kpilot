set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
file(READ "${PROJECT_ROOT}/installer/kpilot.iss" installer)

# X-Plane only discovers Windows plugins at <plugin>/win_x64/*.xpl.
if(NOT installer MATCHES "DestDir: \"\\{code:GetXPlanePluginDir\\}\\\\win_x64\"")
    message(FATAL_ERROR "Windows plugin files are not installed beneath win_x64")
endif()
