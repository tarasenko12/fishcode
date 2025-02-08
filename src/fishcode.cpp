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
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <cstddef>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/timer.h>
#include "block.hpp"
#include "error.hpp"
#include "events.hpp"
#include "file.hpp"
#include "fishcode.hpp"
#include "password.hpp"
#include "strings.hpp"

using std::exception;
using std::invalid_argument;
using std::filesystem::path;
using std::string;
using std::thread;
using std::unique_ptr;

// This defines the equivalent of main() for the current platform.
wxIMPLEMENT_APP(fc::FishCode);

fc::FishCode::FishCode() {
    // Set pointers to the default value (nullptr).
    buttonsSizer = nullptr;
    cancelButton = nullptr;
    chooseButton = nullptr;
    decryptButton = nullptr;
    encryptButton = nullptr;
    frame = nullptr;
    inputFileField = nullptr;
    inputFileLabel = nullptr;
    inputFileSizer = nullptr;
    mainSizer = nullptr;
    menuBar = nullptr;
    menuMore = nullptr;
    outputFileField = nullptr;
    outputFileLabel = nullptr;
    outputFileSizer = nullptr;
    passwordField = nullptr;
    passwordLabel = nullptr;
    passwordSizer = nullptr;
    progressBar = nullptr;
    setButton = nullptr;
    statusBar = nullptr;

    // Configure the about dialog.
    aboutDialogInfo.SetName(STR_NAME0);
    aboutDialogInfo.SetVersion(STR_VERSION);
    aboutDialogInfo.SetDescription(STR_DESCRYPTION);
    aboutDialogInfo.SetCopyright(STR_COPYRIGHT);
    aboutDialogInfo.AddDeveloper(STR_DEVELOPER);
}

bool fc::FishCode::OnInit() try {
    // Create the main window (frame).
    frame = new Frame();

    // Configure timer.
    readyTimer.SetOwner(frame, events::ID_READY);

    // Configure event handlers.
    frame->Bind(wxEVT_MENU, &fc::FishCode::OnAbout, this, wxID_ABOUT);
    frame->Bind(wxEVT_MENU, &fc::FishCode::OnHelp, this, wxID_HELP);
    frame->Bind(fc::events::EVT_UPDATE_DONE, &fc::FishCode::OnDoneUpdate, this, fc::events::ID_DONE);
    frame->Bind(fc::events::EVT_UPDATE_PROGRESS, &fc::FishCode::OnProgressUpdate, this, fc::events::ID_PROGRESS);
    frame->Bind(wxEVT_TIMER, &fc::FishCode::OnReadyTimer, this, events::ID_READY);
    chooseButton->Bind(wxEVT_BUTTON, &fc::FishCode::OnChoose, this, events::ID_CHOOSE);
    setButton->Bind(wxEVT_BUTTON, &fc::FishCode::OnSet, this, events::ID_SET);
    encryptButton->Bind(wxEVT_BUTTON, &fc::FishCode::OnEncrypt, this, events::ID_ENCRYPT);
    decryptButton->Bind(wxEVT_BUTTON, &fc::FishCode::OnDecrypt, this, events::ID_DECRYPT);
    cancelButton->Bind(wxEVT_BUTTON, &fc::FishCode::OnCancel, this, events::ID_CANCEL);

    // Show the window.
    frame->Show();

    // Start the application.
    return true;
} catch (const std::exception& ex) {
    // Display GUI error message.
    wxMessageBox(ex.what(), STR_CAPTION3, wxOK | wxCENTRE | wxICON_ERROR);

    // Print error message to the terminal.
    std::cerr << ex.what() << std::endl;

    // Do not start the application.
    return false;
}

void fc::FishCode::OnAbout(wxCommandEvent& event) {
    // Show about box.
    wxAboutBox(aboutDialogInfo, frame);
}

void fc::FishCode::OnCancel(wxCommandEvent& event) {
    // Disable "Cancel" button.
    frame->DisableCancelButton();

    // Enable task abortion.
    taskShouldCancel = true;

    // Wait for the task termination (if there is a task).
    if (taskThread.joinable()) {
        taskThread.join();
    }

    // Disable task abortion.
    taskShouldCancel = false;

    // Set new status in the status bar.
    frame->SetStatusText(STR_STATUS2);

    // Start timer to the new status.
    readyTimer.StartOnce(3000);
}

void fc::FishCode::OnChoose(wxCommandEvent& event) {
    // Open file selector.
    const auto filePath = wxFileSelector(
        STR_CAPTION1,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        wxFileSelectorDefaultWildcardStr,
        wxFD_OPEN,
        frame
    );

    // Check if user has chosen the file.
    if (!filePath.empty()) {
        // Insert file path to the input file line.
        frame->SetIFPathValue(filePath);
    }
}

