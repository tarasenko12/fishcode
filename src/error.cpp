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
#include "error.hpp"

using std::filesystem::equivalent;
using std::filesystem::exists;
using std::filesystem::path;

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

void fc::CheckFileIO(const path& inputFilePath, const path& outputFilePath) {
  // Check if pathes are not equivalent.
  if (exists(outputFilePath)) {
    if (equivalent(inputFilePath, outputFilePath)) {
      // Invalid file I/O.
      throw error::InvalidFileIO();
    }
  }
}
