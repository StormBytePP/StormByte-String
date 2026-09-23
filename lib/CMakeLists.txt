include(GNUInstallDirs)

option(STORMBYTE_STRING_SHARED "Build StormByte-String as a shared library" ON)

file(GLOB_RECURSE STORMBYTE_STRING_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.cxx")

if(STORMBYTE_STRING_SHARED)
	add_library(StormByte-String SHARED ${STORMBYTE_STRING_SOURCES})
	target_compile_definitions(StormByte-String
		PRIVATE StormByte_String_EXPORTS
		PUBLIC STORMBYTE_STRING_SHARED
	)
else()
	add_library(StormByte-String STATIC ${STORMBYTE_STRING_SOURCES})
endif()

add_library(StormByte::String ALIAS StormByte-String)

set_target_properties(StormByte-String PROPERTIES
	LINKER_LANGUAGE CXX
	SOVERSION ${CMAKE_PROJECT_VERSION}
	VERSION ${CMAKE_PROJECT_VERSION}
	POSITION_INDEPENDENT_CODE ON
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

if(MSVC)
	target_compile_options(StormByte-String PRIVATE
		/EHsc
		$<$<CONFIG:Release>:/O2>
		$<$<CONFIG:Debug>:/Od>
	)
else()
	target_compile_options(StormByte-String PRIVATE
		$<$<CONFIG:Debug>:-pipe -g -ggdb -Wall -Wextra -Wnon-virtual-dtor -pedantic -pedantic-errors -O0>
	)
endif()

target_include_directories(StormByte-String
	SYSTEM BEFORE
		PUBLIC "${CMAKE_CURRENT_LIST_DIR}/public"
		PRIVATE "${CMAKE_CURRENT_LIST_DIR}/private"
)

target_link_libraries(StormByte-String PUBLIC StormByte)
