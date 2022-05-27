@echo off
set /p build_workspace=<.p4script/build_workspace
set /p build_ue_root=<.p4script/build_ue_root
set /p unreal_root=<.p4script/unreal_root

set P4CLIENT=%build_workspace%

rem Reconcile
echo = Syncing =
p4 sync -k %build_ue_root%/Binaries/...

echo.
echo = Reconciling =
p4 reconcile -ead %build_ue_root%/Binaries/...

echo.
echo = Reverting patch files =
rem Annoying regex for UE4
rem I'm banking on all teams not naming their project a bunch of numbers
set UE4_REGEX=\-[0-9]*\.

rem Find UE4 patch files
for %%f in (%unreal_root%\Binaries\Win64\*) do (
	echo %%f | findstr %UE4_REGEX% 1> nul
	rem Can't swap this if statement? Batch, hello?
	if errorlevel 1 (
		echo. > nul
	) else (
		echo %%f
		p4 revert %%f
	)
)

rem Find UE5 patch files
for %%f in (%unreal_root%\Binaries\Win64\*.patch_*) do (
	echo %%f
	p4 revert %%f
)

echo.
echo = Submitting =
p4 submit -d "Binary build"

rem In case of errors, revert everything that stuck around...
echo.
echo = Reverting =
p4 revert -k %build_ue_root%/Binaries/...

echo.
echo Done! :)
pause