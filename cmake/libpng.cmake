include(ExternalProject)

set(LIBPNG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/libpng)
set(LIBPNG_BIN ${CMAKE_CURRENT_BINARY_DIR}/libpng)
set(LIBPNG_STATIC_LIB ${LIBPNG_BIN}/lib/libpng.a)
set(LIBPNG_INCLUDES ${LIBPNG_BIN}/include)

file(MAKE_DIRECTORY ${LIBPNG_INCLUDES})

ExternalProject_Add(libpng
        PREFIX ${LIBPNG_BIN}
        SOURCE_DIR ${LIBPNG_DIR}
        #DOWNLOAD_COMMAND cd ${LIBPNG_DIR} && ${LIBPNG_DIR}/autogen.sh
        CONFIGURE_COMMAND ${LIBPNG_DIR}/configure --srcdir=${LIBPNG_DIR} --prefix=${LIBPNG_BIN} --enable-static=yes --disable-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${LIBPNG_STATIC_LIB}
)

add_library(png STATIC IMPORTED GLOBAL)
add_dependencies(png libpng)

set_target_properties(png PROPERTIES IMPORTED_LOCATION ${LIBPNG_STATIC_LIB})
set_target_properties(png PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${LIBPNG_INCLUDES})