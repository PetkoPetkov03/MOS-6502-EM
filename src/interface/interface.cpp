#include "./interface.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

void Editor::InitEditor() {
  this->width = (Fl::w() / 3);
  this->height = (Fl::h() / 1.5);

  this->position[0] = Fl::w() / 3;
  this->position[1] = 100;
}

void Editor::Draw() {

  editorSurface = new Fl_Box(position[0], position[1], width, height);
  editorSurface->box(FL_EMBOSSED_BOX);
  editorSurface->labelcolor(FL_BLACK);
  editorSurface->labelfont(FL_BOLD);
  editorSurface->labelsize(20);

  editorSurface->labeltype(FL_SHADOW_LABEL);
}

void Interface::InitWindow() {
  width = Fl::w();
  height = Fl::h();
  window = new Fl_Window(width, height);

  window->fullscreen();

  editor.InitEditor();
}

std::string byteToBinaryString(Byte byte) {
  return std::bitset<sizeof(Byte)>(byte).to_string();
}

void Interface::Draw(CPU &cpu, Mem &memory) {
  if (!registerBox) { // Create the box only once
    //
    registerBox = new Fl_Box(20, 30, width / 10, height / 3);
    registerBox->box(FL_DOWN_BOX);
    registerBox->labelcolor(FL_BLACK);
    registerBox->labelfont(FL_BOLD);
    registerBox->labelsize(20);
    registerBox->labeltype(FL_SHADOW_LABEL);
  }

  std::stringstream registerInfo;
  char flagBuffer[50];
  cpu.SP++;
  Byte stack = memory[0x0100 + cpu.SP];
  cpu.SP--;

  std::cout << "STACK POINTER: " << static_cast<float>(cpu.SP) << std::endl;

  sprintf(flagBuffer, "Z %i N %i O %i \n B %i D %i I %i \n Z %i C %i\n", cpu.Z,
          cpu.N, cpu.O, cpu.B, cpu.D, cpu.I, cpu.Z, cpu.C);
  registerInfo << "ACC: " << static_cast<float>(cpu.ACC) << "\n\n"
               << "RegisterX: " << static_cast<float>(cpu.RegisterX) << "\n\n"
               << "RegisterY: " << static_cast<float>(cpu.RegisterY) << "\n\n"
               << "Stack: " << static_cast<float>(stack >> 7)
               << static_cast<char>('0' + ((stack >> 6) & 1))
               << static_cast<char>('0' + ((stack >> 4) & 1))
               << static_cast<char>('0' + ((stack >> 3) & 1))
               << static_cast<char>('0' + ((stack >> 2) & 1))
               << static_cast<char>('0' + ((stack >> 1) & 1))
               << static_cast<char>('0' + (stack & 1)) 
                << "\n\n"
               << flagBuffer;
  registerText = registerInfo.str();
  registerBox->label(registerText.c_str());

  runButton = new Fl_Button(width / 2, 10, 40, 40);
  runButton->label("Run");
  runButton->callback(RunPressed, this);

  resetButton = new Fl_Button(width / 2 + 50, 10, 40, 40);
  resetButton->label("Reset");
  resetButton->callback(ResetPressed, this);
}

void Interface::EventLoop(CPU &cpu, u32 Cycles, Mem &memory, CycleInfo ci) {
  if (this->window == nullptr) {
    PANIC("Window is uninitialized!");
  }
  editor.Draw();
  this->Draw(cpu, memory);
  window->end();
  std::cout << "Test" << std::endl;

  window->show();
  while (Fl::wait()) {
    if (runPressed == true) {
      cpu.Execute(Cycles, ci, memory);
      runPressed = false;
      this->Draw(cpu, memory);
    }

    if (resetPressed == true) {
      cpu.Reset(memory);
      memory[0x4242] = CPU::INS_LDA_ZP;
      memory[0x4243] = 0x42;
      memory[0x0042] = 0x99;

      memory[0xFFFC] = CPU::INS_JSR;
      memory[0xFFFD] = 0x42;
      memory[0xFFFE] = 0x42;

      memory[0x4244] = CPU::INS_LDA_IM;
      memory[0x4245] = 0x05;

      memory[0x4246] = CPU::INS_LDX_ZP;
      memory[0x4247] = 0x32;
      memory[0x0032] = 0x03;

      memory[0x4248] = CPU::INS_ORA_IM;
      memory[0x4249] = 0x03;

      LoadCycles(memory, ci, 0x00FF);

      resetPressed = false;
      runPressed = false;

      this->Draw(cpu, memory);
    }
  }
}

int Interface::Exit() { return Fl::run(); }

void RunPressed(Fl_Widget *w, void *data) {
  Interface *gui_interface = static_cast<Interface *>(data);
  Fl_Button *btn = (Fl_Button *)w;
  gui_interface->runPressed = true;
  btn->label("running");
}

void ResetPressed(Fl_Widget *w, void *data) {
  Interface *gui_interface = static_cast<Interface *>(data);
  Fl_Button *btn = (Fl_Button *)w;

  gui_interface->resetPressed = true;
}

void Interface::CleanUp() {
  delete this->registerBox;
  delete this->runButton;
  delete this->resetButton;
  delete this->window;
}
