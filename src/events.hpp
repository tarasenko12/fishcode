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

#ifndef FISHCODE_EVENT_HPP
#define FISHCODE_EVENT_HPP

#include <wx/event.h>

namespace fc {
    namespace events {
        enum ControlItemID {
            ID_CANCEL = 1,
            ID_CHOOSE,
            ID_DECRYPT,
            ID_ENCRYPT,
            ID_SET
        };

        enum TimerID {
            ID_READY = ID_SET + 1
        };

        enum UpdateID {
            ID_DONE = ID_READY + 1,
            ID_PROGRESS
        };

        enum WindowID {
            ID_FRAME = ID_PROGRESS + 1
        };

        class UpdateDone : public wxEvent {
        public:
            UpdateDone(const int newID);
            UpdateDone(const UpdateDone& otherUpdateDone) = default;
            UpdateDone(UpdateDone&& otherUpdateDone) = default;

            UpdateDone& operator=(const UpdateDone& otherUpdateDone) = default;
            UpdateDone& operator=(UpdateDone&& otherUpdateDone) = default;

            ~UpdateDone() noexcept override = default;

            inline wxEvent* Clone() const override {
                return new UpdateDone(*this);
            }
        };

        wxDECLARE_EVENT(EVT_UPDATE_DONE, UpdateDone);

        class UpdateProgress : public wxEvent {
        public:
            UpdateProgress() = delete;
            UpdateProgress(const int newID, const int newProgress);
            UpdateProgress(const UpdateProgress& otherUpdateProgress) = default;
            UpdateProgress(UpdateProgress&& otherUpdateProgress) = default;

            UpdateProgress& operator=(const UpdateProgress& otherUpdateProgress) = default;
            UpdateProgress& operator=(UpdateProgress&& otherUpdateProgress) = default;

            ~UpdateProgress() noexcept override = default;

            inline wxEvent* Clone() const override {
                return new UpdateProgress(*this);
            }

            inline int GetProgress() const noexcept {
                return progress;
            }
        private:
            int progress;
        };

        wxDECLARE_EVENT(EVT_UPDATE_PROGRESS, UpdateProgress);
    }
}

#endif // FISHCODE_EVENT_HPP
