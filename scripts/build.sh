#!/bin/bash

# Script de compilación

set -e

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

log() {
    echo -e "${GREEN}[BUILD]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

# Limpiar build anterior
clean_build() {
    log "Limpiando directorios de compilación anteriores..."
    if [ -d "build" ]; then
        log "El directorio 'build' ya existe. ¿Deseas eliminarlo y reconstruir? (y/n)"
        read confirmation
        if [ "$confirmation" != "y" ]; then
            log "Compilación cancelada"
            exit 0
        fi
        rm -rf build
    fi
    mkdir build
    cd build
}

# Compilar con CMake
compile_project() {
    log "Compilando proyecto..."
    cmake ..
    make -j$(nproc)
    log "Compilación completada"
}

# Ejecutar tests
run_tests() {
    log "Ejecutando pruebas..."
    if [ -f ./tests/api_tests ]; then
        ./tests/api_tests
    else
        error "No se encontraron pruebas"
    fi
}

main() {
    clean_build
    compile_project
    # Descomentar la siguiente línea si los tests están configurados
    # run_tests
}

main
