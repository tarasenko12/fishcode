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

#ifndef FISHCODE_FILE_HPP
#define FISHCODE_FILE_HPP

#include <filesystem>
#include <fstream>
#include <ios>
#include "block.hpp"
#include "key.hpp"

namespace fc {
    enum class FileType {
        FT_INPUT,
        FT_OUTPUT
    };

    class File {
    public:
        File() = default;
        File(const std::filesystem::path& fsPath, const FileType type);
        File(const File& anotherFile) = delete;
        File(File&& anotherFile) noexcept = default;

        ~File() noexcept = default;

        File& operator=(const File& anotherFile) = delete;
        File& operator=(File&& anotherFile) noexcept = default;

        inline std::streamsize GetSize() const noexcept {
            return size;
        }

        Block ReadBlock(const std::streamsize bytesToRead = Block::SIZE);
        Key ReadKey();
        void WriteBlock(const Block& block);
        void WriteKey(const Key& key);
    private:
        std::streamsize size;
        std::fstream stream;
    };
}

#endif // FISHCODE_FILE_HPP
