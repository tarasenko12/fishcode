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
#include <wx/event.h>
#include "file.hpp"
#include "password.hpp"

namespace fc {
    class TaskData {
    public:
        TaskData() = default;
        TaskData(const TaskData& otherTaskData) = delete;
        TaskData(TaskData&& otherTaskData) noexcept = default;

        ~TaskData() noexcept = default;

        TaskData& operator=(const TaskData& otherTaskData) = delete;
        TaskData& operator=(TaskData&& otherTaskData) noexcept = default;

        inline File& GetInputFile() noexcept {
            return inputFile;
        }

        inline File& GetOutputFile() noexcept {
            return outputFile;
        }

        inline const Password& GetPassword() const noexcept {
            return password;
        }

        inline void SetInputFile(const std::filesystem::path& ifPath) {
            // Open the file.
            inputFile = File(ifPath, FileType::FT_INPUT);
        }

        inline void SetOutputFile(const std::filesystem::path& ofPath) {
            // Create a file.
            outputFile = File(ofPath, FileType::FT_OUTPUT);
        }

        inline void SetPassword(const std::string& passwordString) {
            // Convert and copy password.
            password = Password(passwordString);
        }
    private:
        File inputFile, outputFile;
        Password password;
    };

    extern std::atomic<bool> taskShouldCancel;

    void TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<TaskData> data);
    void TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<TaskData> data);
}

#endif // FISHCODE_TASK_HPP
