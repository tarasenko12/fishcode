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
#include <utility>
#include <cstddef>
#include <cstdint>

namespace fc {
    class Key;

    class Block {
    public:
        static constexpr const std::size_t SIZE = 16;

        Block();
        Block(std::array<std::uint8_t, SIZE>&& newBytes, const std::size_t newRealSize) noexcept;
        Block(const Block& otherBlock) = default;
        Block(Block&& otherBlock) noexcept = default;

        virtual ~Block() noexcept = default;

        Block& operator=(const Block& otherBlock) = default;
        Block& operator=(Block&& otherBlock) noexcept = default;

        inline std::array<std::uint8_t, SIZE> GetBytes() const {
            return bytes;
        }

        inline std::size_t GetRealSize() const noexcept {
            return realSize;
        }

        void Decrypt(const Key& key);
        void Encrypt(const Key& key);
    protected:
        inline void SetBytes(std::array<uint8_t, SIZE>&& newBytes) noexcept {
            bytes = std::move(newBytes);
        }

        inline void SetRealSize(const std::size_t newRealSize) noexcept {
            realSize = newRealSize;
        }
    private:
        std::array<std::uint8_t, SIZE> bytes;
        std::size_t realSize;
    };
}

#endif // FISHCODE_BLOCK_HPP
