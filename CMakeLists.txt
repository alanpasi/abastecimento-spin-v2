cmake_minimum_required(VERSION 3.31)

project("Abastecimento Spin v2"	  # Project name
    VERSION 1.0
    DESCRIPTION "Registro de abastecimentos da Spin"
    LANGUAGES C
)

set(EXEC_NAME "abastecimento_spin")	# Nome do executável conforme o projeto
set(SOURCE_NAME "abastecimento_spin.c")	# Nome do arquivo .c (contendo a função main) conforme o projeto

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c23")
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_executable(${EXEC_NAME} ${SOURCE_NAME} lib/stdpasi.c)
