#pragma once

#include "wx/wx.h"
#include "MainFrame.hh"

class App : public wxApp {
private:
  MainFrame* m_main = nullptr;
public:
  App();
  ~App();

  virtual bool OnInit() wxOVERRIDE;
};

