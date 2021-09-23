#include "../library.h"

#include <iostream>
#include <chrono>

static std::chrono::steady_clock::time_point start;

void Callback(int32_t completed, int32_t total) {
  if (completed == total) {
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                     (end - start))
                     .count()
              << "ms.\n";
  }
}

int32_t main() {
  // Current benchmarks for 249 files:
  // 8900ms  : multi-threaded.
  // 18123ms : single-threaded.
  ::start = std::chrono::high_resolution_clock::now();
  const char* library_paths[] = {"C:\\Users\\alexmercerind\\Music"};
  LibrarySetCachePath("C:\\Users\\alexmercerind\\.library");
  LibrarySetLibraryPaths(1, library_paths);
  LibraryIndex(Callback);
  std::cin.get();
  return EXIT_SUCCESS;
}
