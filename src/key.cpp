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

#include <array>
#include <random>
#include <cstddef>
#include <cstdint>
#include "key.hpp"

fc::Key::Key(std::array<std::uint8_t, fc::Key::SIZE>&& newBytes)
: fc::Block(newBytes, fc::Key::SIZE) {

}

fc::Key fc::Key::Generate() {
    // Create a new random device.
    std::random_device randomDevice;

    // Use 32-bit 'Mersenne Twister' algorithm.
    std::mt19937 generator(randomDevice());

    // Configure generator to produce numbers from 0 to UINT8_MAX.
    std::uniform_int_distribution<unsigned int> distribute(0, UINT8_MAX);

    // Create storage for the generated key bytes.
    std::array<std::uint8_t, SIZE> generatedBytes;

    // Generate key by bytes.
    for (std::size_t counter = 0; counter < SIZE; counter++) {
        // Produce pseudo-random number and store it.
        generatedBytes[counter] = static_cast<std::uint8_t>(distribute(generator));
    }

    // Return generated key.
    return Key(generatedBytes);
}

fc::Key fc::Key::GetRoundKey(const int round) const {
    // Get current key bytes.
    const auto currentBytes = GetBytes();

    // Create storage for the new key bytes.
    std::array<std::uint8_t, SIZE> newBytes;

    // Calculate a 'magic' number.
    const auto magicNumber = static_cast<std::uint8_t>(0x4D ^ round);

    // Combine key bytes and the 'magic' number.
    for (std::size_t index = 0; index < Key::SIZE; index++) {
        newBytes[index] = currentBytes[index] ^ magicNumber;
    }

    // Return new round key.
    return Key(newBytes);
}
