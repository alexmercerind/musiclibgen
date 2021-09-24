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
#include <stdint.h>

#include "structs.h"

#ifndef MUSICLIBGEN_H_
#define MUSICLIBGEN_H_

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT void LibrarySetProgressCallback(void (*callback)(int32_t completed,
                                                           int32_t total));
DLLEXPORT void LibrarySetCachePath(const char* path);
DLLEXPORT void LibrarySetLibraryPaths(int32_t size, const char** paths);
DLLEXPORT void LibraryCreate();
DLLEXPORT int32_t LibraryGetTrackCount();
DLLEXPORT Track* LibraryGetTracks();

#ifdef __cplusplus
}
#endif
#endif
