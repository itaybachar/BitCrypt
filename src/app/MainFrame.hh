#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/dir.h>
#include "BitCrypt.hh"

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
  BitCrypt* crypt;
  uint8_t m_keyLen;
  wxString m_currentFilePath;


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
