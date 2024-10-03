#include "./interface.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>
#include <cstdio>
#include <iostream>
#include <sstream>

void Interface::InitWindow() {
  width = Fl::w();
  height = Fl::h();
  window = new Fl_Window(width, height);

  window->fullscreen();
}

void Interface::Draw(CPU &cpu, Mem &memory) {
  if (!registerBox) { // Create the box only once
    registerBox = new Fl_Box(20, 30, width / 10, height / 3);
    registerBox->box(FL_DOWN_BOX);
    registerBox->labelcolor(FL_BLACK);
    registerBox->labelfont(FL_BOLD);
    registerBox->labelsize(20);
    registerBox->labeltype(FL_SHADOW_LABEL);
  }

  std::stringstream registerInfo;
  char flagBuffer[50];
  sprintf(flagBuffer, "Z %i N %i O %i \n B %i D %i I %i \n Z %i C %i\n", cpu.Z,
          cpu.N, cpu.O, cpu.B, cpu.D, cpu.I, cpu.Z, cpu.C);
  registerInfo << "ACC: " << static_cast<float>(cpu.ACC) << "\n\n"
               << "RegisterX: " << static_cast<float>(cpu.RegisterX) << "\n\n"
               << "RegisterY: " << static_cast<float>(cpu.RegisterY) << "\n\n"
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
