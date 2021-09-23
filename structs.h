/*
 * This file is part of the musiclibgen
 * (https://github.com/alexmercerind/musiclibgen).
 *
 * Copyright (c) 2015 Hitesh Kumar Saini <saini123hitesh@gmail.com>.
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

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct _Track {
  std::string track_name;
  std::string album_name;
  std::string track_number;
  std::string album_length;
  std::string year;
  std::string album_artist_name;
  std::vector<std::string> track_artist_names;
  std::string album_art_uri;
  std::string file_path;
  int32_t file_size;
  uint64_t file_time;
} Track;

typedef struct _Album {
  std::string album_name;
  std::string album_artist_name;
  std::string year;
  std::vector<Track> tracks;
  std::string album_art_uri;
} Album;

typedef struct _Artist {
  std::string artist_name;
  std::vector<Track> tracks;
  std::vector<Album> albums;
} Artist;

typedef struct _Playlist {
  std::string playlist_name;
  std::vector<Track> tracks;
} Playlist;

#endif
