/*
** Copyright (C) 2025 Vitaliy Tarasenko.
**
** This file is part of FishCode (fishcode).
**
** FishCode is free software: you can redistribute it and/or modify it under
** the terms of the GNU General Public License as published by the Free
** Software Foundation, either version 3 of the License, or (at your option)
** any later version.
**
** FishCode is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
** more details.
**
** You should have received a copy of the GNU General Public License along
** with FishCode. If not, see <https://www.gnu.org/licenses/>.
**
** This program uses wxWidgets, a free and open-source cross-platform C++
** library for creating GUIs. wxWidgets is licensed under the wxWindows
** Library License, which is compatible with the GNU GPL.
** See <https://www.wxwidgets.org/about/licence/>.
*/

#include <exception>
#include <memory>
#include <thread>
#include <utility>
#include <wx/aboutdlg.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include "error.hpp"
#include "events.hpp"
#include "frame.hpp"
#include "progress.hpp"
#include "strings.hpp"
#include "task.hpp"

fc::Frame::Frame()
: wxFrame(nullptr, fc::events::ID_FRAME, STR_NAME0) {
    // Create and configure new menu bar for the frame.
    auto menuBar = new wxMenuBar();
    auto menuMore = new wxMenu();
    menuMore->Append(wxID_ABOUT, STR_NAME2, STR_PROMPT0);
    menuMore->Append(wxID_HELP, STR_NAME3, STR_PROMPT1);
    menuBar->Append(menuMore, STR_NAME1);

    // Connect menu bar to the frame.
    SetMenuBar(menuBar);

    // Configure menu bar event handlers.
    Bind(wxEVT_MENU, &fc::Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &fc::Frame::OnHelp, this, wxID_HELP);

    // Create and set new status bar for the frame.
    SetStatusBar(CreateStatusBar());

    // Set the default status text.
    SetStatusText(STR_STATUS0);

    // Initialize new sizer for the main window.
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    // Configure grid sizers layout.
    const wxSize gridLayout(5, 5);

    // Configure sizers (using flags).
    const auto boxSizerFlags = wxSizerFlags().Expand().Border(wxALL, 5);
    const auto gridSizerFlags = wxSizerFlags().Expand().Border(wxALL, 10);
    const auto gridLabelSizerFlags = wxSizerFlags().Center().Right().Border(wxALL, 2);

    // Set layout for the fields.
    const wxSize fieldSize(400, 45);

    // Configure UI sizers.
    wxFlexGridSizer* gridSizers[] = {
        new wxFlexGridSizer(1, 3, gridLayout),
        new wxFlexGridSizer(1, 3, gridLayout),
        new wxFlexGridSizer(1, 2, gridLayout)
    };
    gridSizers[0]->AddGrowableCol(1);
    gridSizers[0]->AddGrowableCol(2);
    gridSizers[1]->AddGrowableCol(1);
    gridSizers[1]->AddGrowableCol(2);
    gridSizers[2]->AddGrowableCol(1);
    auto boxSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create and configure input fields.
    fields[0] = new Field(this);
    fields[1] = new Field(this);
    fields[2] = new PasswordField(this);
    fields[0]->SetMinSize(fieldSize);
    fields[1]->SetMinSize(fieldSize);
    fields[2]->SetMaxLength(Password::SIZE);
    fields[2]->SetMinSize(fieldSize);
    labels[0] = new Label(this, STR_LABEL0);
    labels[1] = new Label(this, STR_LABEL1);
    labels[2] = new Label(this, STR_LABEL4);
    gridSizers[0]->Add(labels[0], gridLabelSizerFlags);
    gridSizers[0]->Add(fields[0], gridSizerFlags);
    gridSizers[1]->Add(labels[1], gridLabelSizerFlags);
    gridSizers[1]->Add(fields[1], gridSizerFlags);
    gridSizers[2]->Add(labels[2], gridLabelSizerFlags);
    gridSizers[2]->Add(fields[2], gridSizerFlags);

    // Create and configure buttons.
    buttons[0] = new Button(this, events::ID_CHOOSE, STR_LABEL2);
    buttons[1] = new Button(this, events::ID_SET, STR_LABEL3);
    buttons[2] = new Button(this, events::ID_ENCRYPT, STR_LABEL5);
    buttons[3] = new Button(this, events::ID_DECRYPT, STR_LABEL6);
    buttons[4] = new Button(this, events::ID_CANCEL, STR_LABEL7);
    gridSizers[0]->Add(buttons[0], gridSizerFlags);
    gridSizers[1]->Add(buttons[1], gridSizerFlags);
    boxSizer->Add(buttons[2], boxSizerFlags);
    boxSizer->Add(buttons[3], boxSizerFlags);

    // Disable "Cancel" button for now.
    buttons[4]->Disable();

    // Add UI sizers to the frame (using sizer).
    mainSizer->Add(gridSizers[0], boxSizerFlags);
    mainSizer->Add(gridSizers[1], boxSizerFlags);
    mainSizer->Add(gridSizers[2], boxSizerFlags);
    mainSizer->Add(boxSizer, boxSizerFlags);

    // Set up button event handlers.
    buttons[0]->Bind(wxEVT_BUTTON, &fc::Frame::OnChoose, this, events::ID_CHOOSE);
    buttons[1]->Bind(wxEVT_BUTTON, &fc::Frame::OnSet, this, events::ID_SET);
    buttons[2]->Bind(wxEVT_BUTTON, &fc::Frame::OnEncrypt, this, events::ID_ENCRYPT);
    buttons[3]->Bind(wxEVT_BUTTON, &fc::Frame::OnDecrypt, this, events::ID_DECRYPT);
    buttons[4]->Bind(wxEVT_BUTTON, &fc::Frame::OnCancel, this, events::ID_CANCEL);

    // Create a progress bar.
    progressBar = new ProgressBar(this);

    // Disable progress bar for now.
    progressBar->Disable();

    // Add progress bar to the frame.
    mainSizer->Add(progressBar, boxSizerFlags);

    // Add "Cancel" button to the frame (bottom).
    mainSizer->Add(buttons[4], boxSizerFlags);

    // Connect main window (frame) with its sizer.
    SetSizerAndFit(mainSizer);

    // Create and configure timer.
    readyTimer = std::make_unique<wxTimer>(this, events::ID_READY);

    // Configure timer event handler.
    Bind(wxEVT_TIMER, &fc::Frame::OnReadyTimer, this, events::ID_READY);

    // Set up status update handlers.
    Bind(events::EVT_UPDATE_DONE, &fc::Frame::OnDoneUpdate, this, events::ID_DONE);
    Bind(events::EVT_UPDATE_PROGRESS, &fc::Frame::OnProgressUpdate, this, events::ID_PROGRESS);

    // Configure frame closing handler.
    Bind(wxEVT_CLOSE_WINDOW, &fc::Frame::OnClose, this, events::ID_CLOSE);
}

