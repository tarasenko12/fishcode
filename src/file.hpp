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
#include <cstddef>
#include "block.hpp"

namespace fc {
    class File {
        using namespace std;
        using namespace std::filesystem;
    public:
        File() = default;
        File(const path& fsPath);
        File(const path& fsPath, const bool isEncrypted);
        File(const File& anotherFile) = delete;
        File(File&& anotherFile) = default;

        ~File() noexcept = default;

        File& operator=(const File& anotherFile) = delete;
        File& operator=(File&& anotherFile) = default;

        size_t GetSize() const noexcept;
        Block ReadBlock(const size_t bytesToRead = Block::SIZE);
        void WriteBlock(const Block& block);
    private:
        fstream stream;
        size_t size, offset;
    };
}

#endif // FISHCODE_FILE_HPP
