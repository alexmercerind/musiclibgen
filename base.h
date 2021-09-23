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

#ifndef BASE_H_
#define BASE_H_

#include <memory>
#include <string>
#include <vector>
#include <future>
#include <cstring>
#include <cassert>
#include <fstream>
#include <optional>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

static std::string g_cache_path = "";
static std::vector<std::string> g_library_paths = {};
static std::vector<std::unordered_map<std::string, std::string>> g_tracks = {};

#endif
