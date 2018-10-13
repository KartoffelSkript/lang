set(kscript_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB kscript_SOURCE_FILES RELATIVE "${kscript_SOURCE_DIR}"
        "*.cc"
        "*.cpp"
        "*.h"
        "*.hpp")

if (kscript_VERBOSE)
    list(LENGTH kscript_SOURCE_FILES kscript_SOURCE_FILES_LENGTH)

    kscript_info("Loaded ${source_files_length} file(s).")
endif ()

add_executable(kscript
        "${kscript_SOURCE_DIR}/log.h"
        "${kscript_SOURCE_DIR}/log.cc"
        "${kscript_SOURCE_DIR}/main.cc")