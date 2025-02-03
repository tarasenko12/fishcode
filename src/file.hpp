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

#ifndef FISHCODE_FILE_HPP
#define FISHCODE_FILE_HPP

#include <filesystem>
#include <fstream>
#include <cstddef>
#include <cstdint>
#include "block.hpp"
#include "key.hpp"

namespace fc {
  class InputFile {
  public:
    InputFile() = default;
    InputFile(const std::filesystem::path& fsPath, const bool isEncrypted);
    InputFile(const InputFile& anotherFile) = delete;
    InputFile(InputFile&& anotherFile) = default;

    ~InputFile() noexcept = default;

    InputFile& operator=(const InputFile& anotherFile) = delete;
    InputFile& operator=(InputFile&& anotherFile) = default;

    std::size_t GetBlocksNumber() const noexcept;
    std::size_t GetPartialBlockSize() const noexcept;
    bool HasPartialBlock() const noexcept;
    Block ReadBlock(const std::size_t bytesToRead = Block::CAPACITY);
    Key ReadKey();
  private:
    std::ifstream stream;
    std::size_t blocksNumber;
    std::size_t partialBlockSize;
    bool hasPartialBlock;
  };

  class OutputFile {
  public:
    OutputFile() = default;
    OutputFile(const std::filesystem::path& fsPath);
    OutputFile(const OutputFile& anotherFile) = delete;
    OutputFile(OutputFile&& anotherFile) = default;

    ~OutputFile() noexcept = default;

    OutputFile& operator=(const OutputFile& anotherFile) = delete;
    OutputFile& operator=(OutputFile&& anotherFile) = default;

    void WriteBlock(const Block& block);
    void WriteKey(const Key& key);
  private:
    std::ofstream stream;
  };
}

#endif // FISHCODE_FILE_HPP

