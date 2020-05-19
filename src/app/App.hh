#pragma once

//#include "wx/wx.h"
#include "MainFrame.hh"


class App : public wxApp {
public:
  App();
  ~App();

  virtual bool OnInit();

private:
  MainFrame* _main = nullptr;

};