void fc::Frame::OnAbout(wxCommandEvent& event) {
    // Configure the about dialog.
    wxAboutDialogInfo aboutDialogInfo;
    aboutDialogInfo.SetName(STR_NAME0);
    aboutDialogInfo.SetVersion(STR_VERSION);
    aboutDialogInfo.SetDescription(STR_DESCRYPTION);
    aboutDialogInfo.SetCopyright(STR_COPYRIGHT);
    aboutDialogInfo.AddDeveloper(STR_DEVELOPER);

    // Show about box.
    wxAboutBox(aboutDialogInfo, this);
}

void fc::Frame::OnCancel(wxCommandEvent& event) {
    // Disable "Cancel" button.
    DisableCancelButton();

    // Enable task abortion.
    taskShouldCancel = true;

    // Wait for the task termination (if there is a task).
    if (taskThread->joinable()) {
        taskThread->join();
    }

    // Disable task abortion.
    taskShouldCancel = false;

    // Set new status in the status bar.
    SetStatusText(STR_STATUS2);

    // Start timer to the new status.
    readyTimer->StartOnce(3000);
}

void fc::Frame::OnChoose(wxCommandEvent& event) {
    // Open file selector.
    const auto filePath = wxFileSelector(
        STR_CAPTION1,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        wxFileSelectorDefaultWildcardStr,
        wxFD_OPEN,
        this
    );

    // Check if user has chosen the file.
    if (!filePath.empty()) {
        // Insert file path to the input field.
        SetIFPathValue(filePath);
    }
}

void fc::Frame::OnClose(wxCloseEvent& event) {
    // Abort the task (if it is running).
    taskShouldCancel = true;

    // Wait for the task termination.
    if (taskThread->joinable()) {
        taskThread->join();
    }

    // Destroy window (frame) and all of its subwindows.
    Destroy();
}

void fc::Frame::OnDecrypt(wxCommandEvent& event) try {
    // Set frame to the "processing" mode.
    DisableButtons();
    DisableFields();
    EnableCancelButton();
    EnableProgressBar();

    // Display new status in the status bar.
    SetStatusText(STR_STATUS4);

    // Get pathes to input and output files.
    const auto ifPath = GetIFPathValue();
    const auto ofPath = GetOFPathValue();

    // Get user password (as string).
    const auto passwordString = GetPasswordValue();

    // Check user data.
    CheckFileIO(ifPath.utf8_string(), ofPath.utf8_string());
    auto inputFile = CheckInputFile(ifPath.utf8_string(), true);
    auto outputFile = CheckOutputFile(ofPath.utf8_string());
    const auto password = CheckPassword(passwordString.utf8_string());

    // Allocate memory for the task.
    std::unique_ptr<Task> task;

    // Open the input file.
    task->SetInputFile(std::move(inputFile));

    // Create an output file.
    task->SetOutputFile(std::move(outputFile));

    // Store user password.
    task->SetPassword(password);

    // Create new thread for the encryption task.
    taskThread = std::make_unique<std::thread>([this, &task]() {
        TaskDecrypt(this, std::move(task));
    });

    // Allow task to run independently.
    taskThread->detach();
} catch (const std::exception& ex) {
    // Send a message about the task abortion.
    wxPostEvent(buttons[4], wxCommandEvent(wxEVT_BUTTON, events::ID_CANCEL));

    // Display GUI error message.
    wxMessageBox(ex.what(), STR_CAPTION4, wxOK | wxCENTRE | wxICON_ERROR, this);
}

