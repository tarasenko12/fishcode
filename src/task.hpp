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
**
** This program uses wxWidgets, a free and open-source cross-platform C++
** library for creating GUIs. wxWidgets is licensed under the wxWindows
** Library License, which is compatible with the GNU GPL.
** See <https://www.wxwidgets.org/about/licence/>.
*/

#ifndef FISHCODE_TASK_HPP
#define FISHCODE_TASK_HPP

#include <atomic>
#include <memory>
#include <utility>
#include <cstddef>
#include <wx/event.h>
#include "block.hpp"
#include "file.hpp"
#include "password.hpp"

namespace fc {
    class Task {
        friend void TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<Task>&& task);
        friend void TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<Task>&& task);
    public:
        Task() = default;
        Task(const Task& otherTask) = default;
        Task(Task&& otherTask) = default;

        ~Task() noexcept = default;

        Task& operator=(const Task& otherTask) = default;
        Task& operator=(Task&& otherTask) = default;

        inline void SetInputFile(File&& newInputFile) {
            data.inputFile = std::move(newInputFile);
        }

        inline void SetOutputFile(File&& newOutputFile) {
            data.outputFile = std::move(newOutputFile);
        }

        inline void SetPassword(const Password& newPassword) {
            data.password = newPassword;
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

    extern std::atomic<bool> taskShouldCancel;

    void TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<Task>&& task);
    void TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<Task>&& task);
}

#endif // FISHCODE_TASK_HPP
