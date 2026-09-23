option(ENABLE_DOC "Enable documentation generation with Doxygen" OFF)

if(ENABLE_DOC)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_PROJECT_NAME "StormByte-String")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")
    set(DOXYGEN_PROJECT_BRIEF "C++26 string module of the StormByte suite")
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")

    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_FILE_PATTERNS "*.hxx" "*.h" "*.txx" "*.md")
    set(DOXYGEN_EXCLUDE_PATTERNS
        "*.cxx"
        "*/thirdparty/*"
    )

    set(DOXYGEN_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/lib/public")
    set(DOXYGEN_STRIP_FROM_PATH "${CMAKE_SOURCE_DIR}")
    set(DOXYGEN_STRIP_FROM_INC_PATH "${CMAKE_SOURCE_DIR}/lib/public")

    set(DOXYGEN_ENABLE_PREPROCESSING YES)
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
    set(DOXYGEN_PREDEFINED
        "STORMBYTE_STRING_PUBLIC:="
        "STORMBYTE_PUBLIC:="
        "WINDOWS="
        "LINUX="
        "MACOS="
    )

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_HTML_OUTPUT ".")
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX YES)

    doxygen_add_docs(
        StormByteStringDoc
        "${CMAKE_SOURCE_DIR}/lib/public/StormByte"
        "${CMAKE_SOURCE_DIR}/README.md"
        COMMENT "Generate HTML pages"
    )
endif()
