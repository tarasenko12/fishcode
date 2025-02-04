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

#ifndef FISHCODE_PASSWORD_HPP
#define FISHCODE_PASSWORD_HPP

#include <string>
#include <cstddef>
#include "block.hpp"

namespace fc {
    class Password : public Block {
    public:
        static constexpr const std::size_t MIN_LENGTH = 8;
        static constexpr const std::size_t MAX_LENGTH = SIZE;

        Password() = default;
        Password(const std::string& passwordString);
        Password(const Password& otherPassword) = default;
        Password(Password&& otherPassword) = default;

        ~Password() noexcept = default;

        Password& operator=(const Password& otherPassword) = default;
        Password& operator=(Password&& otherPassword) = default;
    };
}

#endif // FISHCODE_PASSWORD_HPP
