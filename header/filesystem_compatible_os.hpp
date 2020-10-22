/*
If compilation error occurs "error: ld returned 1 exit status"
Please add compilation option "-lstdc++fs" in end-of-compile-command.
*/
#ifndef FILESYSTEM_COMPATIBLE_OS_HPP_
#define FILESYSTEM_COMPATIBLE_OS_HPP_
  #ifdef __APPLE__
    #include <filesystem>
    namespace fs = std::filesystem;
  #elif __linux__
    //#include <experimental/filesystem>
    #include <filesystem>
    namespace fs = std::filesystem;
    //namespace fs = std::experimental::filesystem;
  #else
    #error "OS not supported."
  #endif
#endif
