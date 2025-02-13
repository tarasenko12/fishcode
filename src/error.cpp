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
*/

#include <filesystem>
#include <string>
#include "error.hpp"
#include "file.hpp"
#include "key.hpp"
#include "password.hpp"

const char* fc::error::InvalidFileIO::what() const noexcept {
    return "Invalid I/O configuration!";
}

const char* fc::error::InvalidInputFile::what() const noexcept {
    return "Invalid input file!";
}

const char* fc::error::InvalidOutputFile::what() const noexcept {
    return "Invalid output file!";
}

const char* fc::error::InvalidPassword::what() const noexcept {
    return "Invalid password!";
}

void fc::CheckFileIO(const std::filesystem::path& ifPath, const std::filesystem::path& ofPath) {
  // Check if pathes are not equivalent.
  if (std::filesystem::exists(ifPath) && std::filesystem::exists(ofPath)) {
    if (std::filesystem::equivalent(ifPath, ofPath)) {
      // Invalid file I/O.
      throw error::InvalidFileIO();
    }
  }
}

void fc::CheckInputFile(const std::filesystem::path& ifPath, const bool isEncrypted) {
    // Check if it is path to a regular file.
    if (std::filesystem::is_regular_file(ifPath)) {
        // Open the file.
        const File inputFile(ifPath, FileType::FT_INPUT);

        // Check file size.
        if (isEncrypted) {
            if (inputFile.GetSize() < Key::SIZE + 1) {
                // Invalid input file.
                throw error::InvalidInputFile();
            }
        } else {
            if (inputFile.GetSize() < 1) {
                // Invalid input file.
                throw error::InvalidInputFile();
            }
        }
    } else {
        // Invalid input file.
        throw error::InvalidInputFile();
    }
}

void fc::CheckOutputFile(const std::filesystem::path& ofPath) {
    // Check if path is not empty.
    if (ofPath.empty()) {
        // Invalid output file (no file).
        throw error::InvalidOutputFile();
    }

    // Check if path points to the existing file.
    if (std::filesystem::exists(ofPath)) {
        // Check if it is regular file.
        if (!std::filesystem::is_regular_file(ofPath)) {
            // Invalid output file.
            throw error::InvalidOutputFile();
        }
    }
}

void fc::CheckPassword(const std::string& passwordString) {
    // Check password length.
    if (passwordString.length() < Password::MIN_LENGTH || passwordString.length() > Password::MAX_LENGTH) {
        // Invalid password.
        throw error::InvalidPassword();
    }

    // Check password symbols.
    for (const auto symbol : passwordString) {
        // Only printable ASCII symbols (no spaces).
        if (symbol < '!' || symbol > '~') {
            // Invalid password.
            throw error::InvalidPassword();
        }
    }
}
