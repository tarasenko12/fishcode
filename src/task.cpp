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

fc::Task::ProgressData::ProgressData() {
    // No progress for now.
    current = 0;

    // No target for now.
    total = 0;
}

// Disable task abortion (default).
std::atomic<bool> fc::taskShouldCancel(false);

void fc::TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task> task) try {
    // Calculate total number of full blocks in the file.
    task->progressData.total = (task->data.inputFile.GetSize() - Key::SIZE) / Block::SIZE;

    // Read decryption (encrypted) key from the input file.
    task->data.key = task->data.inputFile.ReadKey();

    // Decrypt the key.
    task->data.key.Decrypt(task->data.password);

    // Decrypt the input file by blocks.
    while (task->progressData.current < task->progressData.total) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Remove output file (user doesn't need it).
            task->data.outputFile.Remove();

            // Terminate the thread.
            return;
        }

        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock(static_cast<std::streamsize>(Block::SIZE));

        // Decrypt the block.
        block.Decrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Update current progress.
        task->progressData.current++;

        // Send a message about progress update (if it isn't the last block).
        if (task->progressData.current < task->progressData.total) {
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_FRAME,
                (task->progressData.current * 100) / task->progressData.total)
            );
        }
    }

    // Check if there is a partial block.
    if ((task->data.inputFile.GetSize() - Key::SIZE) % Block::SIZE != 0 && !taskShouldCancel) {
        // Calculate size of the partial block.
        const auto blockSize = (task->data.inputFile.GetSize() - Key::SIZE) % Block::SIZE;

        // Read this block from the file.
        auto block = task->data.inputFile.ReadBlock(static_cast<std::streamsize>(blockSize));

        // Decrypt the block.
        block.Decrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);
    }

    // Check for task abortion.
    if (!taskShouldCancel) {
        // Notify the main thread about task completition.
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_FRAME));
    } else {
        // Remove output file (user doesn't need it).
        task->data.outputFile.Remove();
    }
} catch (const std::exception& ex) {
    // Notify main thread about exception in the task thread.
    wxPostEvent(sink, events::TaskException(events::ID_FRAME, ex));

    // Terminate the thread.
    return;
}

void fc::TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task> task) try {
    // Calculate total number of full blocks in the file.
    task->progressData.total = task->data.inputFile.GetSize() / Block::SIZE;

    // Generate encryption key.
    task->data.key = Key::Generate();

    // Encrypt the key.
    task->data.key.Encrypt(task->data.password);

    // Write key (encrypted) to the output file.
    task->data.outputFile.WriteKey(task->data.key);

    // Decrypt the key.
    task->data.key.Decrypt(task->data.password);

    // Decrypt the input file by blocks.
    while (task->progressData.current < task->progressData.total) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Remove output file (user doesn't need it).
            task->data.outputFile.Remove();

            // Terminate the thread.
            return;
        }

        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock(static_cast<std::streamsize>(Block::SIZE));

        // Encrypt the block.
        block.Encrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Update current progress.
        task->progressData.current++;

        // Send a message about progress update (if it isn't the last block).
        if (task->progressData.current < task->progressData.total) {
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_FRAME,
                (task->progressData.current * 100) / task->progressData.total)
            );
        }
    }

    // Check if there is a partial block.
    if (task->data.inputFile.GetSize() % Block::SIZE != 0 && !taskShouldCancel) {
        // Calculate size of the partial block.
        const auto blockSize = task->data.inputFile.GetSize() % Block::SIZE;

        // Read this block from the file.
        auto block = task->data.inputFile.ReadBlock(static_cast<std::streamsize>(blockSize));

        // Encrypt the block.
        block.Encrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);
    }

    // Check for task abortion.
    if (!taskShouldCancel) {
        // Notify the main thread about task completition.
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_FRAME));
    } else {
        // Remove output file (user doesn't need it).
        task->data.outputFile.Remove();
    }
} catch (const std::exception& ex) {
    // Notify main thread about exception in the task thread.
    wxPostEvent(sink, events::TaskException(events::ID_FRAME, ex));

    // Terminate the thread.
    return;
}
