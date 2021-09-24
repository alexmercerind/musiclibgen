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
#ifndef LIBRARY_UTILS_H_
#define LIBRARY_UTILS_H_

#include "base.h"
#include "constants.h"
#include "retriever.h"
#include "structs.h"
#include "utils.h"

static inline void InsertTrack(Track track) {
  char* track_insert_error;
  std::string query =
      std::string("INSERT INTO Tracks VALUES('") + track.track_name + "', '" +
      track.track_artist_names + "', '" + track.album_name + "', '" +
      track.album_artist_name + "', '" + track.track_number + "', '" +
      track.album_length + "', '" + track.year + "', '" + track.genre + "', '" +
      track.writer_name + "', '" + track.track_duration + "', '" +
      track.bitrate + "', '" + track.album_art_uri + "', '" +
      Strings::ReplaceAll(track.file_path, "'", "''") + "', '" +
      std::to_string(track.file_size) + "', '" +
      std::to_string(track.file_time) + "');";
  if (sqlite3_exec(g_library_cache, query.c_str(), nullptr, 0,
                   &track_insert_error) != S_OK) {
    DEBUG_LOG(track_insert_error);
    DEBUG_LOG(track.file_path);
  }
}

static inline std::string GetAlbumArtPath(std::filesystem::path file_path) {
  auto file_name = file_path.filename().string();
  file_name.erase(std::remove_if(file_name.begin(), file_name.end(),
                                 [](char c) { return !std::isalnum(c); }),
                  file_name.end());
  return (std::filesystem::path(g_cache_path) / "album_arts" / file_name)
             .string() +
         ".jpg";
}

static inline Track GetTrackFromCache(char** argv) {
  Track track;
  strncpy(track.track_name, argv[0], 255);
  strncpy(track.track_artist_names, argv[1], 1024);
  strncpy(track.album_name, argv[2], 255);
  strncpy(track.album_artist_name, argv[3], 255);
  strncpy(track.track_number, argv[4], 255);
  strncpy(track.album_length, argv[5], 255);
  strncpy(track.year, argv[6], 255);
  strncpy(track.genre, argv[7], 255);
  strncpy(track.writer_name, argv[8], 255);
  strncpy(track.track_duration, argv[9], 255);
  strncpy(track.bitrate, argv[10], 255);
  strncpy(track.album_art_uri, argv[11], 1024);
  strncpy(track.file_path, argv[12], 1024);
  track.file_size = strtoull(argv[13], nullptr, 10);
  track.file_time = strtoull(argv[14], nullptr, 10);
  return track;
}

static inline std::optional<Track> GetTrackFromEntry(
    std::filesystem::directory_entry entry) {
  auto file_path = entry.path().string();
  auto file_size = entry.file_size();
  auto file_time = entry.last_write_time().time_since_epoch().count();
  if (!entry.is_regular_file() ||
      std::find(kSupportedFileExtensions.begin(),
                kSupportedFileExtensions.end(),
                Strings::ToUpperCase(Strings::Split(file_path, ".").back())) ==
          kSupportedFileExtensions.end())
    return std::nullopt;
  Track track;
  auto retriever = std::make_unique<Retriever>();
  retriever->SetFilePath(file_path);
  auto metadata = retriever->metadata();
  auto album_art = retriever->album_art();
  strncpy(
      track.track_name,
      Strings::EmptyOrAssign(metadata->at("track_name"),
                             std::filesystem::path(entry).filename().string())
          .c_str(),
      255);
  strncpy(track.album_name,
          Strings::EmptyOrAssign(metadata->at("album_name"), kDefaultAlbumName)
              .c_str(),
          255);
  strncpy(track.track_artist_names,
          Strings::EmptyOrAssign(metadata->at("track_artist_names"),
                                 kDefaultArtistName)
              .c_str(),
          1024);
  strncpy(track.album_artist_name,
          Strings::EmptyOrAssign(metadata->at("album_artist_name"),
                                 kDefaultArtistName)
              .c_str(),
          255);
  strncpy(
      track.track_number,
      Strings::EmptyOrAssign(metadata->at("track_number"), kDefaultTrackNumber)
          .c_str(),
      255);
  strncpy(
      track.album_length,
      Strings::EmptyOrAssign(metadata->at("album_length"), kDefaultAlbumLength)
          .c_str(),
      255);
  strncpy(track.year,
          Strings::EmptyOrAssign(metadata->at("year"), kDefaultYear).c_str(),
          255);
  strncpy(track.genre,
          Strings::EmptyOrAssign(metadata->at("genre"), kDefaultGenre).c_str(),
          255);
  strncpy(track.writer_name,
          Strings::EmptyOrAssign(metadata->at("writer_name"), kDefaultWriter)
              .c_str(),
          255);
  strncpy(track.track_duration, metadata->at("track_duration").c_str(), 255);
  strncpy(track.bitrate, metadata->at("bitrate").c_str(), 255);
  strncpy(track.file_path, file_path.c_str(), 1024);
  if (album_art != nullptr) {
    auto path = GetAlbumArtPath(entry.path());
    strncpy(track.album_art_uri, std::string("file://"
#ifdef _WIN32
                                             "/"
#elif
                                             ""
#endif
                                             + path)
                                     .c_str(),
            1024);
    if (!std::filesystem::exists(path)) {
      std::fstream file(path, std::ios::out | std::ios::binary);
      file.write((const char*)(album_art->data()), album_art->size());
      file.close();
    }
  } else {
    strncpy(track.album_art_uri, "", 1024);
  }
  track.file_size = file_size;
  track.file_time = file_time;
  return track;
}

#endif
