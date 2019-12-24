rem set your source path
set rootDir=D:/code/startalk_pc
rem Visual Studio tools
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86

color d

d:
cd %rootDir%

git pull

if exist "%rootDir%\build_startalk" (
	rmdir /s /q "%rootDir%\build_startalk"
)

mkdir build_startalk
cd build_startalk

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DPLATFORM_32BIT=ON -DSTARTALK=ON -G "CodeBlocks - NMake Makefiles" %rootDir%
cmake --build %rootDir%/build_startalk  --target all --

call %rootDir%/build_script/copy_depends.bat %rootDir% %rootDir%/build_startalk/bin 86

pause