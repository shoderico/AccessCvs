set CURRENT_DIR=%~dp0
cd %CURRENT_DIR%
regsvr32 /s /u AccessCvs.dll
reg import uninstall_accesscvs.reg
