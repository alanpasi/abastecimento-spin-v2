cmake_minimum_required(VERSION 3.31)

project("Abastecimento_Spin_v2"	  # Project name
    VERSION 1.0
    DESCRIPTION "Registro de abastecimentos da Spin"
    LANGUAGES C
)

# Variáveis do projeto
set(EXEC_NAME "abastecimento_spin")
set(SOURCE_NAME "abastecimento_spin.c")
set(LIB_SOURCE_NAME "lib/stdpasi.c")

# Configurações de compilação
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

# Configurações específicas do compilador
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE -Wall -std=c17")
    # Use c17 ao invés de c23 para maior compatibilidade
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
endif()

# Diretórios de saída
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Inclusão de diretórios de cabeçalho
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Definição do executável
add_executable(${EXEC_NAME} ${SOURCE_NAME} ${LIB_SOURCE_NAME})
target_link_libraries(${EXEC_NAME} -lsqlite3)
