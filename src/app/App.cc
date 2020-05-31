#include "App.hh"

IMPLEMENT_APP(App); //Creates main function

App::App() {}

App::~App() {}

bool App::OnInit(){
  m_main = new MainFrame();
  m_main->Show(true);

  return true;
}
 
