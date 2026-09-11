set(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")

set(PRIVATE_UI_FILES
    "client/Resources/Views/Settings/SettingsNetwork.qml"
    "client/Resources/Views/ConnectWindow.qml"
    "client/Resources/Views/MainWindow.qml"
    "client/Resources/Components/ConfigRequiredDialog.qml"
    "client/Resources/Components/DisconnectDialog.qml"
    "client/Resources/Components/MicrophoneCalibrationRequired.qml"
    "client/Resources/Views/Settings/SettingsMiscellaneous.qml"
    "client/Resources/Views/Settings/SettingsAudio.qml"
    "client/Resources/Components/DownloadCSLModels/DownloadModels.qml"
    "client/Resources/Components/VersionCheck/DownloadingUpdate.qml"
    "client/Resources/Components/VersionCheck/NewVersionAvailable.qml"
)

foreach(relative_path IN LISTS PRIVATE_UI_FILES)
    file(READ "${PROJECT_ROOT}/${relative_path}" contents)
    string(REPLACE "import org.vatsim.xpilot" "" contents "${contents}")
    if(contents MATCHES "VATSIM|xPilot")
        message(FATAL_ERROR "Legacy public-network branding remains in ${relative_path}")
    endif()
endforeach()

file(READ "${PROJECT_ROOT}/client/src/common/installmodels.cpp" model_installer)
if(model_installer MATCHES "auth\\.vatsim\\.net|VatsimPasswordDecrypted")
    message(FATAL_ERROR "Private credentials can still reach the public CSL authentication service")
endif()

file(READ "${PROJECT_ROOT}/client/Resources/Views/Settings/SettingsNetwork.qml" network_settings)
if(network_settings MATCHES "networkServerList|CachedServers|ServerName")
    message(FATAL_ERROR "Public server selector remains in private network settings")
endif()

foreach(required_label IN ITEMS "Server Address:" "Server Port:" "CID:" "Server Password:")
    string(FIND "${network_settings}" "${required_label}" label_position)
    if(label_position EQUAL -1)
        message(FATAL_ERROR "Missing private-network label: ${required_label}")
    endif()
endforeach()
