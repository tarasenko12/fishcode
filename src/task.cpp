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

void fc::TaskDecrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task> task) {
    // Calculate total number of full blocks in the file.
    task->progressData.total = (task->data.inputFile.GetSize() - Key::SIZE) / Block::SIZE;

    // By default file doesn't contain partial block.
    bool hasPartial = false;

    // Check if there is a partial block.
    if ((task->data.inputFile.GetSize() - Key::SIZE) % Block::SIZE != 0) {
        // Yes, there is a partial block.
        hasPartial = true;

        // Update target progress to count the partial block.
        task->progressData.total++;
    }

    // Read encryption key from the input file.
    task->data.key = task->data.inputFile.ReadKey();

    // Check for task abortion.
    if (taskShouldCancel) {
        // Terminate the thread.
        return;
    }

    // Decrypt the key.
    task->data.key.Decrypt(task->data.password);

    // Decrypt the input file by blocks.
    while (task->progressData.current < task->progressData.total) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Set block size to default value.
        auto blockSize = Block::SIZE;

        // Check if it is not a partial block.
        if (hasPartial && task->progressData.current == task->progressData.total - 1) {
            // Calculate size of the partial block.
            blockSize = (task->data.inputFile.GetSize() - Key::SIZE) % Block::SIZE;
        }

        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock(blockSize);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Decrypt the block.
        block.Decrypt(task->data.key);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Update current progress.
        task->progressData.current++;

        // Send a message about progress update (if it isn't the last block).
        if (task->progressData.current < task->progressData.total) {
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_PROGRESS,
                (task->progressData.current * 100) / task->progressData.total)
            );
        }
    }

    // Notify the main thread about task completition (if it is not aborted).
    if (!taskShouldCancel) {
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_DONE));
    }
}

void fc::TaskEncrypt(wxEvtHandler* sink, std::unique_ptr<fc::Task> task) {
    // Calculate total number of full blocks in the file.
    task->progressData.total = task->data.inputFile.GetSize() / Block::SIZE;

    // By default file doesn't contain partial block.
    bool hasPartial = false;

    // Check if there is a partial block.
    if (task->data.inputFile.GetSize() % Block::SIZE != 0) {
        // Yes, there is a partial block.
        hasPartial = true;

        // Update target progress to count the partial block.
        task->progressData.total++;
    }

    // Generate encryption key.
    task->data.key = Key::Generate();

    // Check for task abortion.
    if (taskShouldCancel) {
        // Terminate the thread.
        return;
    }

    // Encrypt the key.
    task->data.key.Encrypt(task->data.password);

    // Check for task abortion.
    if (taskShouldCancel) {
        // Terminate the thread.
        return;
    }

    // Write key (encrypted) to the output file.
    task->data.outputFile.WriteKey(task->data.key);

    // Check for task abortion.
    if (taskShouldCancel) {
        // Terminate the thread.
        return;
    }

    // Decrypt the key.
    task->data.key.Decrypt(task->data.password);

    // Encrypt the input file by blocks.
    while (task->progressData.current < task->progressData.total) {
        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Set block size to default value.
        auto blockSize = Block::SIZE;

        // Check if it is not a partial block.
        if (hasPartial && task->progressData.current == task->progressData.total - 1) {
            // Calculate size of the partial block.
            blockSize = task->data.inputFile.GetSize() % Block::SIZE;
        }

        // Read one block from the file.
        auto block = task->data.inputFile.ReadBlock(blockSize);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Encrypt the block.
        block.Encrypt(task->data.key);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Store block to the output file.
        task->data.outputFile.WriteBlock(block);

        // Check for task abortion.
        if (taskShouldCancel) {
            // Terminate the thread.
            return;
        }

        // Update current progress.
        task->progressData.current++;

        // Send a message about progress update (if it isn't the last block).
        if (task->progressData.current < task->progressData.total) {
            wxPostEvent(sink, events::UpdateProgress(
                fc::events::ID_PROGRESS,
                (task->progressData.current * 100) / task->progressData.total)
            );
        }
    }

    // Notify the main thread about task completition (if it is not aborted).
    if (!taskShouldCancel) {
        wxPostEvent(sink, events::UpdateDone(fc::events::ID_DONE));
    }
}
