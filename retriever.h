/*
 * This file is part of the musiclibgen
 * (https://github.com/alexmercerind/musiclibgen).
 *
 * Copyright (c) 2021 Hitesh Kumar Saini <saini123hitesh@gmail.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RETRIEVER_H_
#define RETRIEVER_H_

#include "constants.h"

#include "MediaInfoDLL/MediaInfoDLL.h"

class Retriever : public MediaInfoDLL::MediaInfo {
 public:
  Retriever();

  std::unordered_map<std::string, std::string>* metadata() const {
    return metadata_.get();
  }
  std::vector<uint8_t>* album_art() const { return album_art_.get(); }

  void SetFilePath(std::string file_path);

  ~Retriever();

 private:
  std::unique_ptr<std::unordered_map<std::string, std::string>> metadata_ =
      std::make_unique<std::unordered_map<std::string, std::string>>();
  std::unique_ptr<std::vector<uint8_t>> album_art_ = nullptr;
};

#endif
