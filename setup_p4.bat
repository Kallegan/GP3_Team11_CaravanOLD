@echo off

rem Login
echo Welcome to the ultra 1337 h4x0r perforce setup program!
echo Running this will help set up your workspace to work smoothly with Unreal binaries (hopefully).
echo Programmers will no longer need to check out binaries to work, just work like you would normally, then run "push_binaries.bat" when pushing new code.
echo Everone else won't have to manually build binaries or worry about it really, they should just be able to "Get Latest" in P4 and run the Editor.
echo.
echo If there are any issues or questions, hit up your local friendly programmer, or find Emil S on discord and yell at him.
echo.
pause

echo.
echo Step 1. Login
echo You should get this info from Allan/IT
echo.
echo (Protip, the stuff in [...] will be defaulted if nothing is entered)
echo.

set /p port="Perforce Server (in the form IP:PORT) [%P4PORT%]: " || set port=%P4PORT%
set /p user="Perforce Username [%P4USER%]: " || set user=%P4USER%
setx P4PORT %port% > nul
setx P4USER %user% > nul
set P4PORT=%port%
set P4USER=%user%

p4 login

if ERRORLEVEL 1 (
	rem Login failed..
	goto error
)

echo.
echo Step 2. Depot
echo This should be in the form 'FG21FT_GP3_Team_XX'
echo (so for example 'FG21FT_GP3_Team_03' or 'FG21FT_GP3_Team_11' etc.)
echo If you're unsure, ask a teammate or Emil S on discord
echo.

set /p depot="Depot Name: "

echo.
echo Step 3. Workspace Name
echo This is up to you, but name it something useful and UNIQUE! Like '%P4USER%_GP3' or '%P4USER%_GP3_Desktop' for example.
echo.
echo WARNING! If you choose a workspace name from a previous project I do NOT guarantee it will work properly.
echo Do that at your own peril!
echo However if you've made a new one already for GP3, or was assigned a new one from Allan then you can use that!
echo.

set /p workspace="Workspace Name [%P4USER%_GP3]: " || set workspace=%P4USER%_GP3
set build_workspace=%workspace%_BUILD

echo.
echo Step 4. Workspace Root
echo This is where your project is gonna be located. Choose wisely, please dont put it on the desktop!
echo NOTE: This path can be relative or absolute! If you already HAVE a workspace for GP3, feel free to use that :)
echo.

set /p workspace_root="Workspace Root [%cd%]: " || set workspace_root=%cd%
set workspace_root=%workspace_root:/=\%
md %workspace_root% > nul 2> nul
pushd %workspace_root%

echo Step 5. Unreal Project Root
echo This will be the folder where your unreal project (the .uproject file) is located
echo.
echo == IMPORTANT ==
echo This path is RELATIVE to your workspace root.
echo For example:
echo If your unreal project is located at
echo 	D:\Prog\Foo\[Workspace Root]\Bar\[MyProject.uproject]
echo The resulting relative path would be
echo 	Bar
echo.
echo If you didn't put your unreal project in a subfolder, just hit enter!
echo If you're at all unsure what to write, ask a teammate or Emil S!
echo.

set /p unreal_root="Unreal Root ['.']: " || set unreal_root=.
if %unreal_root%==. (
	set depot_ue_root=//%depot%
	set workspace_ue_root=//%workspace%
	set build_ue_root=//%build_workspace%
) else (
	set depot_ue_root=//%depot%/%unreal_root%
	set workspace_ue_root=//%workspace%/%unreal_root%
	set build_ue_root=//%build_workspace%/%unreal_root%
)

rem -- SAVE DATA FOR LOADING IN push_binaries.bat
md .p4script > nul 2> nul
attrib +h .p4script

echo %build_workspace%>.p4script/build_workspace
echo %build_ue_root%>.p4script/build_ue_root
echo %unreal_root%>.p4script/unreal_root

rem Init the ignore list
rem General ignores that noone wants! Ugly!
set view_mapping=--field "View=//%depot%/... //%workspace%/..."
set view_mapping=%view_mapping% --field "View+=-//%depot%/.p4script/... //%workspace%/.p4script/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/.vs/...		%workspace_ue_root%/.vs/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/.idea/...	%workspace_ue_root%/.idea/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/*.sln		%workspace_ue_root%/*.sln"
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/Intermediate/... %workspace_ue_root%/Intermediate/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/Build/...	%workspace_ue_root%/Build/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/DerivedDataCache/... %workspace_ue_root%/DerivedDataCache/..."
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/Saved/...	%workspace_ue_root%/Saved/..."

rem Choose what you are
echo.
echo Step 6. Do you want to work directly with C++?
echo (Are you a coder or someone interested in coding)
echo 1. Yes
echo 2. No
echo.
choice /n /c:12 /m "Choose option (1 or 2): "
if %ERRORLEVEL%==1 goto setup_programmer
if %ERRORLEVEL%==2 goto setup_other

rem ---- PROGRAMMERS ----
:setup_programmer
rem Add Binaries to ignore list, you will be using push_binaries.bat instead
set view_mapping=%view_mapping% --field "View+=-%depot_ue_root%/Binaries/... %workspace_ue_root%/Binaries/..."
echo.
echo Setting up workspace...

p4 %view_mapping% --field "Root=%cd%" client -o %workspace% | p4 client -i
if ERRORLEVEL 1 (
	echo.
	echo Uh oh!!! Error when creating workspace. Did you enter depot '%depot%' correctly?
	echo If you did, let Emil S know.
	echo.
	goto error
)

rem Setup build workspace
echo.
echo Setting up build workspace...

p4 --field "Options=allwrite" --field "Root=%cd%" --field "View=%depot_ue_root%/Binaries/... %build_ue_root%/Binaries/..." client -o %build_workspace% | p4 client -i
if ERRORLEVEL 1 (
	echo.
	echo Error when creating build workspace '%build_workspace%', uhhh ping Emil
	echo.
	goto error
)

rem Clear residual read-only flag from all binary files
attrib /s -r %unreal_root%/Binaries/*

echo.
echo == DONE ==
echo Just hit up P4V, select workspace '%workspace%' and have a blast!
echo You can get latest for all content/code, but will have to build the editor manually
echo 	(either through VS/Rider or starting the editor and building that way)

echo IMPORTANT!
echo When you want to push a changelist;
echo 	1. Get latest from perforce
echo 	2. Build using "Development Editor"
echo 	3. Execute "push_binaries.bat" FROM YOUR WORKSPACE ROOT
echo 	4. Don't forget to actually push your code that you've worked on for the other coders :)
echo 		Just do this normally with P4V
echo.

goto done

rem ---- OTHERS ----
:setup_other
echo.
echo Setting up workspace...

p4 %view_mapping% --field "Root=%cd%" client -o %workspace% | p4 client -i
if ERRORLEVEL 1 (
	echo.
	echo Uh oh!!! Error when creating workspace. Did you enter depot '%depot%' correctly?
	echo If you did, let Emil S know.
	echo.
	goto error
)

echo.
echo == DONE ==
echo Just hit up P4V and have a blast. If you're getting errors that "Binaries are out of date" or what have you, make sure to get latest.
echo Otherwise you can just work normally :) However, changing any C++ files will most likely make things break unless you know what you're doing.

goto done

:done
echo Best of luck in your projects!

:error
popd
pause