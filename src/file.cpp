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

#include <filesystem>
#include <ios>
#include <cstddef>
#include "block.hpp"
#include "file.hpp"

using std::ios;
using std::filesystem::path;
using std::size_t;

fc::File::File(const path& fsPath) {
    // Create a file.
    stream.open(fsPath, ios::out | ios::binary);

    // Now it is empty file.
    size = 0;

    // Configure offset.
    offset = 0;
}

fc::File::File(const path& fsPath, const bool isEncrypted) {
    // Open the file.
    stream.open(fsPath, ios::in | ios::binary | ios::ate);

    // Calculate size of the file.
    size = static_cast<size_t>(stream.tellg());

    // Rewind the stream.
    stream.seekg(ios::beg);

    // Configure offset.
    offset = 0;
}

fc::Block fc::File::ReadBlock(const size_t bytesToRead) {
    // Create new block.
    Block newBlock;

    // Read file by bytes.
    for (size_t counter = 0; counter < bytesToRead && offset < size; counter++, offset++) {
        // Store byte to the block.
        newBlock.Push(stream.get());
    }

    // Return the block.
    return newBlock;
}

void fc::File::WriteBlock(const fc::Block& block) {
    // Get real size of the block.
    const auto realSize = block.GetRealSize();

    // Store block to the file.
    for (size_t counter = 0; counter < realSize; counter++, size++) {
        // Store this byte to the file.
        stream.put(block[counter]);
    }
}
