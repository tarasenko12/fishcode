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

#ifndef FISHCODE_TASK_HPP
#define FISHCODE_TASK_HPP

#include <cstddef>
#include "block.hpp"
#include "file.hpp"
#include "password.hpp"

namespace fc {
    class Task {
    public:
        Task() = default;
        Task(const Task& otherTask) = default;
        Task(Task&& otherTask) = default;

        ~Task() noexcept = default;

        Task& operator=(const Task& otherTask) = default;
        Task& operator=(Task&& otherTask) = default;

        inline File& GetInputFile() const noexcept {
            return data.inputFile;
        }

        inline File& GetOutputFile() const noexcept {
            return data.outputFile;
        }

        inline Block GetKey() const {
            return data.key;
        }

        inline Password GetPassword() const {
            return data.password;
        }

        inline std::size_t GetCurrentProgress() const noexcept {
            return progressData.current;
        }

        inline std::size_t GetTotalProgress() const noexcept {
            return progressData.total;
        }

        inline void SetInputFile(File&& newInputFile) {
            data.inputFile = newInputFile;
        }

        inline void SetOutputFile(File&& newOutputFile) {
            data.outputFile = newOutputFile;
        }

        inline void SetKey(const Block& newKey) {
            data.key = newKey;
        }

        inline void SetPassword(const Password& newPassword) {
            data.password = newPassword;
        }

        inline void SetCurrentProgress(const std::size_t newCurrent) {
            progressData.current = newCurrent;
        }

        inline void SetTotalProgress(const std::size_t newTotal) {
            progressData.total = newTotal;
        }
    private:
        struct Data {
            File inputFile, outputFile;
            Block key;
            Password password;

            Data() = default;
            Data(const Data& otherData) = default;
            Data(Data&& otherData) = default;

            ~Data() noexcept = default;

            Data& operator=(const Data& otherData) = default;
            Data& operator=(Data&& otherData) = default;
        } data;

        struct ProgressData {
            std::size_t current, total;

            ProgressData();
            ProgressData(const ProgressData& otherProgressData) = default;
            ProgressData(ProgressData&& otherProgressData) = default;

            ~ProgressData() noexcept = default;

            ProgressData& operator=(const ProgressData& otherProgressData) = default;
            ProgressData& operator=(ProgressData&& otherProgressData) = default;
        } progressData;
    };

    void TaskEncrypt(wxEvtHandler* sink, Task&& taskData);
    void TaskDecrypt(wxEvtHandler* sink, Task&& taskData);
}

#endif // FISHCODE_TASK_HPP
