
include(FetchContent)
include(CMakeDependentOption)
include(CMakeParseArguments)
include(NanoColor)

function(nano_add_module NAME)
    set(options DEV_MODE)
    set(single PREFIX TAG OPTIONS_MACRO)
    set(multiple "")

    cmake_parse_arguments("OPT" "${options}" "${single}" "${multiple}" ${ARGN})

    if (NOT OPT_TAG)
        set(OPT_TAG master)
    endif()

    set(targetName nano-${NAME})
    set(GIT_REPOSITORY "https://github.com/Meta-Sonic/${targetName}.git")

    if (OPT_DEV_MODE)
        # File name with neither the directory nor the longest extension.
        get_filename_component(repoName  ${GIT_REPOSITORY} NAME_WE)

        if (NOT OPT_PREFIX)
            set(OPT_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/..")
        endif()

        if (IS_DIRECTORY "${OPT_PREFIX}/${repoName}")
            string(TOUPPER ${targetName} targetNameUpper)
            set("NANO_${targetNameUpper}_SOURCE_DIR" "${OPT_PREFIX}/${repoName}")

            # If this is set, no download or update steps are performed for the specified
            # content and the <lcName>_SOURCE_DIR variable returned to the caller is
            # pointed at this location. This gives developers a way to have a separate
            # checkout of the content that they can modify freely without interference
            # from the build. The build simply uses that existing source, but it still
            # defines <lcName>_BINARY_DIR to point inside its own build area.
            set("FETCHCONTENT_SOURCE_DIR_${targetNameUpper}" "${NANO_${targetNameUpper}_SOURCE_DIR}")
        endif()
    endif()

    if(${targetName}_POPULATED)
    message(WARNING DSLJDSKLJDSKJDLKSJDKS)
    endif()
    FetchContent_Declare(${targetName}
        GIT_REPOSITORY ${GIT_REPOSITORY}
        GIT_TAG ${OPT_TAG}
        GIT_PROGRESS ON
        GIT_SHALLOW ON)

    FetchContent_GetProperties(${targetName})

    if(NOT ${targetName}_POPULATED)
        if (OPT_OPTIONS_MACRO)
            cmake_language(CALL ${OPT_OPTIONS_MACRO})
        endif()

        # Fetch the content using previously declared details.
        FetchContent_Populate(${targetName})

        # Bring the populated content into the build.
        add_subdirectory(${${targetName}_SOURCE_DIR} ${${targetName}_BINARY_DIR})
    endif()
endfunction()