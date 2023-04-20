/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Icon_large_ico;
    const int            Icon_large_icoSize = 10437;

    extern const char*   Icon_large_png;
    const int            Icon_large_pngSize = 13755;

    extern const char*   Icon_small_ico;
    const int            Icon_small_icoSize = 4286;

    extern const char*   Icon_small_png;
    const int            Icon_small_pngSize = 4004;

    extern const char*   Logo_ai;
    const int            Logo_aiSize = 242042;

    extern const char*   Logo_png;
    const int            Logo_pngSize = 344369;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
