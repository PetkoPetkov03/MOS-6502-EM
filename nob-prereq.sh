#!/bin/bash
git clone https://github.com/fltk/fltk.git ./deps/fltk
cd deps/fltk
make
cd -
git clone https://github.com/tsoding/nob.h.git ./deps/nob

if [ ! -d ./build ]; then
  mkdir build
fi

