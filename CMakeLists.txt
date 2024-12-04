cmake_minimum_required(VERSION 3.12)
project(CPPMongoAPI)

set(CMAKE_CXX_STANDARD 17)

# Incluir las rutas personalizadas para las dependencias locales
list(APPEND CMAKE_PREFIX_PATH 
    "${CMAKE_SOURCE_DIR}/vendor/mongo-driver-install"
    "${CMAKE_SOURCE_DIR}/vendor/cpprest-install/lib/cmake/cpprestsdk"
)

# Usar el archivo de configuración con su nombre real
set(cpprestsdk_DIR "${CMAKE_SOURCE_DIR}/vendor/cpprest-install/lib/cmake/cpprestsdk")
find_package(cpprestsdk REQUIRED)

# Configurar MongoDB C++ Driver
set(BSONCXX_LIB_DIR "${CMAKE_SOURCE_DIR}/vendor/mongo-driver-install/lib")
set(MONGOCXX_LIB_DIR "${CMAKE_SOURCE_DIR}/vendor/mongo-driver-install/lib")
set(BSONCXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/mongo-driver-install/include/bsoncxx/v_noabi")
set(MONGOCXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/mongo-driver-install/include/mongocxx/v_noabi")

# Incluir archivos fuentes (core)
set(SOURCES
    src/main.cpp
    src/models/User.cpp
    src/models/Plan.cpp
    src/models/PaymentMethod.cpp
    src/models/Payment.cpp
    src/models/UserSubscription.cpp
    src/services/UserService.cpp
    src/services/PlanService.cpp
    src/services/PaymentMethodService.cpp
    src/services/PaymentService.cpp
    src/services/UserSubscriptionService.cpp
    src/controllers/UserController.cpp
    src/controllers/PlanController.cpp
    src/controllers/PaymentMethodController.cpp
    src/controllers/PaymentController.cpp
    src/controllers/UserSubscriptionController.cpp
    src/database/MongoConnection.cpp
)

# Crear ejecutable
add_executable(api_executable ${SOURCES})

# Incluir directorios de encabezados necesarios
target_include_directories(api_executable PRIVATE 
    ${BSONCXX_INCLUDE_DIR}
    ${MONGOCXX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor/cpprest-install/include
)

# Enlazar bibliotecas necesarias
target_link_libraries(api_executable PRIVATE 
    ${BSONCXX_LIB_DIR}/libbsoncxx.so
    ${MONGOCXX_LIB_DIR}/libmongocxx.so
    ${CMAKE_SOURCE_DIR}/vendor/cpprest-install/lib/libcpprest.so
    ssl
    crypto
)

# Directorios de búsqueda para bibliotecas
link_directories(
    ${BSONCXX_LIB_DIR}
    ${MONGOCXX_LIB_DIR}
    ${CMAKE_SOURCE_DIR}/vendor/cpprest-install/lib
)
