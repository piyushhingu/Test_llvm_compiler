This is an simple Kaleidoscope language compiler using LLVM as back-end

How to run 
mkdir build
cd build

If you are using an Unix system use 
cmake -G "Unix Makefiles" ..
make 
or
utilize all core in system 
make -j$(nproc)
If you are using an ninja than use ninja un Unix Makefiles 
else 

cmake ..
make 

now you have an parser in build file and can run than with sample input in inputs file 
./parser ../inputs/input1.txt


