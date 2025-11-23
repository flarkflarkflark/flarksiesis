[Setup]
AppName=Flarksiesis
AppVersion=2.0.0
AppPublisher=flarkAUDIO
AppPublisherURL=https://github.com/yourusername/flarksiesis
AppSupportURL=https://github.com/yourusername/flarksiesis/issues
DefaultDirName={commonpf64}\flarkAUDIO\Flarksiesis
DefaultGroupName=flarkAUDIO
OutputBaseFilename=Flarksiesis-v2.0.0-Windows-Installer
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
LicenseFile=..\LICENSE
InfoBeforeFile=..\README.md
UninstallDisplayIcon={app}\Flarksiesis.exe
SetupIconFile=icon.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation"
Name: "vst3only"; Description: "VST3 Plugin only"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "vst3"; Description: "VST3 Plugin"; Types: full vst3only custom; Flags: fixed
Name: "standalone"; Description: "Standalone Application"; Types: full

[Files]
; VST3
Source: "..\build\Flarksiesis_artefacts\Release\VST3\Flarksiesis.vst3\*"; DestDir: "{commoncf64}\VST3\Flarksiesis.vst3"; Components: vst3; Flags: ignoreversion recursesubdirs createallsubdirs

; Standalone
Source: "..\build\Flarksiesis_artefacts\Release\Standalone\Flarksiesis.exe"; DestDir: "{app}"; Components: standalone; Flags: ignoreversion

; Documentation
Source: "..\README.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Flarksiesis"; Filename: "{app}\Flarksiesis.exe"; Components: standalone
Name: "{group}\Uninstall Flarksiesis"; Filename: "{uninstallexe}"
Name: "{group}\README"; Filename: "{app}\README.md"

[Run]
Filename: "{app}\Flarksiesis.exe"; Description: "Launch Flarksiesis"; Flags: nowait postinstall skipifsilent; Components: standalone

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    MsgBox('Flarksiesis v2.0 has been installed successfully!' + #13#10 + #13#10 +
           'VST3: ' + ExpandConstant('{commoncf64}\VST3\Flarksiesis.vst3') + #13#10 +
           'Standalone: ' + ExpandConstant('{app}\Flarksiesis.exe') + #13#10 + #13#10 +
           'Please restart your DAW to scan for the new plugin.' + #13#10 + #13#10 +
           'by flarkAUDIO',
           mbInformation, MB_OK);
  end;
end;
