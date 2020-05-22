#pragma once

#include "wx/wx.h"
#include "MainFrame.hh"


class App : public wxApp {
public:
  App();
  ~App();

  virtual bool OnInit() wxOVERRIDE;

private:
  MainFrame* _main = nullptr;

};

