#pragma once

#include "wx/wx.h"
#include "wx/treectrl.h"
#include "wx/dir.h"

class MainFrame : public wxFrame{
private:
  wxButton* browseButton;
  wxTreeCtrl* fileTree;
  wxTextCtrl* passwordBox;
  wxStaticText* fileLocation;
  wxCheckBox* hideCtrl;
  wxButton* checkFileButton;
  wxButton* encFileButton;
  wxButton* decFileButton;
  wxDirDialog* dirDialog;
  wxTreeItemId root;

  DECLARE_EVENT_TABLE();
public:
  MainFrame();
  ~MainFrame();

public:
  void chooseDir(wxCommandEvent& event);
  void loadFile(wxTreeEvent& event);
  void hidePass(wxCommandEvent& event);
  void checkPass(wxCommandEvent& event);
  void encryptEvent(wxCommandEvent& event);
  void decryptEvent(wxCommandEvent& event);
private:
  void _doLayout();
};
