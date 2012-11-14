# runs cmake
cmake .

# then deletes cmake generated files while leaving the makefile alone
rm -rf CMakeFiles
rm CMakeCache.txt
rm cmake_install.cmake
