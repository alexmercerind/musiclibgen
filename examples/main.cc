#include "../library.h"

#include <iostream>

int32_t main() {
  const char* library_paths[] = {"C:\\Users\\alexmercerind\\Music"};
  LibrarySetCachePath("C:\\Users\\alexmercerind\\.library");
  LibrarySetLibraryPaths(1, library_paths);
  LibraryIndex([](auto completed, auto total) {
    system("cls");
    std::cout << completed << " / " << total << std::endl;
  });
  std::cin.get();
  return EXIT_SUCCESS;
}
