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
#include <filesystem>
#include <memory>
#include <cstddef>
#include <wx/event.h>
#include "file.hpp"
#include "key.hpp"
#include "password.hpp"

namespace fc {
    class Task {
        friend void TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<Task> task);
        friend void TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<Task> task);
    public:
        Task() = default;
        Task(const Task& otherTask) = delete;
        Task(Task&& otherTask) noexcept = default;

        ~Task() noexcept = default;

        Task& operator=(const Task& otherTask) = delete;
        Task& operator=(Task&& otherTask) noexcept = default;

        inline void SetInputFile(const std::filesystem::path& ifPath) {
            data.inputFile = File(ifPath, FileType::FT_INPUT);
        }

        inline void SetOutputFile(const std::filesystem::path& ofPath) {
            data.outputFile = File(ofPath, FileType::FT_OUTPUT);
        }

        inline void SetPassword(const std::string& passwordString) {
            data.password = Password(passwordString);
        }
    private:
        struct Data {
            File inputFile, outputFile;
            Key key;
            Password password;

            Data() = default;
            Data(const Data& otherData) = delete;
            Data(Data&& otherData) noexcept = default;

            ~Data() noexcept = default;

            Data& operator=(const Data& otherData) = default;
            Data& operator=(Data&& otherData) noexcept = default;
        } data;

        struct ProgressData {
            std::size_t current, total;

            ProgressData();
            ProgressData(const ProgressData& otherProgressData) = delete;
            ProgressData(ProgressData&& otherProgressData) noexcept = default;

            ~ProgressData() noexcept = default;

            ProgressData& operator=(const ProgressData& otherProgressData) = default;
            ProgressData& operator=(ProgressData&& otherProgressData) noexcept = default;
        } progressData;
    };

    extern std::atomic<bool> taskShouldCancel;

    void TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<Task> task);
    void TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<Task> task);
}

#endif // FISHCODE_TASK_HPP
