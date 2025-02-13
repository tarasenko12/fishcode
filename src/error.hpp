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

#ifndef FISHCODE_ERROR_HPP
#define FISHCODE_ERROR_HPP

#include <exception>
#include <string>
#include "file.hpp"
#include "password.hpp"

namespace fc {
    namespace error {
        class InvalidFileIO : public std::exception {
        public:
            InvalidFileIO() noexcept = default;
            InvalidFileIO(const InvalidFileIO& other) = default;
            InvalidFileIO(InvalidFileIO&& other) noexcept = default;

            ~InvalidFileIO() noexcept = default;

            InvalidFileIO& operator=(const InvalidFileIO& other) = default;
            InvalidFileIO& operator=(InvalidFileIO&& other) noexcept = default;

            const char* what() const noexcept override;
        };

        class InvalidInputFile : public std::exception {
        public:
            InvalidInputFile() noexcept = default;
            InvalidInputFile(const InvalidInputFile& other) = default;
            InvalidInputFile(InvalidInputFile&& other) noexcept = default;

            ~InvalidInputFile() noexcept = default;

            InvalidInputFile& operator=(const InvalidInputFile& other) = default;
            InvalidInputFile& operator=(InvalidInputFile&& other) noexcept = default;

            const char* what() const noexcept override;
        };

        class InvalidOutputFile : public std::exception {
        public:
            InvalidOutputFile() noexcept = default;
            InvalidOutputFile(const InvalidOutputFile& other) = default;
            InvalidOutputFile(InvalidOutputFile&& other) noexcept = default;

            ~InvalidOutputFile() noexcept = default;

            InvalidOutputFile& operator=(const InvalidOutputFile& other) = default;
            InvalidOutputFile& operator=(InvalidOutputFile&& other) noexcept = default;

            const char* what() const noexcept override;
        };

        class InvalidPassword : public std::exception {
        public:
            InvalidPassword() noexcept = default;
            InvalidPassword(const InvalidPassword& other) = default;
            InvalidPassword(InvalidPassword&& other) noexcept = default;

            ~InvalidPassword() noexcept = default;

            InvalidPassword& operator=(const InvalidPassword& other) = default;
            InvalidPassword& operator=(InvalidPassword&& other) noexcept = default;

            const char* what() const noexcept override;
        };
    }

    void CheckFileIO(const std::filesystem::path& ifPath, const std::filesystem::path& ofPath);
    void CheckInputFile(const std::filesystem::path& inputFilePath, const bool isEncrypted);
    void CheckOutputFile(const std::filesystem::path& outputFilePath);
    void CheckPassword(const std::string& passwordString);
}

#endif // FISHCODE_ERROR_HPP
