#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <charconv>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>
#include "info.h"

using namespace std;
const unsigned int width = GetSystemMetrics(SM_CXSCREEN);
const unsigned int height = GetSystemMetrics(SM_CYSCREEN);

// GUI Classes
class LoginDialog : public wxDialog {
private:
    wxTextCtrl* passwordCtrl;
    wxTextCtrl* checkPasswordCtrl;
    string* resultPassword;
    wxWindow* Parent;

    void setAccel(wxDialog* parent) {
        wxAcceleratorEntry entries[2];
        entries[0].Set(wxACCEL_NORMAL, '\n', wxID_OK);
        entries[1].Set(wxACCEL_ALT, 'x', wxID_CANCEL);
        wxAcceleratorTable accel(2, entries);
        parent->SetAcceleratorTable(accel);
    }


public:
    LoginDialog(wxWindow* parent, string* password)
        : wxDialog(parent, wxID_ANY, "Password Manager Login", wxPoint((width/2)-150,(height/2)-150), wxSize(300, 150)),
          resultPassword(password),Parent(parent){
        
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);

        inputSizer->Add(new wxStaticText(this, wxID_ANY, "Password: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        passwordCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
        inputSizer->Add(passwordCtrl, 1, wxALL | wxEXPAND, 5);

        mainSizer->Add(inputSizer, 0, wxALL | wxEXPAND, 5);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(new wxButton(this, wxID_OK, "Login"), 0, wxALL, 5);
        buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

        setAccel(this);
        SetSizer(mainSizer);

        Bind(wxEVT_BUTTON, &LoginDialog::OnLogin, this, wxID_OK);
    }

    LoginDialog(wxWindow* parent, string* password, string checkPasswoed)
        : wxDialog(parent, wxID_ANY, "Password Manager Login", wxPoint((width / 2) - 300, (height / 2) - 150), wxSize(600, 300)),
        resultPassword(password), Parent(parent) {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* inputChecker = new wxBoxSizer(wxHORIZONTAL);

        mainSizer->Add(new wxStaticText(this, wxID_ANY, "Welcome in Password manager\nFor the first usage, please insert your master password and keep it in mind."), 0, wxALL | wxALIGN_LEFT, 5);

        inputSizer->Add(new wxStaticText(this, wxID_ANY, "Password: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        passwordCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
        inputSizer->Add(passwordCtrl, 1, wxALL | wxEXPAND, 5);

        inputChecker->Add(new wxStaticText(this, wxID_ANY, "Verify your password: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        checkPasswordCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
        inputChecker->Add(checkPasswordCtrl, 1, wxALL | wxEXPAND, 5);

        mainSizer->Add(inputSizer, 0, wxALL | wxEXPAND, 5);
        mainSizer->Add(inputChecker, 0, wxALL | wxEXPAND, 5);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(new wxButton(this, wxID_OK, "Login"), 0, wxALL, 5);
        buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

        setAccel(this);
        SetSizer(mainSizer);

        Bind(wxEVT_BUTTON, &LoginDialog::OnFirstLogin, this, wxID_OK);

        }

    void OnLogin(wxCommandEvent& event){
        string password = passwordCtrl->GetValue().ToStdString();

        ifstream test("test.dat");
        if (test.good()) {
            fstream file;
            file.open("test.dat");
            string key;
            file >> key;
            file.close();
            if (password.length() < 12) {
                wxMessageBox("Wrong password!", "Error", wxICON_ERROR);
            }
            else {
                if (key == hashing(obfuscation(password))) {
                    *resultPassword = password;
                    this->Parent->Show();
                    EndModal(wxID_OK);
                }
                else {
                    wxMessageBox("Wrong password!", "Error", wxICON_ERROR);
                }
            }
        } 
        
    }
    void OnFirstLogin(wxCommandEvent& event) {
        string password = passwordCtrl->GetValue().ToStdString();
        string checkPassword = checkPasswordCtrl->GetValue().ToStdString();
        if (password != checkPassword) {
            wxMessageBox("The two password should be identic\nPlease try again", "Error", wxICON_ERROR);
            return;
        }

        ifstream test("test.dat");
        if (test.good()) return;
        if (password.length() >= 12) {
            string key = hashing(obfuscation(password));
            ofstream file("test.dat", ios::binary);
            file << key;
            file.close();
            *resultPassword = password;
            this->Parent->Show();
            EndModal(wxID_OK);
        }
        else {
            wxMessageBox("Password must be at least 12 characters long!", "Error", wxICON_ERROR);
        }
    }
};

class AddEntryDialog : public wxDialog {
private:
    wxTextCtrl* entityCtrl;
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;
    wxTextCtrl* emailCtrl;
    wxSlider* passwordSize = new wxSlider(this, wxID_ANY, 12, 12, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);

public:
    AddEntryDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, "Add New Entry", wxDefaultPosition, wxSize(400, 300)) {
        
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
      
        auto AddField = [&](const wxString& label) {
            wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
            row->Add(new wxStaticText(this, wxID_ANY, label), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
            wxTextCtrl* ctrl = new wxTextCtrl(this, wxID_ANY);
            row->Add(ctrl, 1, wxALL | wxEXPAND, 5);
            mainSizer->Add(row, 0, wxEXPAND | wxALL, 5);
            return ctrl;
        };

        entityCtrl = AddField("Entity: ");
        usernameCtrl = AddField("Username: ");
        passwordCtrl = AddField("Password: ");
        emailCtrl = AddField("Email: ");
        passwordSize->SetToolTip("Password size");
       

        wxButton* generateBtn = new wxButton(this, wxID_ANY, "Generate Password");
        wxBoxSizer* passwordParam = new wxBoxSizer(wxHORIZONTAL);
        passwordParam->Add(generateBtn,  0, wxALL , 5);
        passwordParam->Add(passwordSize, 0, wxALL , 5);
        mainSizer->Add(passwordParam, 0, wxALIGN_CENTER | wxALL, 5);
        

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(new wxButton(this, wxID_OK, "Add"), 0, wxALL, 5);
        buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 5);

        SetSizer(mainSizer);

        generateBtn->Bind(wxEVT_BUTTON, &AddEntryDialog::OnGeneratePassword, this);
    }

    void OnGeneratePassword(wxCommandEvent& event) {
        passwordCtrl->SetValue(generatePassword(passwordSize->GetValue()));
    }

    Informations GetEntryData() {
        return Informations(
            entityCtrl->GetValue().ToStdString(),
            passwordCtrl->GetValue().ToStdString(),
            emailCtrl->GetValue().ToStdString(),
            usernameCtrl->GetValue().ToStdString(),
            0
        );
    }
};

class MainFrame : public wxFrame {
private:
    wxGrid* grid;
    string masterPassword;
    class Data data;

public:
    MainFrame() : wxFrame(nullptr, wxID_ANY, "Password Manager", wxPoint((width-800) / 2, (height-600)/2), wxSize(800, 600)){
        wxMenuBar* menuBar = new wxMenuBar;
        wxMenu* fileMenu = new wxMenu;
        fileMenu->Append(wxID_NEW, "&New Entry\tCtrl-N");
        fileMenu->Append(wxID_DELETE, "&Delete Entry\tDel");
        fileMenu->AppendSeparator();
        fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X");
        fileMenu->Append(wxID_PAGE_SETUP, "&Parameters");
        menuBar->Append(fileMenu, "&File");
        SetMenuBar(menuBar);

        wxAcceleratorEntry entries[1];
        entries[0].Set(wxACCEL_CTRL, 'c', wxID_COPY);
        wxAcceleratorTable accel(1, entries);
        SetAcceleratorTable(accel);


        wxToolBar* toolBar = CreateToolBar();
        toolBar->AddTool(wxID_NEW, "New Entry", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR));
        toolBar->AddTool(wxID_DELETE, "Delete Entry", wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR));
        toolBar->Realize();

        grid = new wxGrid(this, wxID_ANY);
        grid->CreateGrid(0, 4);
        grid->SetColLabelValue(0, "Entity");
        grid->SetColLabelValue(1, "Username");
        grid->SetColLabelValue(2, "Password");
        grid->SetColLabelValue(3, "Email");

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(grid, 1, wxEXPAND);
        SetSizer(sizer);

        Bind(wxEVT_MENU, &MainFrame::OnNewEntry, this, wxID_NEW);
        Bind(wxEVT_MENU, &MainFrame::OnDeleteEntry, this, wxID_DELETE);
        Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
        Bind(wxEVT_BUTTON, &MainFrame::OnCopy, this, wxID_COPY);

        if (!ShowLoginDialog()) {
            Close();
            return;
        }

        RefreshGrid();
    }
    
private:
    bool ShowLoginDialog() {
        ifstream test("test.dat");
        if (test.good()) {
        LoginDialog dialog(this, &masterPassword);
        return dialog.ShowModal() == wxID_OK;
        }
        LoginDialog dialog(this, &masterPassword,"First connection");
        return dialog.ShowModal() == wxID_OK;
    }

    void RefreshGrid() {
        grid->ClearGrid();
        if (grid->GetNumberRows() > 0)
            grid->DeleteRows(0, grid->GetNumberRows());

        int row = 0;
        data.infos = getInformations(masterPassword);
        for (const auto& info : data.infos) {
            grid->AppendRows();
            grid->SetCellValue(row, 0, info.getEntity());
            grid->SetCellValue(row, 1, info.getUsername());
            grid->SetCellValue(row, 2, info.getPassword());
            grid->SetCellValue(row, 3, info.getEmail());
            row++;
        }

        for (int col = 0; col < grid->GetNumberCols(); col++) {
            grid->AutoSizeColumn(col);
        }
    }

    void OnNewEntry(wxCommandEvent& event){
        AddEntryDialog dialog(this);
        if (dialog.ShowModal() == wxID_OK) {
            Informations info = dialog.GetEntryData();
            data.addInformations(masterPassword, info);
            RefreshGrid();
        }
    }

    void OnDeleteEntry(wxCommandEvent& event) {
        int row = grid->GetGridCursorRow();
        if (row >= 0 && row < grid->GetNumberRows()) {
            if (wxMessageBox("Are you sure you want to delete this entry?", 
                           "Confirm Delete", 
                           wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) {
                data.removeInformations(masterPassword, row);
                RefreshGrid();
            }
        }
    }
    
    void OnCopy(wxCommandEvent& event) {
        int col = grid->GetGridCursorCol();
        int row = grid->GetGridCursorCol();
        auto info = grid->GetCellValue(row,col);

        if (wxTheClipboard->Open())
        {
            wxTheClipboard->SetData(new wxTextDataObject(info));
            wxTheClipboard->Close();
        }
    }

    void OnExit(wxCommandEvent& event) {
        Close();
    }
};

class PasswordManagerApp : public wxApp {
public:
    bool OnInit(){
        MainFrame* frame = new MainFrame();
        return true;
    }
};

wxIMPLEMENT_APP(PasswordManagerApp);
