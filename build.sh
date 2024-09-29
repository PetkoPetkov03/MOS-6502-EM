if [ ! -d ./build ]; then
  mkdir ./build
fi

if [ ! -d ./build/linux/ ]; then
  mkdir ./build/linux/
fi

if [ ! -d ./build/win/ ]; then
  mkdir ./build/win/
fi

cd ./build/linux/
 cmake ../.. -G "Unix Makefiles" 
cmake --build .
cd -

cd ./build/win/
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../toolchain-mingw.cmake -G "Unix Makefiles"
cmake --build .
cd -

cp ./build/win/lib* ./build/win/dist/
cp ./win-dlls/* ./build/win/dist/
