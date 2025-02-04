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

#include "task.hpp"

fc::Task::ProgressData::ProgressData() {
    // No progress for now.
    current = 0;

    // No target for now.
    total = 0;
}

// TODO
void fc::FishCode::Encrypt(wxFrame* sink, fc::FishCode::Data&& data) {
  // Calculate total number of full blocks in the file.
  const auto total = data.inputFile.GetBlocksNumber();

  // Calculate number of blocks for one percent.
  const auto onePercent = total / 100;

  // Create progress counter (blocks).
  std::size_t counter = 0;

  // Decrypt the input file by blocks.
  while (counter < total && !shouldCancel) {
    // Read one block from the file.
    auto block = data.inputFile.ReadBlock();

    // Encrypt the block.
    block.Encrypt(data.key);

    // Store block to the output file.
    data.outputFile.WriteBlock(block);

    // Increment progress counter by 1 block.
    counter++;

    // Check if there is a valuable progress.
    if (counter % onePercent == 0) {
      // Increment progress counter by 1%.
      progress++;

      // Notify the main thread about UI update.
      wxPostEvent(sink, wxCommandEvent(wxEVT_UPDATE_UI, ID_UPDATE_PROGRESS));
    }
  }

  // Check if the input file has partial block.
  if (data.inputFile.HasPartialBlock() && !shouldCancel) {
    // Read partial block.
    auto block = data.inputFile.ReadBlock(data.inputFile.GetPartialBlockSize());

    // Encrypt the block.
    block.Encrypt(data.key);

    // Store block to the output file.
    data.outputFile.WriteBlock(block);
  }

  // Notify the main thread about task completition (if it is not aborted).
  if (!shouldCancel) {
    wxPostEvent(sink, wxCommandEvent(wxEVT_UPDATE_UI, ID_UPDATE_DONE));
  }
}
// TODO
void fc::FishCode::Decrypt(wxFrame* sink, fc::FishCode::Data&& data) {
  // Calculate total number of full blocks in the file.
  const auto total = data.inputFile.GetBlocksNumber();

  // Calculate number of blocks for one percent.
  const auto onePercent = total / 100;

  // Create progress counter (blocks).
  std::size_t counter = 0;

  // Decrypt the input file by blocks.
  while (counter < total && !shouldCancel) {
    // Read one block from the file.
    auto block = data.inputFile.ReadBlock();

    // Decrypt the block.
    block.Decrypt(data.key);

    // Store block to the output file.
    data.outputFile.WriteBlock(block);

    // Increment progress counter by 1 block.
    counter++;

    // Check if there is a valuable progress.
    if (counter % onePercent == 0) {
      // Increment progress counter by 1%.
      progress++;

      // Notify the main thread about UI update.
      wxPostEvent(sink, wxCommandEvent(wxEVT_UPDATE_UI, ID_UPDATE_PROGRESS));
    }
  }

  // Check if the input file has partial block.
  if (data.inputFile.HasPartialBlock() && !shouldCancel) {
    // Read partial block.
    auto block = data.inputFile.ReadBlock(data.inputFile.GetPartialBlockSize());

    // Decrypt the block.
    block.Decrypt(data.key);

    // Store block to the output file.
    data.outputFile.WriteBlock(block);
  }

  // Notify the main thread about task completition (if it is not aborted).
  if (!shouldCancel) {
    wxPostEvent(sink, wxCommandEvent(wxEVT_UPDATE_UI, ID_UPDATE_DONE));
  }
}