void fc::FishCode::OnDecrypt(wxCommandEvent& event) try {
    // Disable controls.
    frame->DisableButtons();
    frame->DisableFields();

    // Enable progress bar.
    frame->EnableProgressBar();

    // Enable "Cancel" button.
    frame->EnableCancelButton();

    // Display new status in the status bar.
    frame->SetStatusText(STR_STATUS4);

    // Get pathes to input and output files.
    const auto inputFilePath = GetFilePath(inputFileField);
    const auto outputFilePath = GetFilePath(outputFileField);

    // Get user password (as string).
    const string passwordString = frame->GetPasswordValue().utf8_string();

    // Check user data.
    CheckFileIO(inputFilePath, outputFilePath);
    //CheckInputFile();
    //CheckOutputFile();
    //CheckPassword();

    // Allocate memory for the task.
    unique_ptr<Task> task;

    // Open the input file.
    task->SetInputFile(File(inputFilePath, true));

    // Create an output file.
    task->SetOutputFile(File(outputFilePath));

    // Store user password.
    task->SetPassword(Password(passwordString));

    // Create new thread for the encryption task.
    shared_ptr<taskThread> = make_shared<std::thread>([this, &task]() {
        TaskDecrypt(this->frame, std::move(task));
    });

    // Allow task to continue independently.
    taskThread.detach();
} catch (const std::exception& ex) {
    // Display GUI error message.
    wxMessageBox(ex.what(), STR_CAPTION4, wxOK | wxCENTRE | wxICON_ERROR, frame);

    // Abort the task (if it is running).
    if (!taskShouldCancel) {
        AbortTask();
    }
}

void fc::FishCode::OnDoneUpdate(fc::events::UpdateDone& event) {
    // Disable "Cancel" button.
    frame->DisableCancelButton();

    // Set new status in the status bar.
    frame->SetStatusText(STR_STATUS1);

    // Start timer to the new status.
    readyTimer.StartOnce(3000);
}

void fc::FishCode::OnEncrypt(wxCommandEvent& event) try {
    // Disable controls.
    DisableControls();

    // Enable "Cancel" button.
    cancelButton->Enable();

    // Display new status in the status bar.
    statusBar->SetStatusText(STR_STATUS3);

    // Get pathes to input and output files.
    const auto inputFilePath = GetFilePath(inputFileField);
    const auto outputFilePath = GetFilePath(outputFileField);

    // Get user password (as string).
    const string passwordString = passwordField->GetValue().utf8_string();

    // Check user data.
    CheckFileIO(inputFilePath, outputFilePath);
    //CheckInputFile();
    //CheckOutputFile();
    //CheckPassword();

    // Allocate memory for the task.
    unique_ptr<Task> task;

    // Open the input file.
    task->SetInputFile(File(inputFilePath, false));

    // Create an output file.
    task->SetOutputFile(File(outputFilePath));

    // Store user password.
    task->SetPassword(Password(passwordString));

    // Create new thread for the encryption task.
    taskThread = thread([this, &task]() {
        TaskEncrypt(this->frame, std::move(task));
    });

    // Allow task to continue independently.
    taskThread.detach();
} catch (const std::exception& ex) {
    // Display GUI error message.
    wxMessageBox(ex.what(), STR_CAPTION4, wxOK | wxCENTRE | wxICON_ERROR, frame);

    // Abort the task (if it is running).
    if (!taskShouldCancel) {
        AbortTask();
    }
}

void fc::FishCode::OnHelp(wxCommandEvent& event) {
    // Display a message box with short documentation.
    wxMessageBox(
        STR_DOCUMENTATION,
        STR_CAPTION0,
        wxOK | wxCENTRE | wxICON_QUESTION,
        frame
    );
}

void fc::FishCode::OnReadyTimer(wxTimerEvent& event) {
    // Set default status in the status bar.
    statusBar->SetStatusText(STR_STATUS0);

    // Set progress bar to the default state.
    progressBar->SetValue(0);

    // Enable controls.
    EnableControls();
}

void fc::FishCode::OnSet(wxCommandEvent& event) {
    // Open file selector.
    const auto filePath = wxFileSelector(
        STR_CAPTION2,
        wxEmptyString,
        wxEmptyString,
        wxEmptyString,
        wxFileSelectorDefaultWildcardStr,
        wxFD_SAVE,
        frame
    );

    // Check if user has set the file.
    if (!filePath.empty()) {
        // Insert file path to the output file line.
        outputFileField->ChangeValue(filePath);
    }
}

void fc::FishCode::DisableControls() {
    // Disable all control items (except "Cancel" button).
    inputFileField->Disable();
    chooseButton->Disable();
    outputFileField->Disable();
    setButton->Disable();
    passwordField->Disable();
    encryptButton->Disable();
    decryptButton->Disable();
}

void fc::FishCode::EnableControls() {
    // Enable all control items (except "Cancel" button).
    inputFileField->Enable();
    chooseButton->Enable();
    outputFileField->Enable();
    setButton->Enable();
    passwordField->Enable();
    encryptButton->Enable();
    decryptButton->Enable();
}
