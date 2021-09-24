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

static auto g_count = 0;
static auto g_futures = std::vector<std::future<void>>{};
static auto library_create_promise = std::promise<void>();
static auto library_create_completed = false;
static auto g_entries = std::vector<std::filesystem::directory_entry>{};
static auto g_mutex = std::mutex();
static void (*g_progress_callback)(int32_t, int32_t) = nullptr;

static inline int32_t LibraryCallback(void*, int argc, char** argv,
                                      char** azColName) {
  auto track = GetTrackFromCache(argv);
  g_tracks.emplace_back(track);
  return 0;
}

static inline void LibraryIndex() {
  g_count = 0;
  for (const auto& library_path : g_library_paths) {
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(library_path)) {
      g_entries.emplace_back(entry);
    }
  }
  for (const auto& entry : g_entries) {
    g_futures.emplace_back(std::async([=]() {
      auto track = GetTrackFromEntry(entry);
      g_mutex.lock();
      if (track) {
        g_tracks.emplace_back(*track);
      }
      g_count++;
      (*g_progress_callback)(g_count, g_entries.size());
      g_mutex.unlock();
      if (g_count == g_entries.size()) {
        for (const auto& track : g_tracks) {
          InsertTrack(track);
        }
        if (!library_create_completed) {
          library_create_promise.set_value();
          library_create_completed = true;
        }
      }
    }));
  }
}

static inline void LibraryIndexAlbumsArtists() {
  for (auto& track : g_tracks) {
    int32_t is_album_added = false;
    for (auto& album : g_albums) {
      if (strcmp(album.album_name, track.album_name) == 0) {
        is_album_added = true;
        album.tracks[album.tracks_size++] = track;
        break;
      }
    }
    if (!is_album_added) {
      Album album;
      strncpy(album.album_name, track.album_name, 255);
      strncpy(album.album_artist_name, track.album_artist_name, 255);
      strncpy(album.year, track.year, 255);
      album.tracks[0] = track;
      album.tracks_size = 1;
      strncpy(album.album_art_uri, track.album_art_uri, 1024);
      g_albums.emplace_back(album);
    }
  }
}

DLLEXPORT
void LibrarySetProgressCallback(void (*callback)(int32_t completed,
                                                 int32_t total)) {
  g_progress_callback = callback;
}

DLLEXPORT void LibrarySetCachePath(const char* path) {
  g_cache_path = path;
  if (!std::filesystem::exists(std::filesystem::path(g_cache_path) /
                               "album_arts")) {
    std::filesystem::create_directories(std::filesystem::path(g_cache_path) /
                                        "album_arts");
  }
}

DLLEXPORT void LibrarySetLibraryPaths(int32_t size, const char** paths) {
  g_library_paths.reserve(size);
  for (size_t index = 0; index < size; index++) {
    g_library_paths.emplace_back(std::string(paths[index]));
  }
}

DLLEXPORT void LibraryCreate() {
  auto cache_path = std::filesystem::path(g_cache_path) / "tracks.db";
  if (!std::filesystem::exists(cache_path)) {
    sqlite3_open(cache_path.string().c_str(), &g_library_cache);
    sqlite3_exec(g_library_cache, kLibraryDatabaseCreateQuery, nullptr, 0,
                 nullptr);
    LibraryIndex();
    if (!library_create_completed) {
      library_create_promise.get_future().wait();
    }
  } else {
    sqlite3_open(cache_path.string().c_str(), &g_library_cache);
    sqlite3_exec(g_library_cache, "SELECT * FROM Tracks;", LibraryCallback, 0,
                 nullptr);
    std::remove_if(g_tracks.begin(), g_tracks.end(), [](auto track) {
      auto exists =
          std::filesystem::exists(std::filesystem::path(track.file_path));
      if (!exists) {
        sqlite3_exec(g_library_cache,
                     (std::string("DELETE FROM Tracks WHERE file_path='") +
                      Strings::ReplaceAll(track.file_path, "'", "''") + "';")
                         .c_str(),
                     LibraryCallback, 0, nullptr);
      }
      return !exists;
    });
  }
  LibraryIndexAlbumsArtists();
}

DLLEXPORT int32_t LibraryGetTrackCount() { return g_tracks.size(); };

DLLEXPORT Track* LibraryGetTracks() { return g_tracks.data(); };

DLLEXPORT int32_t LibraryGetAlbumCount() { return g_albums.size(); };

DLLEXPORT Album* LibraryGetAlbums() { return g_albums.data(); };

#ifdef __cplusplus
}
#endif