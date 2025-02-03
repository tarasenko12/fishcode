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

#include <filesystem>
#include <ios>
#include <cstddef>
#include <cstdint>
#include "block.hpp"
#include "error.hpp"
#include "file.hpp"
#include "key.hpp"

fc::InputFile::InputFile(
  const std::filesystem::path& fsPath,
  const bool isEncrypted
) {
  // Check path to the file.
  if (!std::filesystem::is_regular_file(fsPath)) {
    // Invaid input file.
    throw InvalidInputFileError();
  }

  // Open the input file.
  stream.open(fsPath, std::ios::binary | std::ios::ate);

  // Calculate size of the input file.
  const auto fileSize = static_cast<std::size_t>(stream.tellg());

  // Check file size.
  if (isEncrypted) {
    if (fileSize < Key::SIZE + 1) {
      // Invalid encrypted input file.
      throw InvalidInputFileError();
    }
  } else {
    if (fileSize < 1) {
      // Invalid input file.
      throw InvalidInputFileError();
    }
  }

  // Rewind the stream.
  stream.seekg(std::ios::beg);

  // Obtain information about the file.
  if (isEncrypted) {
    // Count number of blocks in the encrypted file.
    blocksNumber = (fileSize - Key::SIZE) / Block::CAPACITY;

    // Calculate partial block size.
    partialBlockSize = (fileSize - Key::SIZE) % Block::CAPACITY;
  } else {
    // Count number of blocks in the plaintext file.
    blocksNumber = fileSize / Block::CAPACITY;

    // Calculate partial block size.
    partialBlockSize = fileSize % Block::CAPACITY;
  }

  // Check if file contains partial block.
  if (partialBlockSize != 0) {
    // Contains.
    hasPartialBlock = true;
  } else {
    // Does not contain.
    hasPartialBlock = false;
  }
}

std::size_t fc::InputFile::GetBlocksNumber() const noexcept {
  // Return number of blocks in the file.
  return blocksNumber;
}

std::size_t fc::InputFile::GetPartialBlockSize() const noexcept {
  // Return size (in bytes) of partial block.
  return partialBlockSize;
}

bool fc::InputFile::HasPartialBlock() const noexcept {
  // Return partial block status.
  return hasPartialBlock;
}

fc::Block fc::InputFile::ReadBlock(const std::size_t bytesToRead) {
  // Create new block.
  Block newBlock;

  // Read file by bytes.
  for (std::size_t counter = 0; counter < bytesToRead; counter++) {
    // Store byte to the block.
    newBlock.PushByte(stream.get());
  }

  // Return the block.
  return newBlock;
}

fc::Key fc::InputFile::ReadKey() {
  // Create new key.
  Key newKey;

  // Read file by bytes.
  for (auto& byte : newKey) {
    // Store byte to the key.
    byte = stream.get();
  }

  // Return the key.
  return newKey;
}

fc::OutputFile::OutputFile(const std::filesystem::path& fsPath) {
  // Check path to the output file.
  if (fsPath.has_parent_path()) {
    if (!std::filesystem::exists(fsPath.parent_path())) {
      // Invalid output file.
      throw InvalidOutputFileError();
    }
  }

  // Create output file.
  stream.open(fsPath, std::ios::binary);
}

void fc::OutputFile::WriteBlock(const Block& block) {
  // Store one block to the file.
  for (const auto byte : block) {
    // Store this byte to the file.
    stream.put(byte);
  }
}

void fc::OutputFile::WriteKey(const Key& key) {
  // Store key to the file.
  for (const auto byte : key) {
    // Store this byte to the file.
    stream.put(byte);
  }
}

