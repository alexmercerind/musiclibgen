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
#include "base64.h"

#include "base.h"
#include "retriever.h"
#include "utils.h"

Retriever::Retriever() { Option(L"Cover_Data", L"base64"); }

void Retriever::SetFilePath(std::string file_path) {
  Open(TO_WIDESTRING(file_path));
  for (auto & [ property, key ] : kMetadataKeys) {
    std::string value = TO_STRING(Get(MediaInfoDLL::Stream_General, 0, key));
    metadata_->insert(std::make_pair(property, value));
  }
  metadata_->insert(std::make_pair("filePath", file_path));
  try {
    if (Get(MediaInfoDLL::Stream_General, 0, L"Cover") == L"Yes") {
      std::vector<uint8_t> decoded_album_art = Base64Decode(
          TO_STRING(Get(MediaInfoDLL::Stream_General, 0, L"Cover_Data")));
      album_art_.reset(new std::vector<uint8_t>(decoded_album_art));
      auto format = TO_STRING(Get(MediaInfoDLL::Stream_General, 0, L"Format"));
      if (Strings::ToUpperCase(format) == "OGG") {
        uint8_t* data = decoded_album_art.data();
        size_t size = decoded_album_art.size();
        size_t header = 0;
        uint32_t length = 0;
        RM(4);
        length = U32_AT(data);
        header += length;
        RM(4);
        RM(length);
        length = U32_AT(data);
        header += length;
        RM(4);
        RM(length);
        RM(4 * 4);
        length = U32_AT(data);
        RM(4);
        header += 32;
        size = length;
        album_art_.reset(new std::vector(data, data + length));
      }
    } else {
      album_art_ = nullptr;
    }
  } catch (...) {
    album_art_ = nullptr;
  }
}

Retriever::~Retriever() {}
