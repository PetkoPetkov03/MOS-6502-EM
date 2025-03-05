#define NOB_IMPLEMENTATION
#define BUILD_DIR "build/"
#define SRC "src/"
#include "../deps/nob/nob.h"
#include "nob_panic.h"

int main(int argc, char** argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);
  if(!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;
  
  Nob_Cmd cmd = {0};
   
  nob_cmd_append(&cmd, argv[1], "-Wall", "src/MOS.cpp",
                 "src/memory/memory.cpp", "src/cpu/cpu.cpp",
                 "src/cpu/ci.cpp", "src/cpu/opimpl.cpp", "src/interface/interface.cpp"
                 , "-Isrc/types", "deps/fltk/lib/libfltk.a", "deps/fltk/lib/libfltk_gl.a", 
                 "deps/fltk/lib/libfltk_forms.a", "-o", "build/MOS", "-lX11", "-lXft",
                 "-lfontconfig", "-lXrender", "-lXinerama");


  while(!nob_cmd_run_sync(cmd)) return 1;

  return 0;
}