void fc::Frame::OnDoneUpdate(fc::events::UpdateDone& event) {
    // Disable "Cancel" button.
    DisableCancelButton();

    // Set new status in the status bar.
    SetStatusText(STR_STATUS1);

    // Start timer to the new status.
    readyTimer->StartOnce(3000);
}

void fc::Frame::OnEncrypt(wxCommandEvent& event) try {
    // Set frame to the "processing" mode.
    DisableButtons();
    DisableFields();
    EnableCancelButton();
    EnableProgressBar();

    // Display new status in the status bar.
    SetStatusText(STR_STATUS3);

    // Get pathes to input and output files.
    const auto ifPath = GetIFPathValue();
    const auto ofPath = GetOFPathValue();

    // Get user password (as string).
    const auto passwordString = GetPasswordValue();

    // Check user data.
    CheckFileIO(ifPath.utf8_string(), ofPath.utf8_string());
    auto inputFile = CheckInputFile(ifPath.utf8_string(), false);
    auto outputFile = CheckOutputFile(ofPath.utf8_string());
    const auto password = CheckPassword(passwordString.utf8_string());

    // Allocate memory for the task.
    std::unique_ptr<Task> task;

    // Open the input file.
    task->SetInputFile(std::move(inputFile));

    // Create an output file.
    task->SetOutputFile(std::move(outputFile));

    // Store user password.
    task->SetPassword(password);

    // Create new thread for the encryption task.
    taskThread = std::make_unique<std::thread>([this, &task]() {
        TaskEncrypt(this, std::move(task));
    });

    // Allow task to run independently.
    taskThread->detach();
} catch (const std::exception& ex) {
    // Send a message about the task abortion.
    wxPostEvent(buttons[4], wxCommandEvent(wxEVT_BUTTON, events::ID_CANCEL));

    // Display GUI error message.
    wxMessageBox(ex.what(), STR_CAPTION4, wxOK | wxCENTRE | wxICON_ERROR, this);
}

void fc::Frame::OnHelp(wxCommandEvent& event) {
    // Display a message box with short documentation.
    wxMessageBox(STR_DOCUMENTATION, STR_CAPTION0, wxOK | wxCENTRE | wxICON_QUESTION, this);
}

void fc::Frame::OnProgressUpdate(fc::events::UpdateProgress& event) {
    // Set new value in the progress bar.
    progressBar->SetValue(event.GetProgress());

    // Notify user about progress update.
    progressBar->Pulse();
}

void fc::Frame::OnReadyTimer(wxTimerEvent& event) {
    // Set default status in the status bar.
    SetStatusText(STR_STATUS0);

    // Set progress bar to the default state.
    progressBar->SetValue(0);

    // Set frame to the default state.
    EnableButtons();
    EnableFields();
    DisableProgressBar();
}

void fc::Frame::OnSet(wxCommandEvent& event) {
    // Open file selector.
    const auto filePath = wxFileSelector(
        STR_CAPTION2,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        wxFileSelectorDefaultWildcardStr,
        wxFD_SAVE,
        this
    );

    // Check if user has set the file.
    if (!filePath.empty()) {
        // Insert file path to the input field.
        SetOFPathValue(filePath);
    }
}

void fc::Frame::DisableButtons() noexcept {
    // Disable all buttons in the collection (except "Cancel" button).
    for (auto button : buttons) {
        // Check if it is not a "Cancel" button.
        if (button == buttons[4]) {
            // Skip.
            continue;
        }

        // Disable this button.
        button->Disable();
    }
}

void fc::Frame::DisableFields() noexcept {
    // Disable all input fields.
    for (auto field : fields) {
        // Disable this field.
        field->Disable();
    }
}

void fc::Frame::EnableButtons() noexcept {
    // Enable all buttons in the collection (except "Cancel" button).
    for (auto button : buttons) {
        // Check if it is not a "Cancel" button.
        if (button == buttons[4]) {
            // Skip.
            continue;
        }

        // Enable this button.
        button->Enable();
    }
}

void fc::Frame::EnableFields() noexcept {
    // Enable all input fields.
    for (auto field : fields) {
        // Enable this field.
        field->Enable();
    }
}
