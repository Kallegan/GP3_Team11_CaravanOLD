@echo off
set /p unreal_root=<.p4script/unreal_root

rem Annoying regex for UE4
rem I'm banking on all teams not naming their project a bunch of numbers
set UE4_REGEX=\-[0-9]*\.

rem Find UE4 patch files
for %%f in (%unreal_root%\Binaries\Win64\*) do (
	echo %%f | findstr %UE4_REGEX% 1> nul
	if errorlevel 1 (
		echo. > nul
	) else (
		echo %%f
		del %%f > nul
	)
)

rem Find UE5 patch files
for %%f in (%unreal_root%\Binaries\Win64\*.patch_*) do (
	echo %%f
	del %%f > nul
)

echo Done!
pause