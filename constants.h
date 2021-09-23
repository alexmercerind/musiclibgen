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
#include <string>
#include <vector>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

constexpr auto kDefaultAlbumName = "Unknown Album";
constexpr auto kDefaultArtistName = "Unknown Artist";
constexpr auto kDefaultTrackNumber = "1";
constexpr auto kDefaultAlbumLength = "1";
constexpr auto kDefaultYear = "Unknown Year";
static const std::vector<std::string> kSupportedFileExtensions = {
    "AAC", "AIFF", "FLAC", "M4A", "MP3", "OGA",
    "OGG", "OGX",  "OPUS", "WAV", "WMA"};

#endif
