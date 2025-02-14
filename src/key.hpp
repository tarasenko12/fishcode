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

#ifndef FISHCODE_KEY_HPP
#define FISHCODE_KEY_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include "block.hpp"

namespace fc {
    class Key : public Block {
    public:
        Key() = default;
        Key(std::array<std::uint8_t, SIZE>&& newBytes) noexcept;
        Key(const Key& otherKey) = default;
        Key(Key&& otherKey) noexcept = default;

        virtual ~Key() noexcept override = default;

        Key& operator=(const Key& otherKey) = default;
        Key& operator=(Key&& otherKey) noexcept = default;

        static Key Generate();

        Key GetRoundKey(const int round) const;
    };
}

#endif // FISHCODE_KEY_HPP
