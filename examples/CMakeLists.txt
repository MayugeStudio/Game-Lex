include(CTest)

set(EXAMPLE_1_ASM ${CMAKE_CURRENT_SOURCE_DIR}/examples/123.gla)
set(EXAMPLE_1_BIN ${CMAKE_CURRENT_SOURCE_DIR}/examples/123.glb)
set(EXAMPLE_1 ${EXAMPLE_1_ASM} ${EXAMPLE_1_BIN})

set(EXAMPLE_2_ASM ${CMAKE_CURRENT_SOURCE_DIR}/examples/fib.gla)
set(EXAMPLE_2_BIN ${CMAKE_CURRENT_SOURCE_DIR}/examples/fib.glb)
set(EXAMPLE_2 ${EXAMPLE_2_ASM} ${EXAMPLE_2_BIN})

add_test(NAME assembly-test-1 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/gla.exe ${EXAMPLE_1})
add_test(NAME assembly-test-2 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/gla.exe ${EXAMPLE_2})

add_test(NAME emulator-test-1 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/gle.exe -i ${EXAMPLE_1_BIN})
add_test(NAME emulator-test-2 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/gle.exe -i ${EXAMPLE_2_BIN})
