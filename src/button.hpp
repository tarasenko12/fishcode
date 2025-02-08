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

#ifndef FISHCODE_BUTTON_HPP
#define FISHCODE_BUTTON_HPP

#include <wx/button.h>
#include <wx/string.h>
#include <wx/window.h>

namespace fc {
    class Button : public wxButton {
    public:
        Button(wxWindow* parent, const int id, const wxString& label);
        Button(const Button& otherButton) = delete;
        Button(Button&& otherButton) = delete;

        Button& operator=(const Button& otherButton) = delete;
        Button& operator=(Button&& otherButton) = delete;

        ~Button() noexcept override = default;
    };
}

#endif // FISHCODE_BUTTON_HPP
