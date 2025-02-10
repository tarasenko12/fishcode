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
#include <utility>
#include "block.hpp"
#include "error.hpp"
#include "file.hpp"
#include "password.hpp"

using std::filesystem::equivalent;
using std::filesystem::exists;
using std::filesystem::is_regular_file;
using std::move;
using std::filesystem::path;
using std::string;

const char* fc::error::InvalidFileIO::what() const noexcept {
    return "Invalid I/O configuration!";
}

const char* fc::error::InvalidInputFile::what() const noexcept {
    return "Invalid I/O configuration!";
}

const char* fc::error::InvalidOutputFile::what() const noexcept {
    return "Invalid I/O configuration!";
}

const char* fc::error::InvalidPassword::what() const noexcept {
    return "Invalid I/O configuration!";
}

void fc::CheckFileIO(const path& ifPath, const path& ofPath) {
  // Check if pathes are not equivalent.
  if (exists(ifPath) && exists(ofPath)) {
    if (equivalent(ifPath, ofPath)) {
      // Invalid file I/O.
      throw error::InvalidFileIO();
    }
  }
}

fc::File fc::CheckInputFile(const path& ifPath, const bool isEncrypted) {
    // Check if it is path to a regular file.
    if (is_regular_file(ifPath)) {
        // Open file.
        const File inputFile(ifPath, isEncrypted);

        // Check file size.
        if (isEncrypted) {
            if (inputFile.GetSize() == Block::SIZE + 1) {
                // OK. Valid. Return file object.
                return move(inputFile);
            } else {
                // Invalid input file.
                throw error::InvalidInputFile();
            }
        } else {
            if (inputFile.GetSize() > 0) {
                // OK. Valid. Return file object.
                return move(inputFile);
            } else {
                // Invalid input file.
                throw error::InvalidInputFile();
            }
        }
    } else {
        // Invalid input file.
        throw error::InvalidInputFile();
    }
}

fc::File fc::CheckOutputFile(const path& ofPath) {
    // Check if path points to the existing file.
    if (exists(ofPath)) {
        // Check if it is regular file.
        if (is_regular_file(ofPath)) {
            // Create file.
            return File(ofPath);
        } else {
            // Invalid output file.
            throw error::InvalidOutputFile();
        }
    } else {
        // Create file and return it.
        return File(ofPath);
    }
}

fc::Password fc::CheckPassword(const string& passwordString) {
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

    // Create password object and return it.
    return Password(passwordString);
}
