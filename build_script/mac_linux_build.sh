rootDir=~/code/startalk_pc
cd $rootDir || exit

git pull

rm -rf build_startalk
mkdir build_startalk
cd build_startalk || exit

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -G "CodeBlocks - Unix Makefiles" $rootDir
cmake --build $rootDir/build_startalk --target all -- -j 8