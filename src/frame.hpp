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

#ifndef FISHCODE_FRAME_HPP
#define FISHCODE_FRAME_HPP

#include <array>
#include <memory>
#include <thread>
#include <wx/event.h>
#include <wx/frame.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/timer.h>
#include "button.hpp"
#include "events.hpp"
#include "field.hpp"
#include "label.hpp"
#include "progress.hpp"
#include "strings.hpp"

namespace fc {
    class Frame : public wxFrame {
    public:
        Frame();
        Frame(const Frame& otherFrame) = delete;
        Frame(Frame&& otherFrame) noexcept = delete;

        Frame& operator=(const Frame& otherFrame) = delete;
        Frame& operator=(Frame&& otherFrame) noexcept = delete;

        ~Frame() noexcept override = default;

        void OnAbout(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnChoose(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnDecrypt(wxCommandEvent& event);
        void OnDoneUpdate(events::UpdateDone& event);
        void OnEncrypt(wxCommandEvent& event);
        void OnHelp(wxCommandEvent& event);
        void OnProgressUpdate(events::UpdateProgress& event);
        void OnReadyTimer(wxTimerEvent& event);
        void OnTaskException(events::TaskException& event);
        void OnSet(wxCommandEvent& event);
    private:
        std::unique_ptr<std::thread> taskThread;
        std::array<Button*, 5> buttons;
        std::array<Field*, 3> fields;
        std::array<Label*, 3> labels;
        std::unique_ptr<wxTimer> readyTimer;
        ProgressBar* progressBar;

        inline wxString GetIFPathValue() const noexcept {
            return fields[0]->GetValue();
        }

        inline wxString GetOFPathValue() const noexcept {
            return fields[1]->GetValue();
        }

        inline wxString GetPasswordValue() const noexcept {
            return fields[2]->GetValue();
        }

        inline void SetIFPathValue(const wxString& newValue) noexcept {
            fields[0]->ChangeValue(newValue);
        }

        inline void SetOFPathValue(const wxString& newValue) noexcept {
            fields[1]->ChangeValue(newValue);
        }

        inline void DisableCancelButton() noexcept {
            buttons[4]->Disable();
        }

        void DisableButtons() noexcept;
        void DisableFields() noexcept;

        inline void DisableProgressBar() noexcept {
            progressBar->Disable();
        }

        inline void EnableCancelButton() noexcept {
            buttons[4]->Enable();
        }

        void EnableButtons() noexcept;
        void EnableFields() noexcept;

        inline void EnableProgressBar() noexcept {
            progressBar->Enable();
        }
    };
}

#endif // FISHCODE_FRAME_HPP
