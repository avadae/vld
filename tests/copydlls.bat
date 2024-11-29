REM Copying over Visual Leak Detector Dependencies
xcopy "%~dp0..\vld.ini" "%~3" /y /d
echo "%~dp0..\bin\%1\vld_%2.dll"
xcopy "%~dp0..\bin\%1\vld_%2.dll" "%~3\vld_%2.dll" /y /d
xcopy "%~dp0..\bin\%1\vld_%2.pdb" "%~3\vld_%2.pdb" /y /d
xcopy "%~dp0..\setup\dbghelp\%2\dbghelp.dll" "%~3\dbghelp.dll" /y /d
xcopy "%~dp0..\setup\dbghelp\%2\Microsoft.DTfW.DHL.manifest" "%~3\Microsoft.DTfW.DHL.manifest" /y /d