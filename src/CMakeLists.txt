#cmake_minimum_required (VERSION 2.6)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

#optional sets to force CLang as compiler
set (CMAKE_C_COMPILER clang)

set (RNG_LIB PCG)
set (SCRYPT_LIB SCRYPT)
set (SKEIN3FISH_LIB SKEIN3FISH)
set (TEST_SUITE ${PROJECT_NAME}_tests)

#C files for threefizer
set (threefizer_c_src
     arguments.c
     cbc.c
     cbcDecrypt.c
     cbcEncrypt.c
     chunk.c
     chunkQueue.c
     controller.c
     cryptoThread.c
     decrypt.c
     encrypt.c
     error.c
     fileIO.c
     hash.c
     key.c
     mac.c
     macThread.c
     noHash.c
     pad.c
     print.c
     progressBar.c
     random.c
     readThread.c
     tfHeader.c
     threefizer.c
     util.c
     writeThread.c
    )

set (test_dir test)

#C files for threefizer tests
set (tests_c_src
     ${test_dir}/cbcTests.c
     ${test_dir}/chunkTests.c
     ${test_dir}/fileIoTests.c
     ${test_dir}/skeinTests.c
     ${test_dir}/testSuite.c
     ${test_dir}/threefishTests.c
     ${test_dir}/utilTests.c
    )

set (files_to_test_against
     cbc.c
     cbcEncrypt.c
     cbcDecrypt.c
     chunk.c
     chunkQueue.c
     fileIO.c
     util.c
    )

#build the PCG static lib first
add_subdirectory (pcg)
add_subdirectory (scrypt)
add_subdirectory (skein3fish)
add_executable (${PROJECT_NAME} ${threefizer_c_src})
add_executable (${TEST_SUITE} ${tests_c_src} ${files_to_test_against})

execute_process (COMMAND mkdir src/tests)
execute_process (COMMAND cp ../src/test/file_tests/empty_file src/tests/)
execute_process (COMMAND cp ../src/test/file_tests/simple_lines.dat src/tests/)

target_link_libraries (${PROJECT_NAME} ${RNG_LIB} ${SKEIN3FISH_LIB} ${SCRYPT_LIB} ${THREAD_LIB})
target_link_libraries (${TEST_SUITE} ${SKEIN3FISH_LIB})
