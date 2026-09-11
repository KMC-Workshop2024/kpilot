#ifndef KPilotClientDir
  #error KPilotClientDir must point to the deployed K-Pilot client directory.
#endif

#ifndef XPlanePluginDir
  #error XPlanePluginDir must point to the built X-Plane plugin directory.
#endif

[Setup]
AppId={{7F7D67AE-8348-4C1B-A020-F094BB79CD72}
AppName=K-Pilot
AppVersion=3.0.0
AppPublisher=KMC Workshop
DefaultDirName={autopf}\K-Pilot
DefaultGroupName=K-Pilot
DisableProgramGroupPage=yes
OutputBaseFilename=K-Pilot-Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=lowest
SetupIconFile=..\client\Resources\Icons\KPilot.ico
UninstallDisplayIcon={app}\K-Pilot.exe
CloseApplications=yes
RestartApplications=no

[Components]
Name: "client"; Description: "K-Pilot desktop client"; Types: full compact custom; Flags: fixed
Name: "xplane"; Description: "X-Plane 11/12 plugin"; Types: full
Name: "msfs2024"; Description: "Microsoft Flight Simulator 2024 support"; Types: full

[Files]
Source: "{#KPilotClientDir}\*"; DestDir: "{app}"; Components: client; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#XPlanePluginDir}\*"; DestDir: "{code:GetXPlanePluginDir}"; Components: xplane; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\client\Resources\Sounds\*"; DestDir: "{localappdata}\org.kpilot.client\Sounds"; Components: client; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\K-Pilot"; Filename: "{app}\K-Pilot.exe"
Name: "{autodesktop}\K-Pilot"; Filename: "{app}\K-Pilot.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Shortcuts:"; Flags: unchecked

[Registry]
Root: HKCU; Subkey: "Software\KMC Workshop\K-Pilot"; ValueType: string; ValueName: "MSFS2024Enabled"; ValueData: "1"; Components: msfs2024; Flags: uninsdeletevalue

[Code]
var
  XPlanePage: TInputDirWizardPage;

procedure InitializeWizard;
begin
  XPlanePage := CreateInputDirPage(wpSelectComponents,
    'Choose X-Plane Directory',
    'Where is X-Plane installed?',
    'Select the X-Plane root directory. Clear the X-Plane component on the previous page to skip plugin installation.',
    False, '');
  XPlanePage.Add('');
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := (PageID = XPlanePage.ID) and not WizardIsComponentSelected('xplane');
end;

function NextButtonClick(CurPageID: Integer): Boolean;
begin
  Result := True;
  if (CurPageID = XPlanePage.ID) and WizardIsComponentSelected('xplane') then
  begin
    if (Trim(XPlanePage.Values[0]) = '') or
       not DirExists(AddBackslash(XPlanePage.Values[0]) + 'Resources') then
    begin
      MsgBox('Select a valid X-Plane directory, or go back and clear the X-Plane component.', mbError, MB_OK);
      Result := False;
    end;
  end;
end;

function GetXPlanePluginDir(Param: String): String;
begin
  Result := AddBackslash(XPlanePage.Values[0]) + 'Resources\plugins\xPilot';
end;
