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
#include <memory>
#include <cstddef>
#include <wx/event.h>
#include "block.hpp"
#include "events.hpp"
#include "task.hpp"

fc::Task::ProgressData::ProgressData() {
    // No progress for now.
    current = 0;

    // No target for now.
    total = 0;
}

// Disable task abortion (default).
std::atomic<bool> fc::taskShouldCancel(false);

void fc::TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task>&& task) {
    // Calculate total number of full blocks in the file.
    task->progressData.total = task->data.inputFile.GetSize() / Block::SIZE;

    // Read decryption (encrypted) key from the file.
    task->data.key = task->data.inputFile.ReadKey();

    // Decrypt the key using password.
    task->data.key.Decrypt(task->data.password);

    // Decrypt the input file by blocks.
    for (std::size_t counter = 0; counter < task->progressData.total; counter++, task->progressData.current++) {
        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock();

        // Decrypt the block.
        block.Decrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Stop the loop.
            break;
        } else {
            // Notify the main thread about UI update.
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_PROGRESS,
                task->progressData.current / task->progressData.total * 100)
            );
        }
    }

    // Check if file contains a partial block (realSize < SIZE).
    if (task->data.inputFile.GetSize() % Block::SIZE != 0 && !taskShouldCancel) {
        // Calculate block size.
        const auto blockSize = task->data.inputFile.GetSize() % Block::SIZE;

        // Read block from the file.
        auto block = task->data.inputFile.ReadBlock(blockSize);

        // Decrypt the block.
        block.Decrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Set progress to 100%, if it is not already set.
        if (task->progressData.current == 0) {
            // Notify the main thread about UI update.
            wxPostEvent(sink, events::UpdateProgress(fc::events::ID_PROGRESS, 100));
        }
    }

    // Notify the main thread about task completition (if it is not aborted).
    if (!taskShouldCancel) {
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_DONE));
    }
}

void fc::TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task>&& task) {
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

    // Encrypt the input file by blocks.
    for (std::size_t counter = 0; counter < task->progressData.total; counter++, task->progressData.current++) {
        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock();

        // Encrypt the block.
        block.Encrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Stop the loop.
            break;
        } else {
            // Notify the main thread about UI update.
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_PROGRESS,
                task->progressData.current / task->progressData.total * 100)
            );
        }
    }

    // Check if file contains a partial block (realSize < SIZE).
    if (task->data.inputFile.GetSize() % Block::SIZE != 0 && !taskShouldCancel) {
        // Calculate block size.
        const auto blockSize = task->data.inputFile.GetSize() % Block::SIZE;

        // Read block from the file.
        auto block = task->data.inputFile.ReadBlock(blockSize);

        // Encrypt the block.
        block.Encrypt(task->data.key);

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Set progress to 100%, if it is not already set.
        if (task->progressData.current == 0) {
            // Notify the main thread about UI update.
            wxPostEvent(sink, events::UpdateProgress(fc::events::ID_PROGRESS, 100));
        }
    }

    // Notify the main thread about task completition (if it is not aborted).
    if (!taskShouldCancel) {
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_DONE));
    }
}
