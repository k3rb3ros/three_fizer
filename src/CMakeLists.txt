#cmake_minimum_required (VERSION 2.6)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

#optional sets to force CLang as compiler
set (CMAKE_C_COMPILER clang)
set (CMAKE_CXX_COMPILER clang++)

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
     skeinApi.c
     skeinBlockNo3F.c
     skein.c
     tfHeader.c
     threefish1024Block.c
     threefish512Block.c
     threefish256Block.c
     threefishApi.c
     threefizer.c
     util.c
     writeThread.c
    )

set (test_dir test)

#C files for threefizer tests
set (tests_c_src
     ${test_dir}/cbcTests.c
     ${test_dir}/chunkTests.c
     ${test_dir}/fileIOtests.c
     ${test_dir}/skeinTests.c
     ${test_dir}/testSuite.c
     ${test_dir}/threefishTests.c
    )

set (files_to_test_against
     cbc.c
     cbcEncrypt.c
     cbcDecrypt.c
     chunk.c
     chunkQueue.c
     fileIO.c
     skein.c
     skeinApi.c
     skeinBlockNo3F.c
     threefish1024Block.c
     threefish512Block.c
     threefish256Block.c
     threefishApi.c
    )

add_executable (${PROJECT_NAME} ${threefizer_c_src})
add_executable (${TEST_SUITE} ${tests_c_src} ${files_to_test_against})

execute_process (COMMAND mkdir src/tests)
execute_process (COMMAND cp ../src/test/file_tests/empty_file src/tests/)
execute_process (COMMAND cp ../src/test/file_tests/simple_lines.dat src/tests/)

add_dependencies (${PROJECT_NAME} ${THREAD_LIB})
target_link_libraries (${PROJECT_NAME} ${THREAD_LIB})
