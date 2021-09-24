#include "../library.h"

#include <iostream>

int32_t main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  const char* library_paths[] = {"C:\\Users\\alexmercerind\\Music"};
  LibrarySetCachePath("C:\\Users\\alexmercerind\\.library");
  LibrarySetLibraryPaths(1, library_paths);
  LibrarySetProgressCallback([](auto completed, auto total) {
    std::cout << completed << " / " << total << "\n";
  });
  LibraryCreate();
  for (auto i = 0; i < LibraryGetTrackCount(); i++) {
    std::cout << LibraryGetTracks()[i].album_name << "\n";
  }
  std::cin.get();
  return EXIT_SUCCESS;
}
