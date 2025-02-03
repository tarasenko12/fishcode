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

#ifndef FISHCODE_KEY_HPP
#define FISHCODE_KEY_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace fc {
  class Key {
  public:
    static constexpr const std::size_t SIZE = 16;

    static Key Generate();

    Key() = default;
    Key(const Key& otherKey) = default;
    Key(Key&& otherKey) = default;

    ~Key() noexcept = default;

    Key& operator=(const Key& otherKey) = default;
    Key& operator=(Key&& otherKey) = default;

    inline const std::uint8_t& operator[](const std::size_t index)
    const noexcept {
      // Call corresponding std::array overloaded operator.
      return bytes[index];
    }

    inline std::array<std::uint8_t, SIZE>::iterator begin() noexcept {
      // Call corresponding std::array method.
      return bytes.begin();
    }

    inline std::array<std::uint8_t, SIZE>::iterator end() noexcept {
      // Call corresponding std::array method.
      return bytes.end();
    }

    inline std::array<std::uint8_t, SIZE>::const_iterator begin()
    const noexcept {
      // Call corresponding std::array method.
      return bytes.begin();
    }

    inline std::array<std::uint8_t, SIZE>::const_iterator end()
    const noexcept {
      // Call corresponding std::array method.
      return bytes.end();
    }

    Key GetRoundKey(const int round) const;
    void Encrypt(const Key& anotherKey) noexcept;
    void Decrypt(const Key& anotherKey) noexcept;
  protected:
    std::array<std::uint8_t, SIZE> bytes;
  };
}

#endif // FISHCODE_KEY_HPP

