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
  wxString m_currentFilePath;
  wxMenuBar* menu;
  wxMenu* file;
  wxMenu* help;
  wxMenu* options;

private:
  bool hidingPass;
  uint8_t m_keyLen;
  BitCrypt* crypt;

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
  void menuBar(wxCommandEvent& event);
  void helpBox(wxCommandEvent& event);
  void passChange(wxCommandEvent& event);
private:
  void _doLayout();
};
