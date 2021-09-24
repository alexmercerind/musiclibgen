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

#ifndef UTILS_H_
#define UTILS_H_

#define RM(x)  \
               \
  size -= (x); \
               \
  data += (x);

static inline uint32_t U32_AT(const void* data) {
  uint32_t x;
  memcpy(&x, data, sizeof(x));
#ifdef _WIN32
  return ((x & 0x000000FF) << 24) | ((x & 0x0000FF00) << 8) |
         ((x & 0x00FF0000) >> 8) | ((x & 0xFF000000) >> 24);
#else
  return __builtin_bswap32(x);
#endif
}

#ifdef _WIN32
static inline char* strndup(const char* array, size_t size) {
  char* buffer;
  size_t n;
  buffer = (char*)malloc(size + 1);
  if (buffer) {
    for (n = 0; ((n < size) && (array[n] != 0)); n++) buffer[n] = array[n];
    buffer[n] = 0;
  }
  return buffer;
}
#endif

auto TO_WIDESTRING = [](std::string string) -> std::wstring {
  return std::wstring(string.begin(), string.end());
};

auto TO_STRING = [](std::wstring wide_string) -> std::string {
#ifdef _WIN32
  if (wide_string.empty()) {
    return std::string();
  }
  int target_length = ::WideCharToMultiByte(
      CP_UTF8, WC_ERR_INVALID_CHARS, wide_string.data(),
      static_cast<int>(wide_string.length()), nullptr, 0, nullptr, nullptr);
  if (target_length == 0) {
    return std::string();
  }
  std::string utf8_string;
  utf8_string.resize(target_length);
  int converted_length = ::WideCharToMultiByte(
      CP_UTF8, WC_ERR_INVALID_CHARS, wide_string.data(),
      static_cast<int>(wide_string.length()), utf8_string.data(), target_length,
      nullptr, nullptr);
  if (converted_length == 0) {
    return std::string();
  }
  return utf8_string;
#elif __linux__
  std::vector<char> buffer(wide_string.size() * 4);
  size_t size = wcstombs(&buffer[0], &wide_string[0], buffer.size());
  return std::string(buffer.data(), size);
#endif
};

class Strings {
 public:
  static inline bool StartsWith(std::string string, std::string subString) {
    if (string.substr(0, subString.size()) == subString)
      return true;
    else
      return false;
  }

  static inline std::vector<std::string> Split(std::string string,
                                               std::string match) {
    std::vector<std::string> result = {};
    size_t match_size = match.size();
    size_t last = 0;
    for (int index = 0; index < string.size(); index++) {
      if (string.substr(index, match_size) == match) {
        result.emplace_back(string.substr(last, index - last));
        last = match_size + index;
      }
    }
    result.emplace_back(string.substr(last, string.size()));
    return result;
  }

  static inline std::string Replace(std::string string, std::string match,
                                    std::string replace) {
    std::string result;
    size_t match_size = match.size();
    size_t last_index = 0;
    size_t match_counter = 0;
    for (int index = 0; index < string.size(); index++) {
      if (match_counter != 0) {
        if (match_counter == match.size() - 1)
          match_counter = 0;
        else
          match_counter++;
      } else if (index == string.size() - match.size()) {
        result.append(replace);
        break;
      } else if (string.substr(index, match_size) == match &&
                 match_counter == 0) {
        result.append(replace);
        last_index = match_size + index;
        match_counter++;
      } else {
        result.push_back(string[index]);
      }
    }
    return result;
  }

  static inline std::string ToUpperCase(std::string& string) {
    std::transform(string.begin(), string.end(), string.begin(), ::toupper);
    return string;
  }

  static inline std::string ReplaceAll(std::string string,
                                       const std::string& from,
                                       const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = string.find(from, start_pos)) != std::string::npos) {
      string.replace(start_pos, from.length(), to);
      start_pos += to.length();
    }
    return string;
  }

  static inline std::string EmptyOrAssign(std::string string,
                                          std::string replacement) {
    if (string.compare("") == 0) {
      return replacement;
    }
    return Strings::ReplaceAll(string, "'", "''");
  }
};

#endif