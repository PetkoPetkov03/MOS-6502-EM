#ifndef INTERFACE
#define INTERFACE

#include "../cpu/cpu.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include "FL/Fl_Window.H"
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <string>

struct Editor {
  Fl_Box* editorSurface = nullptr;
  Vec<int> position;
  int height;
  int width;

  std::string content;

  void InitEditor();

  void Draw();
};

void ResetPressed(Fl_Widget* w, void* data);

void RunPressed(Fl_Widget* w, void* data);

struct Interface {
  int width;
  int height;
  Fl_Window* window = nullptr;

  bool runPressed = false;
  bool resetPressed = false;

  Fl_Box* registerBox = nullptr;
  Fl_Button* runButton = nullptr;

  Fl_Button* resetButton = nullptr;

  Editor editor;

  std::string registerText;

  // Fl fl; 

  void InitWindow();
  
  void Draw(CPU& cpu, Mem& memory);

  void EventLoop(CPU& cpu, u32 Cycles, Mem& memory, CycleInfo ci);

  void CleanUp();

  int Exit();
};

#endif
