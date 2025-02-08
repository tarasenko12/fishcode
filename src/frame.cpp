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

#include <wx/event.>
#include <wx/frame.h>
#include "events.hpp"
#include "frame.hpp"
#include "strings.hpp"

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

    // Create and set new status bar for the frame.
    SetStatusBar(CreateStatusBar());

    // Set the default status text.
    SetStatusText(STR_STATUS0);

    // Initialize new sizer for the main window.
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    // Configure grid sizers layout.
    const wxSize gridLayout(5, 5);

    // Configure sizers (using flags).
    const auto mainSizerFlags = wxSizerFlags().Expand().Border(wxALL, 5);
    const auto gridSizerFlags = wxSizerFlags().Expand().Border(wxALL, 10);
    const auto gridLabelSizerFlags = wxSizerFlags().Center().Right().Border(wxALL, 2);

    // Set layout for the fields.
    const wxSize fieldSize(400, 45);


    // Configure UI sizers.
    auto sizers[] = {
        new wxFlexGridSizer(1, 3, gridLayout);
        new wxFlexGridSizer(1, 3, gridLayout);
        new wxFlexGridSizer(1, 2, gridLayout);
        new wxFlexGridSizer(1, 2, gridLayout);
    };
    sizers[0]->AddGrowableCol(1);
    sizers[0]->AddGrowableCol(2);
    sizers[1]->AddGrowableCol(1);
    sizers[1]->AddGrowableCol(2);
    sizers[2]->AddGrowableCol(1);

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
    sizers[0]->Add(labels[0], gridLabelSizerFlags);
    sizers[0]->Add(fields[0], gridSizerFlags);
    sizers[1]->Add(labels[1], gridLabelSizerFlags);
    sizers[1]->Add(fields[1], gridSizerFlags);
    sizers[2]->Add(labels[2], gridLabelSizerFlags);
    sizers[2]->Add(fields[2]], gridSizerFlags);


    // Create and configure buttons.
    buttons[0] = new Button(this, events::ID_CHOOSE, STR_LABEL2);
    buttons[1] = new Button(this, events::ID_SET, STR_LABEL3);
    buttons[2] = new Button(this, events::ID_ENCRYPT, STR_LABEL5);
    buttons[3] = new Button(this, events::ID_DECRYPT, STR_LABEL6);
    buttons[4] = new Button(this, events::ID_CANCEL, STR_LABEL7);
    sizers[0]->Add(buttons[0], gridSizerFlags);
    sizers[1]->Add(buttons[1], gridSizerFlags);
    sizers[3]->Add(buttons[0], gridSizerFlags);
    sizers[3]->Add(buttons[0], gridSizerFlags);

    // Disable "Cancel" button for now.
    buttons[4]->Disable();

    // Add UI sizers (fields) context to the frame (using sizer).
    mainSizer->Add(sizers[0], mainSizerFlags);
    mainSizer->Add(sizers[1], mainSizerFlags);
    mainSizer->Add(sizers[3], mainSizerFlags);

    // Create a progress bar.
    progressBar = new ProgressBar(this);

    // Disable progress bar for now.
    progressBar->Disable();

    // Add progress bar to the frame.
    mainSizer->Add(progressBar, mainSizerFlags);

    // Add "Cancel" button to the frame (bottom).
    mainSizer->Add(buttons[0], gridSizerFlags);

    // Connect main window (frame) with its sizer.
    SetSizerAndFit(mainSizer);
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
