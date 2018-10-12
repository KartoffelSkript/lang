function(kscript_info message)
    if (kscript_VERBOSE)
        message("Build-Info: ${message}")
    endif ()
endfunction()

function(kscript_panic message)
    message(FATAL_ERROR message)
endfunction()