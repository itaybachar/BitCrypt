#include "MainFrame.hh"
#include "icon.xpm"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Bit Crypt: File Locker", wxDefaultPosition, wxDefaultSize),
  m_keyLen(AES_128),
  hidingPass(false)
{
  //Icon
  SetIcon(lock_icon);

  wxColour c;
  c.Set("#ffe680");
  //c.Set("#00b3b3");
  SetBackgroundColour(c);

  //MenuBar
  menu = new wxMenuBar;
  file = new wxMenu;
  options = new wxMenu;
  help = new wxMenu;

  file->AppendSubMenu(options,"AES Options");
  file->AppendSeparator();
  file->Append(wxID_EXIT, "Quit");
  
  help->Append(wxID_HELP, "Help");
  
  options->AppendRadioItem(0,"AES_128");
  options->AppendRadioItem(1,"AES_192");
  options->AppendRadioItem(2,"AES_256");

  menu->Append(file,"File");
  menu->Append(help,"Help");
  SetMenuBar(menu);

  //Initialize Componenets
  browseButton = new wxButton(this, 10001,"Browse Folder");
  fileTree = new wxTreeCtrl(this, 10002);
  root = fileTree->AddRoot("Please Select A Folder");
  passwordBox = new wxTextCtrl(this, 10003,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PASSWORD);
  hideCtrl = new wxCheckBox(this, 10004, "Show Password");
  checkFileButton = new wxButton(this, 10005, "Check File");
  encFileButton = new wxButton(this, 10006, "Encrypt File");
  decFileButton = new wxButton(this, 10007, "Decrypt File");
  dirDialog = new wxDirDialog(this, "Select Directory", wxEmptyString, wxDD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, "wxDirDialog");

  checkFileButton->Disable();
  encFileButton->Disable();
  decFileButton->Disable();
  crypt = new BitCrypt(m_keyLen);
  _doLayout();

  SetMinSize(wxSize(600,475));
  //Positions frame in the center of the screen
  Centre();

}

MainFrame::~MainFrame(){

}

void MainFrame::_doLayout(){
  //Create Sizers
  wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL); //Contains everything
  wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

  //File Selection
  leftSizer->Add(browseButton, 0, wxALIGN_CENTER | wxBOTTOM | wxTOP, 10);
  leftSizer->Add(fileTree, 1, wxEXPAND | wxALL, 5);
  mainSizer->Add(leftSizer, 1, wxEXPAND, 0);

  //Current File Display
  wxStaticText* t1 = new wxStaticText(this,wxID_ANY, "Selected File");
  fileLocation = new wxStaticText(this,wxID_ANY, "Choose A File");
  rightSizer->Add(t1, 0, wxALIGN_CENTER | wxTOP, 45);
  rightSizer->Add(fileLocation, 1, wxALIGN_CENTER | wxTOP, 10); //Password Area
  wxStaticText* t3 = new wxStaticText(this,wxID_ANY,"Password");
  rightSizer->Add(t3, 0, wxALIGN_CENTER | wxTOP, 10);

  wxBoxSizer* passwordHolder = new wxBoxSizer(wxHORIZONTAL);
  passwordHolder->Add(passwordBox,0,wxALIGN_CENTER | wxLEFT | wxRIGHT,0);
  rightSizer->Add(passwordHolder, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);

  rightSizer->Add(hideCtrl, 0, wxALIGN_CENTER | wxBOTTOM, 65);

  //Action Buttons
  rightSizer->Add(checkFileButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);
  rightSizer->Add(encFileButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);
  rightSizer->Add(decFileButton, 0, wxALIGN_CENTER | wxBOTTOM, 42);

  //Add right sizer to main
  mainSizer->Add(rightSizer, 1, wxEXPAND, 0);

  //Set Frame sizer
  SetSizer(mainSizer);
  Layout();
};

//Choose Folder Action
void MainFrame::chooseDir(wxCommandEvent& event){

  //Check that Item Selected is not already selected
  wxDir dir;
  dirDialog->SetPath("");
  bool open = false;
  int status = dirDialog->ShowModal();
  if(status == wxID_OK){
    fileTree->DeleteChildren(root);
    fileTree->SetItemText(root,dirDialog->GetPath());
    open = dir.Open(dirDialog->GetPath());
  }

  wxString filename;
  bool cont = false;
  /*
     cont = dir.GetFirst(&filename,wxEmptyString,wxDIR_DIRS);
   */

  if(open){
    cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
    while(cont){
      fileTree->AppendItem(root,filename,1);
      cont = dir.GetNext(&filename);
    }
  }
  dir.Close();
  fileTree->ExpandAll();
  event.Skip();
}

