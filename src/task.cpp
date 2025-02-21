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

#include <atomic>
#include <exception>
#include <ios>
#include <memory>
#include <wx/event.h>
#include "block.hpp"
#include "events.hpp"
#include "key.hpp"
#include "task.hpp"

// Disable task abortion (default).
std::atomic<bool> fc::taskShouldCancel(false);

void fc::TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<fc::TaskData> data) try {
    // Obtain user data.
    auto& inputFile = data->GetInputFile();
    auto& outputFile = data->GetOutputFile();
    const auto& password = data->GetPassword();

    // Calculate total number of full blocks in the input file.
    const auto total = (inputFile.GetSize() - Key::SIZE) / Block::SIZE;

    // Calculate number of partial blocks in the input file.
    const auto partial = (inputFile.GetSize() - Key::SIZE) % Block::SIZE;

    // Read decryption (encrypted) key from the input file.
    auto key = inputFile.ReadKey();

    // Decrypt the key.
    key.Decrypt(password);

    // Calculate 1% of blocks in the input file.
    const auto onePercent = total / 100;

    // Decrypt the input file by blocks.
    for (std::size_t current = 0; current < total; current++) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Remove output file (user doesn't need it).
            outputFile.Remove();

            // Terminate the thread.
            return;
        }

        // Read one block from the file.
        auto block = inputFile.ReadBlock(static_cast<std::streamsize>(Block::SIZE));

        // Decrypt the block.
        block.Decrypt(key);

        // Store block to the output file.
        outputFile.WriteBlock(block);

        // Calculate current percentage of task completition.
        const int percent = (current * 100) / total;

        // Check if there is a valuable progress.
        if (onePercent > 0) {
            if (current % onePercent == 0) {
                // Send a message about progress update.
                wxPostEvent(sink, events::UpdateProgress(fc::events::ID_FRAME, percent));
            }
        }
    }

    // Check if there is a partial block.
    if (partial != 0 && !taskShouldCancel) {
        // Read this block from the file.
        auto block = inputFile.ReadBlock(static_cast<std::streamsize>(partial));

        // Decrypt the block.
        block.Decrypt(key);

        // Store block to the output file.
        outputFile.WriteBlock(block);
    }

    // Check for task abortion.
    if (!taskShouldCancel) {
        // Notify the main thread about task completition.
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_FRAME));
    } else {
        // Remove output file (user doesn't need it).
        outputFile.Remove();
    }
} catch (const std::exception& ex) {
    // Notify main thread about exception in the task thread.
    wxPostEvent(sink, events::TaskException(events::ID_FRAME, ex));
}

void fc::TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<fc::TaskData> data) try {
    // Obtain user data.
    auto& inputFile = data->GetInputFile();
    auto& outputFile = data->GetOutputFile();
    const auto& password = data->GetPassword();

    // Calculate total number of full blocks in the input file.
    const auto total = inputFile.GetSize() / Block::SIZE;

    // Calculate number of partial blocks in the input file.
    const auto partial = inputFile.GetSize() % Block::SIZE;

    // Generate encryption key.
    auto key = Key::Generate();

    // Encrypt the key.
    key.Encrypt(password);

    // Write decryption (encrypted) key to the output file.
    outputFile.WriteKey(key);

    // Decrypt the key.
    key.Decrypt(password);

    // Calculate 1% of blocks in the input file.
    const auto onePercent = total / 100;

    // Encrypt the input file by blocks.
    for (std::size_t current = 0; current < total; current++) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Remove output file (user doesn't need it).
            outputFile.Remove();

            // Terminate the thread.
            return;
        }

        // Read one block from the file.
        auto block = inputFile.ReadBlock(static_cast<std::streamsize>(Block::SIZE));

        // Encrypt the block.
        block.Encrypt(key);

        // Store block to the output file.
        outputFile.WriteBlock(block);

        // Calculate current percentage of task completition.
        const int percent = (current * 100) / total;

        // Check if there is a valuable progress.
        if (onePercent > 0) {
            if (current % onePercent == 0) {
                // Send a message about progress update.
                wxPostEvent(sink, events::UpdateProgress(fc::events::ID_FRAME, percent));
            }
        }
    }

    // Check if there is a partial block.
    if (partial != 0 && !taskShouldCancel) {
        // Read this block from the file.
        auto block = inputFile.ReadBlock(static_cast<std::streamsize>(partial));

        // Encrypt the block.
        block.Encrypt(key);

        // Store block to the output file.
        outputFile.WriteBlock(block);
    }

    // Check for task abortion.
    if (!taskShouldCancel) {
        // Notify the main thread about task completition.
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_FRAME));
    } else {
        // Remove output file (user doesn't need it).
        outputFile.Remove();
    }
} catch (const std::exception& ex) {
    // Notify main thread about exception in the task thread.
    wxPostEvent(sink, events::TaskException(events::ID_FRAME, ex));
}
