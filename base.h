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

#ifndef BASE_H_
#define BASE_H_

#include "structs.h"

#include <memory>
#include <string>
#include <vector>
#include <future>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <mutex>
#include <optional>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

extern "C" {
#define SQLITE_API __declspec(dllexport)
#include "external/sqlite3.h"
}

#ifdef DEBUG
#define DEBUG_LOG(value) printf("%s : %s\n", #value, value)
#else
#define DEBUG_LOG(value)
#endif

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

static std::string g_cache_path = "";
static std::vector<std::string> g_library_paths = {};
static std::vector<Track> g_tracks = {};
static std::vector<Album> g_albums = {};
static std::vector<Artist> g_artists = {};
static sqlite3* g_library_cache = nullptr;
#endif
