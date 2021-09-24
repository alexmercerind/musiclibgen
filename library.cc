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

#ifdef __cplusplus
extern "C" {
#endif

int32_t g_count = 0;
auto g_futures = std::vector<std::future<void>>{};
auto g_entries = std::vector<std::filesystem::directory_entry>{};
auto g_mutex = std::mutex();

static int LibraryCallback(void* data, int argc, char** argv,
                           char** azColName) {
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void LibrarySetCachePath(const char* path) {
  g_cache_path = path;
  if (!std::filesystem::exists(std::filesystem::path(g_cache_path) /
                               "album_arts")) {
    std::filesystem::create_directories(std::filesystem::path(g_cache_path) /
                                        "album_arts");
  }
  if (g_library_database == nullptr) {
    auto database_path = std::filesystem::path(path) / "tracks.db";
    sqlite3_open(database_path.string().c_str(), &g_library_database);
    if (std::filesystem::exists(database_path.string())) {
      sqlite3_exec(g_library_database, kLibraryDatabaseCreateQuery, nullptr, 0,
                   nullptr);
    }
  }
}

void LibrarySetLibraryPaths(int32_t size, const char** paths) {
  g_library_paths.reserve(size);
  for (size_t index = 0; index < size; index++) {
    g_library_paths.emplace_back(std::string(paths[index]));
  }
}

void LibraryIndex(void (*callback)(int32_t completed, int32_t total)) {
  g_count = 0;
  for (const auto& library_path : g_library_paths) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(library_path)) {
      g_entries.emplace_back(entry);
    }
  }
  for (const auto& entry : g_entries) {
    g_futures.emplace_back(std::async([=]() {
      auto track = GetTrack(entry);
      g_mutex.lock();
      if (track) {
        g_tracks.emplace_back(*track);
      }
      g_count++;
      (*callback)(g_count, g_entries.size());
      g_mutex.unlock();
      if (g_count == g_entries.size()) {
        for (const auto& track : g_tracks) {
          InsertTrack(track);
        }
        sqlite3_exec(g_library_database, "SELECT * FROM Tracks;",
                     LibraryCallback, 0, nullptr);
      }
    }));
  }
}

#ifdef __cplusplus
}
#endif