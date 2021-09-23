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
#include "base.h"
#include "library.h"
#include "retriever.h"
#include "library_utils.h"

DLLEXPORT void LibrarySetCachePath(const char* path) {
  g_cache_path = path;
  if (!std::filesystem::exists(g_cache_path)) {
    std::filesystem::create_directory(g_cache_path);
  }
}

DLLEXPORT void LibrarySetLibraryPaths(int32_t size, const char** paths) {
  g_library_paths.reserve(size);
  for (size_t index = 0; index < size; index++) {
    g_library_paths.emplace_back(std::string(paths[index]));
  }
}

DLLEXPORT void LibraryIndex(void (*callback)(int32_t completed,
                                             int32_t total)) {
  // TODO (alexmercerind): Free objects.
  auto count = new int32_t[1];
  auto futures = new std::vector<std::future<void>>();
  auto entries = new std::vector<std::filesystem::directory_entry>();
  *count = 0;
  for (const auto& library_path : g_library_paths) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(library_path)) {
      entries->emplace_back(entry);
    }
  }
  for (const auto& entry : *entries) {
    futures->emplace_back(std::async([=]() {
      auto track = GetTrack(entry);
      (*count)++;
      if (track.has_value()) {
      }
      (*callback)(*count, entries->size());
    }));
  }
}
