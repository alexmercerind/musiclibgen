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
#include <string.h>
#include <stdint.h>

#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef struct _Track {
  char track_name[255];
  char album_name[255];
  char track_artist_names[1024];
  char album_artist_name[255];
  char track_number[255];
  char album_length[255];
  char year[255];
  char genre[255];
  char writer_name[255];
  char track_duration[255];
  char bitrate[255];
  char album_art_uri[1024];
  char file_path[1024];
  int32_t file_size;
  uint64_t file_time;
} Track;

typedef struct _Album {
  char album_name[255];
  char album_artist_name[255];
  char year[255];
  Track tracks[32];
  int32_t tracks_size;
  char album_art_uri[1024];
} Album;

typedef struct _Artist {
  char artist_name[255];
  Track tracks[512];
  int32_t tracks_size;
  Album albums[64];
  int32_t albums_size;
} Artist;

typedef struct _Playlist {
  char playlist_name[255];
  Track tracks[128];
  int32_t tracks_size;
} Playlist;

#endif
