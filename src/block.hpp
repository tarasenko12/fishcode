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

#ifndef FISHCODE_BLOCK_HPP
#define FISHCODE_BLOCK_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace fc {
    class Block {
    public:
        static constexpr const std::size_t SIZE = 16;

        Block();
        Block(const Block& otherBlock) = default;
        Block(Block&& otherBlock) = default;

        ~Block() noexcept = default;

        Block& operator=(const Block& otherBlock) = default;
        Block& operator=(Block&& otherBlock) = default;

        static Block Generate();

        inline std::uint8_t& operator[](const std::size_t index) noexcept {
            // Call corresponding std::array overloaded operator.
            return bytes[index];
        }

        inline const std::uint8_t& operator[](const std::size_t index) const noexcept {
            // Call corresponding std::array overloaded operator.
            return bytes[index];
        }

        inline std::size_t GetRealSize() const noexcept {
            return realSize;
        }

        void Push(const std::uint8_t byte);

        void Decrypt(const Block& key) noexcept;
        void Encrypt(const Block& key) noexcept;
    private:
        std::array<std::uint8_t, SIZE> bytes;
        std::size_t offset, realSize;

        Block GetRoundVersion(const int round) const;
    };
}

#endif // FISHCODE_BLOCK_HPP
