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

#ifndef FISHCODE_LABEL_HPP
#define FISHCODE_LABEL_HPP

#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/window.h>

namespace fc {
    class Label : public wxStaticText {
    public:
        Label(wxWindow* parent, const wxString& text);
        Label(const Label& otherLabel) = delete;
        Label(Label&& otherLabel) noexcept = delete;

        Label& operator=(const Label& otherLabel) = delete;
        Label& operator=(Label&& otherLabel) noexcept = delete;

        ~Label() noexcept override = default;
    };
}

#endif // FISHCODE_LABEL_HPP
