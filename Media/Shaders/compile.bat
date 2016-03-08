@echo off

cd %CD%

rem -H - human readable

for %%i in (*.vert) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.tesc) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.tese) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.geom) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.frag) do glslangValidator.exe %%i -V -o %%i.spv
for %%i in (*.comp) do glslangValidator.exe %%i -V -o %%i.spv

pause
