option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST)
	enable_testing()

	add_executable(StringTests string_test.cxx)
	target_link_libraries(StringTests StormByte-String)
	add_test(NAME StringTests COMMAND StringTests)

	add_executable(WStringTests wstring_test.cxx)
	target_link_libraries(WStringTests StormByte-String)
	add_test(NAME WStringTests COMMAND WStringTests)
endif()
