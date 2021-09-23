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
#include "constants.h"
#include "retriever.h"
#include "structs.h"
#include "utils.h"

#ifndef LIBRARY_UTILS_H_
#define LIBRARY_UTILS_H_

std::string GetAlbumArtPath(std::filesystem::path file_path) {
  auto file_name = file_path.filename().string();
  file_name.erase(std::remove_if(file_name.begin(), file_name.end(),
                                 [](char c) { return !std::isalnum(c); }),
                  file_name.end());
  return (std::filesystem::path(g_cache_path) /
          std::filesystem::path(file_name))
             .string() +
         ".jpg";
}

std::optional<Track> GetTrack(std::filesystem::directory_entry entry) {
  auto file_path = entry.path().string();
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
  track.track_name = EmptyOrAssign(metadata->at("track_name"),
                                   entry.path().filename().string());
  track.album_name =
      EmptyOrAssign(metadata->at("album_name"), kDefaultAlbumName);
  track.track_number =
      EmptyOrAssign(metadata->at("track_number"), kDefaultTrackNumber);
  track.album_length =
      EmptyOrAssign(metadata->at("album_length"), kDefaultAlbumLength);
  track.year = EmptyOrAssign(metadata->at("year"), kDefaultYear);
  track.album_artist_name =
      EmptyOrAssign(metadata->at("album_artist_name"), kDefaultArtistName);
  track.track_artist_names =
      Strings::Split(metadata->at("track_artist_names"), "/");
  std::for_each(
      track.track_artist_names.begin(), track.track_artist_names.end(),
      [](auto& artist) { return EmptyOrAssign(artist, kDefaultArtistName); });

  if (album_art != nullptr) {
    auto path = GetAlbumArtPath(entry.path());
    track.album_art_uri =
        "file://"
#ifdef _WIN32
        "/"
#elif
        ""
#endif
        + path;
    if (!std::filesystem::exists(path)) {
      std::fstream file(path, std::ios::out | std::ios::binary);
      file.write((const char*)(album_art->data()), album_art->size());
      file.close();
    }
  } else {
    track.album_art_uri = "";
  }
  track.file_path = file_path;
  track.file_time = file_time;
  return track;
}

#endif
