cd ~/cpp/KLT/build
cmake .. -DCMAKE_BUILD_TYPE=Debug
#cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
mv main ../main
