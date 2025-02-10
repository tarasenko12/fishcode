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
#include <iostream>
#include <wx/msgdlg.h>
#include "fishcode.hpp"
#include "frame.hpp"
#include "strings.hpp"

// This defines the equivalent of main() for the current platform.
wxIMPLEMENT_APP(fc::FishCode);

bool fc::FishCode::OnInit() try {
    // Create the main window (frame).
    auto frame = new Frame();

    // Show the window.
    frame->Show();

    // Start the application.
    return true;
} catch (const std::exception& ex) {
    // Print error message to the terminal.
    std::cerr << ex.what() << std::endl;

    // Display GUI error message (if it is possible).
    wxMessageBox(ex.what(), STR_CAPTION3, wxOK | wxCENTRE | wxICON_ERROR);

    // Do not start the application.
    return false;
}
