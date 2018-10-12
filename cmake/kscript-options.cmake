# If the verbose mode is activated, kscript will log more
# information while building. This information may include
# statistics, dependencies and versions.
option(kscript_VERBOSE "Enable for verbose logging." OFF)

# If activated, the language standard is set to C++17,
# otherwise the C++11 standard is used.
option(kscript_USE_CPP_17 "Enable for a C++17 language standard" ON)
