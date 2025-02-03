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

#include <array>
#include <random>
#include <cstddef>
#include <cstdint>
#include "key.hpp"

fc::Key fc::Key::Generate() {
  // Create a new empty key.
  Key newKey;

  // Create a new random device.
  std::random_device randomDevice;

  // Use 32-bit 'Mersenne Twister' algorithm.
  std::mt19937 generator(randomDevice());

  // Configure generator to produce numbers from 0 to UINT8_MAX.
  std::uniform_int_distribution<unsigned int> distribute(0, UINT8_MAX);

  // Generate key by bytes.
  for (auto& byte : newKey) {
    // Produce pseudo-random number and store it.
    byte = static_cast<std::uint8_t>(distribute(generator));
  }

  // Return the key.
  return newKey;
}

fc::Key fc::Key::GetRoundKey(const int round) const {
  // Create new empty round key.
  Key newRoundKey;

  // Calculate 'magic' number.
  const auto magicNumber = static_cast<std::uint8_t>(0x4D ^ round);

  // Combine key and the 'magic' number.
  for (std::size_t index = 0; index < SIZE; index++) {
    newRoundKey.bytes[index] = bytes[index] ^ magicNumber;
  }

  // Return round key.
  return newRoundKey;
}

void fc::Key::Encrypt(const fc::Key& anotherKey) noexcept {
  // Encrypt key within 15 rounds.
  for (int round = 0; round < 15; round++) {
    // Step 1: swap bytes.
    for (std::size_t index = 1; index < SIZE; index += 2) {
      // Copy 'index - 1 byte' to the temporary storage.
      const auto temp = bytes[index - 1];

      // Move 'index byte'.
      bytes[index - 1] = bytes[index];

      // Store 'index - 1 byte'.
      bytes[index] = temp;
    }

    // Step 2: get round key.
    const auto roundKey = anotherKey.GetRoundKey(round);

    // Step 3: xor key.
    for (std::size_t index = 0; index < SIZE; index++) {
      bytes[index] ^= roundKey.bytes[index];
    }
  }
}

void fc::Key::Decrypt(const fc::Key& anotherKey) noexcept {
  // Decrypt key within 15 rounds.
  for (int round = 14; round >= 0; round--) {
    // Step 1: get round key.
    const auto roundKey = anotherKey.GetRoundKey(round);

    // Step 2: xor key.
    for (std::size_t index = 0; index < SIZE; index++) {
      bytes[index] ^= roundKey.bytes[index];
    }

    // Step 3: swap bytes.
    for (std::size_t index = 1; index < SIZE; index += 2) {
      // Copy 'index - 1 byte' to the temporary storage.
      const auto temp = bytes[index - 1];

      // Move 'index byte'.
      bytes[index - 1] = bytes[index];

      // Store 'index - 1 byte'.
      bytes[index] = temp;
    }
  }
}

