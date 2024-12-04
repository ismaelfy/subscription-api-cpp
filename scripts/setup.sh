#!/bin/bash

set -e  # Salir si ocurre un error

# Colores para mensajes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

log() {
    echo -e "${GREEN}[SETUP]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

# Preguntar al usuario si desea reinstalar dependencias
ask_reinstall() {
    echo "¿Deseas reinstalar los paquetes y configuraciones necesarias? (y/n)"
    read confirmation
    if [ "$confirmation" != "y" ]; then
        log "Instalación de dependencias cancelada."
        exit 0
    fi
}

# Verificar y reinstalar las dependencias del sistema
install_dependencies() {
    log "Instalando dependencias del sistema..."
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        cmake \
        git \
        libssl-dev \
        libsasl2-dev \
        libcurl4-openssl-dev \
        g++ \
        wget \
        curl \
        pkg-config
    log "Dependencias del sistema instaladas correctamente."
}

# Descargar e instalar el MongoDB C++ Driver
install_mongodb_driver() {
    local driver_dir="vendor/mongo-cxx-driver"

    # Eliminar instalación previa
    if [ -d "$driver_dir" ]; then
        log "Eliminando instalación previa del driver de MongoDB..."
        rm -rf "$driver_dir"
    fi
    sudo rm -rf /usr/local/lib/libbsoncxx* /usr/local/lib/libmongocxx*

    # Descargar y construir el driver
    log "Clonando el repositorio del MongoDB C++ Driver..."
    git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable "$driver_dir"

    log "Construyendo e instalando el MongoDB C++ Driver..."
    mkdir -p "$driver_dir/build"
    cd "$driver_dir/build"

    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../mongo-driver-install
    make -j$(nproc)
    make install

    cd ../../../
    log "MongoDB C++ Driver instalado correctamente."
}

# Descargar e instalar el C++ REST SDK
install_cpprest() {
    local cpprest_dir="vendor/cpprestsdk"

    # Eliminar instalación previa
    if [ -d "$cpprest_dir" ]; then
        log "Eliminando instalación previa de cpprest..."
        rm -rf "$cpprest_dir"
    fi

    # Descargar y construir cpprest
    log "Clonando el repositorio de cpprest..."
    git clone https://github.com/microsoft/cpprestsdk.git "$cpprest_dir"

    log "Construyendo e instalando cpprest..."
    mkdir -p "$cpprest_dir/Release/build"
    cd "$cpprest_dir/Release/build"

    # Configurar con opciones para evitar tratar advertencias como errores
    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DCMAKE_CXX_FLAGS="-Wno-error=format-truncation" -DCMAKE_INSTALL_PREFIX=../../../cpprest-install
    make -j$(nproc)
    make install

    cd ../../../../
    log "C++ REST SDK instalado correctamente."
}

# Configurar el archivo de entorno
setup_env() {
    log "Configurando variables de entorno..."
    if [ -f ".env" ]; then
        echo "El archivo .env ya existe. ¿Deseas sobrescribirlo? (y/n)"
        read overwrite
        if [ "$overwrite" != "y" ]; then
            log "Configuración de .env omitida."
            return
        fi
    fi

    cat > .env <<EOL
MONGODB_URI=mongodb+srv://<username>:<password>@<cluster-url>
PORT=8080
LOG_LEVEL=info
EOL

    log "Archivo .env configurado correctamente."
}

# Función principal
main() {
    log "Iniciando configuración del proyecto..."
    ask_reinstall
    install_dependencies
    install_mongodb_driver
    install_cpprest
    setup_env
    log "Configuración completada. Ahora puedes ejecutar './scripts/build.sh' para compilar el proyecto."
}

main
