#include <stdio.h>
#include "../library.h"

void Progress(int completed, int total) {
  printf("%d / %d\n", completed, total);
}

int main() {
  const char* library_paths[] = {"C:\\Users\\alexmercerind\\Music"};
  LibrarySetCachePath("C:\\Users\\alexmercerind\\.library");
  LibrarySetLibraryPaths(1, library_paths);
  LibrarySetProgressCallback(Progress);
  LibraryCreate();
  for (int i = 0; i < LibraryGetAlbumCount(); i++) {
    printf("%s\n", LibraryGetAlbums()[i].album_name);
    for (int j = 0; j < LibraryGetAlbums()[i].tracks_size; j++) {
      printf("%d . %s\n", j, LibraryGetAlbums()[i].tracks[j].track_name);
    }
  }
  getchar();
  return 0;
}
