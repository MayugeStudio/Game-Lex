enable_testing()

set(EXAMPLE_1_ASM ${GLEX_DIR}/tests/1_123.gla)
set(EXAMPLE_1_BIN ${GLEX_DIR}/tests/1_123.glb)
set(EXAMPLE_1 ${EXAMPLE_1_ASM} ${EXAMPLE_1_BIN})

set(EXAMPLE_2_ASM ${GLEX_DIR}/tests/2_comments.gla)
set(EXAMPLE_2_BIN ${GLEX_DIR}/tests/2_comments.glb)
set(EXAMPLE_2 ${EXAMPLE_2_ASM} ${EXAMPLE_2_BIN})

set(EXAMPLE_3_ASM ${GLEX_DIR}/tests/3_fibs.gla)
set(EXAMPLE_3_BIN ${GLEX_DIR}/tests/3_fibs.glb)
set(EXAMPLE_3 ${EXAMPLE_3_ASM} ${EXAMPLE_3_BIN})

add_test(NAME assembly-test-1 COMMAND ${GLEX_DIR}/bin/gla.exe ${EXAMPLE_1}) # TODO RENAME assembly to assembler
add_test(NAME assembly-test-2 COMMAND ${GLEX_DIR}/bin/gla.exe ${EXAMPLE_2}) # TODO RENAME assembly to assembler
add_test(NAME assembly-test-3 COMMAND ${GLEX_DIR}/bin/gla.exe ${EXAMPLE_3}) # TODO RENAME assembly to assembler

add_test(NAME emulator-test-1 COMMAND ${GLEX_DIR}/bin/gle.exe -i ${EXAMPLE_1_BIN})
add_test(NAME emulator-test-2 COMMAND ${GLEX_DIR}/bin/gle.exe -i ${EXAMPLE_2_BIN})
add_test(NAME emulator-test-3 COMMAND ${GLEX_DIR}/bin/gle.exe -i ${EXAMPLE_3_BIN})
