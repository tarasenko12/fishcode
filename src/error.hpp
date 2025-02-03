/*
** Copyright (C) 2024 Vitaliy Tarasenko.
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

#ifndef FISHCODE_ERROR_HPP
#define FISHCODE_ERROR_HPP

#include <exception>

namespace fc {
  class InvalidPasswordError : public std::exception {
  public:
    InvalidPasswordError() noexcept = default;
    InvalidPasswordError(const InvalidPasswordError& other) = default;
    InvalidPasswordError(InvalidPasswordError&& other) = default;

    ~InvalidPasswordError() noexcept = default;

    InvalidPasswordError& operator=(const InvalidPasswordError& other) = default;
    InvalidPasswordError& operator=(InvalidPasswordError&& other) = default;

    const char* what() const noexcept override;
  };

  class InvalidInputFileError : public std::exception {
  public:
    InvalidInputFileError() noexcept = default;
    InvalidInputFileError(const InvalidInputFileError& other) = default;
    InvalidInputFileError(InvalidInputFileError&& other) = default;

    ~InvalidInputFileError() noexcept = default;

    InvalidInputFileError& operator=(const InvalidInputFileError& other) = default;
    InvalidInputFileError& operator=(InvalidInputFileError&& other) = default;

    const char* what() const noexcept override;
  };

  class InvalidOutputFileError : public std::exception {
  public:
    InvalidOutputFileError() noexcept = default;
    InvalidOutputFileError(const InvalidOutputFileError& other) = default;
    InvalidOutputFileError(InvalidOutputFileError&& other) = default;

    ~InvalidOutputFileError() noexcept = default;

    InvalidOutputFileError& operator=(const InvalidOutputFileError& other) = default;
    InvalidOutputFileError& operator=(InvalidOutputFileError&& other) = default;

    const char* what() const noexcept override;
  };
}

#endif // FISHCODE_ERROR_HPP

