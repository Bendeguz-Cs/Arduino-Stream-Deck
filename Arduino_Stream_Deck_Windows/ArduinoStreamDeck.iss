//PS C:\Qt\6.12.0\mingw_64\bin> .\windeployqt.exe --release --qmldir C:\Users\User\Documents\Arduino\Arduino-Stream-Deck\Arduino_Stream_Deck_Windows C:\Users\User\Documents\Arduino\Arduino-Stream-Deck\Arduino_Stream_Deck_Windows\build\Desktop_Qt_6_12_0_MinGW_64_bit_Release\appStreamDeck.exe

#define MyAppName "Arduino Stream Deck"
#define MyAppVersion "1.2.3"
#define MyAppExeName "appStreamDeck.exe"

[Setup]
AppId={{A7C4D293-86F1-4D70-9B37-2C51E8D0A612}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
DefaultDirName={localappdata}\Programs\ArduinoStreamDeck
DefaultGroupName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir=Output
OutputBaseFilename=ArduinoStreamDeckSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=lowest
ArchitecturesInstallIn64BitMode=x64

[Tasks]
Name: "desktopicon"; \
    Description: "Create a desktop shortcut"; \
    GroupDescription: "Additional shortcuts:"

Name: "autostart"; \
    Description: "Start Arduino Stream Deck when I sign in to Windows"; \
    GroupDescription: "Startup options:"; \
    Flags: unchecked

[Files]
Source: "C:\Users\User\Documents\Arduino\Arduino-Stream-Deck\Arduino_Stream_Deck_Windows\build\Desktop_Qt_6_12_0_MinGW_64_bit_Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\User\Documents\Arduino\Arduino-Stream-Deck\Arduino_Stream_Deck_Windows\Shutdown.bat"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Arduino Stream Deck"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"
Name: "{group}\Shutdown"; Filename: "{app}\Shutdown.bat"; WorkingDir: "{app}"; IconFilename: "{sys}\shell32.dll"; IconIndex: 27
Name: "{autodesktop}\Arduino Stream Deck"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{userstartup}\Arduino Stream Deck.lnk"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Tasks: autostart

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch Arduino Stream Deck"; Flags: postinstall nowait skipifsilent