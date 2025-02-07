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

#ifndef FISHCODE_HPP
#define FISHCODE_HPP

#include <filesystem>
#include <thread>
#include <wx/aboutdlg.h>
#include <wx/app.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/gauge.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include "events.hpp"
#include "frame.hpp"
#include "task.hpp"

namespace fc {
    class FishCode : public wxApp {
    public:
        FishCode();
        FishCode(const FishCode& otherFishCode) = delete;
        FishCode(FishCode&& otherFishCode) = delete;

        FishCode& operator=(const FishCode& otherFishCode) = delete;
        FishCode& operator=(FishCode&& otherFishCode) = delete;

        ~FishCode() noexcept override = default;

        bool OnInit() override;
        int OnExit() override;
    private:
        std::thread taskThread;

        wxAboutDialogInfo aboutDialogInfo;
        wxGridSizer* buttonsSizer;
        wxButton* cancelButton;
        wxButton* chooseButton;
        wxButton* decryptButton;
        wxButton* encryptButton;
        Frame* frame;
        wxTextCtrl* inputFileField;
        wxStaticText* inputFileLabel;
        wxFlexGridSizer* inputFileSizer;
        wxBoxSizer* mainSizer;
        wxMenuBar* menuBar;
        wxMenu* menuMore;
        wxTextCtrl* outputFileField;
        wxStaticText* outputFileLabel;
        wxFlexGridSizer* outputFileSizer;
        wxTextCtrl* passwordField;
        wxStaticText* passwordLabel;
        wxFlexGridSizer* passwordSizer;
        wxGauge* progressBar;
        wxTimer readyTimer;
        wxButton* setButton;
        wxStatusBar* statusBar;

        void OnAbout(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnChoose(wxCommandEvent& event);
        void OnDecrypt(wxCommandEvent& event);
        void OnDoneUpdate(events::UpdateDone& event);
        void OnEncrypt(wxCommandEvent& event);
        void OnHelp(wxCommandEvent& event);

        inline void OnProgressUpdate(events::UpdateProgress& event) {
            // Update progress value in the progress bar.
            progressBar->SetValue(event.GetProgress());
        }

        void OnReadyTimer(wxTimerEvent& event);
        void OnSet(wxCommandEvent& event);

        std::filesystem::path GetFilePath(wxTextCtrl* field);

        inline void AbortTask() {
            // Send a message to abort the task.
            wxPostEvent(cancelButton, wxCommandEvent(wxEVT_BUTTON, events::ID_CANCEL));
        }

        void DisableControls();
        void EnableControls();
    };
}

#endif // FISHCODE_HPP
