cmake_minimum_required(VERSION 3.31)

project("Abastecimento_Spin_v2"	  # Project name
    VERSION 1.0
    DESCRIPTION "Registro de abastecimentos da Spin"
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Encontra o pacote GTK4 usando PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(SQLITE3 REQUIRED sqlite3) # Encontra o SQLite3

# Define onde estão os arquivos de cabeçalho
include_directories(include ${GTK4_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIRS}) # Adiciona includes do SQLite3

# Cria a biblioteca estática "stdpasi"
add_library(stdpasi STATIC
    lib/stdpasi.c
    lib/init_gtk.c # Adiciona init_gtk.c à biblioteca
)

# Cria o executável "abastecimento_spin"
add_executable(abastecimento_spin abastecimento_spin.c)

# Vincula o executável à biblioteca "stdpasi", GTK4 e SQLite3
target_link_libraries(abastecimento_spin stdpasi ${GTK4_LIBRARIES} ${SQLITE3_LIBRARIES}) # Vincula com SQLite3

# Adiciona as flags de compilação do GTK4 e SQLite3
target_compile_options(abastecimento_spin PRIVATE ${GTK4_CFLAGS_OTHER} ${SQLITE3_CFLAGS_OTHER})