//Tree Selection (double click)
void MainFrame::loadFile(wxTreeEvent& event){
  wxTreeItemId id = event.GetItem();
  if(!fileTree->ItemHasChildren(id) && id != root){
    wxString path = fileTree->GetItemText(id);

    int i =path.Cmp(fileLocation->GetLabel());
    if(path.Cmp(fileLocation->GetLabel())){

      encFileButton->Disable();
      decFileButton->Disable();
      if(id != root){
        wxTreeItemId par = fileTree->GetItemParent(id);
        do{
          path = fileTree->GetItemText(par) + "/" + path;
          par = fileTree->GetItemParent(id);
        }while(par != root);
      }

      m_currentFilePath = path;
      fileLocation->SetLabel(fileTree->GetItemText(id));
      Refresh();
      Layout();
    }
    checkFileButton->Enable();
  }
  event.Skip();
}

//CheckBox Action
void MainFrame::hidePass(wxCommandEvent& event){
  hidingPass = true;
  wxSizer* szr = passwordBox->GetContainingSizer();
  wxString pass = passwordBox->GetValue();
  uint16_t s = (hideCtrl->IsChecked()?0:wxTE_PASSWORD);
  szr->Detach(passwordBox);
  delete passwordBox;
  passwordBox = new wxTextCtrl(this,10003,pass,wxDefaultPosition,wxDefaultSize,s);
  szr->Add(passwordBox, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 0);
  szr->Layout();

  event.Skip();
  hidingPass = false;
}

void MainFrame::checkPass(wxCommandEvent& event){
  int8_t out = crypt->checkFile(m_currentFilePath,passwordBox->GetValue(),passwordBox->GetValue().Len());

  wxMessageDialog dialog(this,wxEmptyString);
  switch(out){
    case -1:
      dialog.SetMessage("File not Encrypted:\nEncryption Enabled");
      dialog.ShowModal();
      encFileButton->Enable();
      break;
    case 0:
      dialog.SetMessage("File Encrypted:\n Wrong Password");
      dialog.ShowModal();
      break;
    case 1:
      dialog.SetMessage("File Encrypted:\n Decryption Enabled");
      dialog.ShowModal();
      decFileButton->Enable();
      break;
    default:
      break;
  }

  event.Skip();
}
void MainFrame::encryptEvent(wxCommandEvent& event){
  wxString pass = passwordBox->GetValue();
  crypt->encryptFile(m_currentFilePath,pass,pass.Len());

  wxMessageDialog dialog(this,"Encryption Successful!");
  dialog.ShowModal();

  encFileButton->Disable();
  event.Skip();
}
void MainFrame::decryptEvent(wxCommandEvent& event){
  wxString pass = passwordBox->GetValue();
  crypt->decryptFile(m_currentFilePath,pass,pass.Len());

  wxMessageDialog dialog(this,"Decryption Successful!");
  dialog.ShowModal();

  decFileButton->Disable();
  event.Skip();
}

void MainFrame::menuBar(wxCommandEvent& event){
  uint32_t id = event.GetId();
  if(id == wxID_EXIT)
    Close(true);
  
  if(id == 0){
    m_keyLen = AES_128;
  }else if(id == 1){
    m_keyLen = AES_192;
  }else if(id == 2){
    m_keyLen = AES_256;
  }
  crypt->setEncryptionType(m_keyLen);
}

void MainFrame::helpBox(wxCommandEvent& event){
  wxMessageBox("1) Choose a File\n2)Check the File(with or without a password)\n3)Proper Actions will be unlocked",
                "BitCrypt Help", wxOK | wxICON_INFORMATION);

}

void MainFrame::passChange(wxCommandEvent& event){
  if(!hidingPass && decFileButton->IsEnabled())
    decFileButton->Disable();
}


BEGIN_EVENT_TABLE(MainFrame,wxFrame)
  //Function Binding
  EVT_BUTTON(10001,MainFrame::chooseDir)
  EVT_TREE_ITEM_ACTIVATED(10002,MainFrame::loadFile)
  EVT_TEXT(10003,MainFrame::passChange)
  EVT_CHECKBOX(10004,MainFrame::hidePass)
  EVT_BUTTON(10005,MainFrame::checkPass)
  EVT_BUTTON(10006,MainFrame::encryptEvent)
  EVT_BUTTON(10007,MainFrame::decryptEvent)
  EVT_MENU(wxID_EXIT,MainFrame::menuBar)
  EVT_MENU(0,MainFrame::menuBar)
  EVT_MENU(1,MainFrame::menuBar)
  EVT_MENU(2,MainFrame::menuBar)
  EVT_MENU(wxID_HELP, MainFrame::helpBox)
END_EVENT_TABLE()

