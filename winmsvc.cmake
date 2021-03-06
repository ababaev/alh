if(NOT LINKING_TYPE)
    message(FATAL_ERROR "Please specify -DLINKING_TYPE=static or -DLINKING_TYPE=dynamic")
    return()
endif()

if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "Please specify CMAKE_TOOLCHAIN_FILE, for example -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake")
    message(FATAL_ERROR "Please clean CMake cache and start again")
    return()
endif()


if(LINKING_TYPE MATCHES "static")
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
else()
    set(VCPKG_TARGET_TRIPLET "x64-windows")
endif()

message(STATUS "VCPKG_TARGET_TRIPLET:${VCPKG_TARGET_TRIPLET}")

if (VCPKG_TARGET_TRIPLET MATCHES "static")
    message(STATUS "static")
    add_definitions(-D_UNICODE -DUNICODE -DwxUSE_GUI=1 -D__WXMSW__)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
else()
    message(STATUS "dynamic")
    add_definitions(-D_UNICODE -DUNICODE -DWXUSINGDLL -DwxUSE_GUI=1 -D__WXMSW__)
endif()

include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/mswu ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
set(wxWidgets_LIB_DIR ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib)
set(wxWidgets_LIBRARIES
    ${wxWidgets_LIB_DIR}/wxbase31u_net.lib
    ${wxWidgets_LIB_DIR}/wxmsw31u_core.lib
    ${wxWidgets_LIB_DIR}/wxbase31u.lib
    ${wxWidgets_LIB_DIR}/wxregexu.lib
    ${wxWidgets_LIB_DIR}/libpng16.lib
    ${wxWidgets_LIB_DIR}/zlib.lib
    comctl32 Rpcrt4
    )