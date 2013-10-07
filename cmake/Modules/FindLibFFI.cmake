# Try to find LibFFI
# Once done this will define
#   LIBFFI_FOUND - System has libffi
#   LIBFFI_INCLUDE_DIRS - The libffi include directories
#   LIBFFI_LIBRARIES - The libraries needed to use libffi
#   LIBFFI_DEFINITIONS - Compiler switches required for using libffi

# Invoke pkg-config.
find_package(PkgConfig)
pkg_check_modules(PC_LIBFFI QUIET libffi)

# Set definitions.
set(LIBFFI_DEFINITIONS ${PC_LIBFFI_CFLAGS_OTHER})

# Set include directories.
find_path(  LIBFFI_INCLUDE_DIR ffi.h
            HINTS ${PC_LIBFFI_INCLUDEDIR} ${PC_LIBFFI_INCLUDE_DIRS}
            #PATH_SUFFIXES libxml2
            )

# Set the library.           
find_library(LIBFFI_LIBRARY NAMES ffi
             HINTS ${PC_LIBFFI_LIBDIR} ${PC_LIBFFI_LIBRARY_DIRS} )

             
set(LIBFFI_LIBRARIES ${LIBFFI_LIBRARY} )
set(LIBFFI_INCLUDE_DIRS ${LIBFFI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibFFI  DEFAULT_MSG
                                  LIBFFI_LIBRARY LIBFFI_INCLUDE_DIR)
mark_as_advanced(LIBFFI_INCLUDE_DIR LIBFFI_LIBRARY )