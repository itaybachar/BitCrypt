#include "App.hh"
#include <iostream>

IMPLEMENT_APP(App); //Creates main function

App::App() {
}

App::~App() {}

bool App::OnInit(){
  _main = new MainFrame();
  _main->Show(true);

  return true;
}
 
