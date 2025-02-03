/*
** Copyright (C) 2024-2025 Vitaliy Tarasenko.
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

#ifndef FISHCODE_BLOCK_HPP
#define FISHCODE_BLOCK_HPP

#include <vector>
#include <cstddef>
#include <cstdint>
#include "key.hpp"

namespace fc {
  class Block {
  public:
    static constexpr const std::size_t CAPACITY = 16;

    Block();
    Block(const Block& otherBlock) = default;
    Block(Block&& otherBlock) = default;

    ~Block() noexcept = default;

    Block& operator=(const Block& otherBlock) = default;
    Block& operator=(Block&& otherBlock) = default;

    inline std::vector<std::uint8_t>::const_iterator begin() const noexcept {
      // Call corresponding std::vector method.
      return bytes.begin();
    }

    inline std::vector<std::uint8_t>::const_iterator end() const noexcept {
      // Call corresponding std::vector method.
      return bytes.end();
    }

    inline void PushByte(const std::uint8_t byte) {
      // Store one byte to the vector.
      bytes.push_back(byte);
    }

    void Encrypt(const Key& key) noexcept;
    void Decrypt(const Key& key) noexcept;
  private:
    std::vector<std::uint8_t> bytes;
  };
}

#endif // FISHCODE_BLOCK_HPP

