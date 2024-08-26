if [ ! -d "./dist" ]
then mkdir "./dist"
fi

clang++ -o "./dist/MOS" ./src/MOS.cpp
