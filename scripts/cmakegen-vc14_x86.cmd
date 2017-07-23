@echo off

if not exist ..\cmake-build\vc14-x86 mkdir ..\cmake-build\vc14-x86
pushd ..\cmake-build\vc14-x86

cmake ^
	-G"Visual Studio 14 2015" ^
	"-DBUILD_WITH_STATIC_CRT:BOOL=ON" ^
	../..

if errorlevel 1 goto :error

echo [*] Project generation succeeded!

popd
goto :eof

:error
echo [!] ERROR: Failed to generate project - see above and correct.
popd
exit /b 1
