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
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <string>
#include <vector>

static constexpr auto kDefaultAlbumName = "Unknown Album";
static constexpr auto kDefaultArtistName = "Unknown Artist";
static constexpr auto kDefaultTrackNumber = "1";
static constexpr auto kDefaultAlbumLength = "1";
static constexpr auto kDefaultYear = "Unknown Year";
static constexpr auto kDefaultGenre = "Unknown Genre";
static constexpr auto kDefaultWriter = "Unknown Writer";
constexpr auto kLibraryDatabaseCreateQuery =
    "CREATE TABLE Tracks("
    "   track_name CHAR(255) NOT NULL,"
    "   track_artist_names VARCHAR NOT NULL,"
    "   album_name CHAR(255) NOT NULL,"
    "   album_artist_name CHAR(255) NOT NULL,"
    "   track_number CHAR(255) NOT NULL,"
    "   album_length CHAR(255) NOT NULL,"
    "   year CHAR(255) NOT NULL,"
    "   genre CHAR(255) NOT NULL,"
    "   writer_name CHAR(255) NOT NULL,"
    "   track_duration CHAR(255) NOT NULL,"
    "   bitrate CHAR(255) NOT NULL,"
    "   album_art_uri VARCHAR NOT NULL,"
    "   file_path VARCHAR PRIMARY KEY NOT NULL,"
    "   file_size BIGINT NOT NULL,"
    "   file_time BIGINT NOT NULL"
    ");";
static const std::vector<std::string> kSupportedFileExtensions = {
    "AAC", "AIFF", "FLAC", "M4A", "MP3", "OGA",
    "OGG", "OGX",  "OPUS", "WAV", "WMA"};
static const std::unordered_map<std::string, std::wstring> kMetadataKeys = {
    {"track_name", L"Track"},
    {"track_artist_names", L"Performer"},
    {"album_name", L"Album"},
    {"album_artist_name", L"Album/Performer"},
    {"track_number", L"Track/Position"},
    {"album_length", L"Track/Position_Total"},
    {"year", L"Recorded_Date"},
    {"genre", L"Genre"},
    {"writer_name", L"WrittenBy"},
    {"track_duration", L"Duration"},
    {"bitrate", L"OverallBitRate"},
};
#endif
