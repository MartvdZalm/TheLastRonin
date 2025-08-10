[Setup]
AppName=The Last Ronin
AppVersion=1.0.0
AppPublisher=Mart van der Zalm
AppPublisherURL=https://github.com/MartvdZalm/TheLastRonin
DefaultDirName={autopf}\TheLastRonin
DefaultGroupName=The Last Ronin
OutputDir=.
OutputBaseFilename=TheLastRonin-Windows-Setup
Compression=lzma2/ultra64
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=lowest
WizardStyle=modern
DisableWelcomePage=no
UninstallDisplayIcon={app}\TheLastRonin.exe

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
; Deploy all files from the deploy directory (GitHub Actions will populate this)
Source: "deploy\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\The Last Ronin"; Filename: "{app}\TheLastRonin.exe"
Name: "{commondesktop}\The Last Ronin"; Filename: "{app}\TheLastRonin.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\TheLastRonin.exe"; Description: "{cm:LaunchProgram,The Last Ronin}"; Flags: nowait postinstall skipifsilent