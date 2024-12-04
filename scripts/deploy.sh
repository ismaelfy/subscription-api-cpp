#!/bin/bash

# Script de despliegue

set -e

# Variables de despliegue
SERVER_USER="ubuntu"
SERVER_HOST="tu-servidor.com"
DEPLOY_PATH="/opt/cpp-mongodb-api"

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

log() {
    echo -e "${GREEN}[DEPLOY]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

# Preparar artefactos
prepare_artifacts() {
    log "Preparando artefactos de despliegue..."
    
    # Compilar proyecto
    ./scripts/build.sh
    
    # Crear carpeta de distribución
    mkdir -p dist
    cp build/api_executable dist/
    cp .env dist/
}

# Transferir al servidor
transfer_to_server() {
    log "Transfiriendo artefactos al servidor..."
    
    ssh "$SERVER_USER@$SERVER_HOST" "mkdir -p $DEPLOY_PATH"
    
    # Transferir binario y configuración
    scp -r dist/* "$SERVER_USER@$SERVER_HOST:$DEPLOY_PATH/"
}

# Configurar servicio systemd
configure_systemd() {
    log "Configurando servicio systemd..."
    
    ssh "$SERVER_USER@$SERVER_HOST" << EOL
cat > /etc/systemd/system/cpp-api.service << EOF
[Unit]
Description=C++ MongoDB API Service
After=network.target

[Service]
Type=simple
User=$SERVER_USER
WorkingDirectory=$DEPLOY_PATH
ExecStart=$DEPLOY_PATH/api_executable
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF

    systemctl daemon-reload
    systemctl enable cpp-api
    systemctl restart cpp-api
EOL
}

# Función principal de despliegue
main() {
    prepare_artifacts
    transfer_to_server
    configure_systemd
    
    log "Despliegue completado exitosamente"
}

main