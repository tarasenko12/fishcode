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
#include <filesystem>
#include <ios>
#include <utility>
#include <cstddef>
#include <cstdint>
#include "block.hpp"
#include "file.hpp"
#include "key.hpp"

fc::File::File() {
    // Now its file stream is not valid (no real file).
    stream.setstate(std::ios::badbit | std::ios::eofbit);

    // Now it is empty file.
    size = 0;
}

fc::File::File(const std::filesystem::path& newFSPath, const fc::FileType type)
: fsPath(newFSPath) {
    if (type == FileType::FT_INPUT) {
        // Open a file.
        stream.open(newFSPath, std::ios::in | std::ios::binary | std::ios::ate);

        // Calculate size of the file.
        size = static_cast<std::streamsize>(stream.tellg());

        // Rewind the stream.
        stream.seekg(std::ios::beg);
    } else {
        // Create a file.
        stream.open(newFSPath, std::ios::out | std::ios::binary);

        // Now it is empty file.
        size = 0;
    }
}

fc::Block fc::File::ReadBlock(const std::streamsize bytesToRead) {
    // Create storage for the block (raw bytes).
    std::array<std::uint8_t, Block::SIZE> bytes;

    // Read block (raw bytes) from the file.
    stream.read(reinterpret_cast<char*>(bytes.data()), bytesToRead);

    // Create a real block object and return it.
    return Block(std::move(bytes), static_cast<std::size_t>(bytesToRead));
}

fc::Key fc::File::ReadKey() {
    // Create storage for the key (raw bytes).
    std::array<std::uint8_t, Key::SIZE> bytes;

    // Read key (raw bytes) from the file.
    stream.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(Key::SIZE));

    // Create a real key object and return it.
    return Key(std::move(bytes));
}

void fc::File::WriteBlock(const fc::Block& block) {
    // Get a copy of the block bytes.
    const auto bytes = block.GetBytes();

    // Write these bytes to the file.
    stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(block.GetRealSize()));

    // Update file size.
    size += static_cast<std::streamsize>(block.GetRealSize());
}

void fc::File::WriteKey(const fc::Key& key) {
    // Get a copy of the key bytes.
    const auto bytes = key.GetBytes();

    // Write these bytes to the file.
    stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(Key::SIZE));

    // Update file size.
    size += static_cast<std::streamsize>(Key::SIZE);
}
