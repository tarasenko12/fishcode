/*
** Copyright (C) 2025 Vitaliy Tarasenko.
**
** This file is part of FishCode.
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
*/

#ifndef FISHCODE_STRINGS_HPP
#define FISHCODE_STRINGS_HPP

namespace fc {
  constexpr const auto STR_DESCRYPTION =
    "FishCode is a program for encrypting and decrypting files.\n\n"
    "FishCode is free software: you can redistribute it and/or modify it "
    "under the terms of the GNU General Public License as published by the "
    "Free Software Foundation, either version 3 of the License, or "
    "(at your option) any later version.\n\n"
    "FishCode is distributed in the hope that it will be useful, but WITHOUT "
    "ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
    "FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for "
    "more details.";
  constexpr const auto STR_DOCUMENTATION =
    "\tFishcode interface consists of two main components: the main window "
    "and an additional menu called \"More...\".\n\n"
    "\tThe main window has three input fields (\"Input file\", "
    "\"Output file\", \"Password\") and four buttons (\"Choose...\", "
    "\"Set...\", \"Encrypt\", \"Decrypt\").\n\n"
    "\tThe \"Input file\" and \"Output file\" fields are for entering "
    "the path to the corresponding files on the disk. The file path "
    "can be either absolute, such as \"C:\\users\\user\\Downloads\\file\", "
    "or relative, such as \"Downloads/file\".\n\n"
    "\tThe \"Choose...\" and \"Set...\" buttons are alternative ways to "
    "identify the relevant files. These buttons bring up the corresponding "
    "dialog boxes.\n\n"
    "\tThe \"Encrypt\" and \"Decrypt\" buttons perform the operations "
    "corresponding to their name. The status of the operation is displayed "
    "in the progress bar and at the bottom of the window, "
    "in the status field.\n\n"
    "\tNote: password cannot contain spaces and non-Latin letters and symbols "
    "that are not part of the ASCII character set.";
  constexpr const auto STR_COPYRIGHT =
    "Copyright (C) 2024-2025 Vitaliy Tarasenko.";
  constexpr const auto STR_VERSION = "v1.0";
  constexpr const auto STR_DEVELOPER = "Vitaliy Tarasenko";
  constexpr const auto STR_CAPTION0 = "User documentation";
  constexpr const auto STR_CAPTION1 = "Choose an input file";
  constexpr const auto STR_CAPTION2 = "Set an output file";
  constexpr const auto STR_CAPTION3 = "Fatal error!";
  constexpr const auto STR_CAPTION4 = "Error!";
  constexpr const auto STR_NAME0 = "FishCode";
  constexpr const auto STR_NAME1 = "More...";
  constexpr const auto STR_NAME2 = "About";
  constexpr const auto STR_NAME3 = "Help";
  constexpr const auto STR_LABEL0 = "Input file:";
  constexpr const auto STR_LABEL1 = "Output file:";
  constexpr const auto STR_LABEL2 = "Choose...";
  constexpr const auto STR_LABEL3 = "Set...";
  constexpr const auto STR_LABEL4 = "Password:";
  constexpr const auto STR_LABEL5 = "Encrypt";
  constexpr const auto STR_LABEL6 = "Decrypt";
  constexpr const auto STR_LABEL7 = "Cancel";
  constexpr const auto STR_PROMPT0 = "Get more information about the program.";
  constexpr const auto STR_PROMPT1 = "Get user documentation.";
  constexpr const auto STR_STATUS0 = "Ready";
  constexpr const auto STR_STATUS1 = "All done";
  constexpr const auto STR_STATUS2 = "Abort";
  constexpr const auto STR_STATUS3 = "Encrypting...";
  constexpr const auto STR_STATUS4 = "Decrypting...";
}

#endif // FISHCODE_STRINGS_HPP

