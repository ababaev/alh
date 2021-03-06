cmake_minimum_required(VERSION 3.10)
project(ah VERSION 1.0)

set(AH_CPP
    "${PROJECT_SOURCE_DIR}/ahapp.cpp"
    "${PROJECT_SOURCE_DIR}/ah_control.cpp"
    "${PROJECT_SOURCE_DIR}/ahframe.cpp"
    "${PROJECT_SOURCE_DIR}/atlaparser.cpp"
    "${PROJECT_SOURCE_DIR}/cfgfile.cpp"
    "${PROJECT_SOURCE_DIR}/collection.cpp"
    "${PROJECT_SOURCE_DIR}/consts_ah.cpp"
    "${PROJECT_SOURCE_DIR}/createnewunitdlg.cpp"
    "${PROJECT_SOURCE_DIR}/cstr.cpp"
    "${PROJECT_SOURCE_DIR}/data_control.cpp"
    "${PROJECT_SOURCE_DIR}/data.cpp"
    "${PROJECT_SOURCE_DIR}/editpane.cpp"
    "${PROJECT_SOURCE_DIR}/editsframe.cpp"
    "${PROJECT_SOURCE_DIR}/errs.cpp"
    "${PROJECT_SOURCE_DIR}/extend.cpp"
    "${PROJECT_SOURCE_DIR}/extend_no.cpp"
    "${PROJECT_SOURCE_DIR}/files.cpp"
    "${PROJECT_SOURCE_DIR}/flagsdlg.cpp"
    "${PROJECT_SOURCE_DIR}/hash.cpp"
    "${PROJECT_SOURCE_DIR}/hexfilterdlg.cpp"
    "${PROJECT_SOURCE_DIR}/listcoledit.cpp"
    "${PROJECT_SOURCE_DIR}/listpane.cpp"
    "${PROJECT_SOURCE_DIR}/mapframe.cpp"
    "${PROJECT_SOURCE_DIR}/mappane.cpp"
    "${PROJECT_SOURCE_DIR}/msgframe.cpp"
    "${PROJECT_SOURCE_DIR}/objs.cpp"
    "${PROJECT_SOURCE_DIR}/optionsdlg.cpp"
    "${PROJECT_SOURCE_DIR}/py_extend.cpp"
    "${PROJECT_SOURCE_DIR}/receivedlg.cpp"
    "${PROJECT_SOURCE_DIR}/routeplanner.cpp"
    "${PROJECT_SOURCE_DIR}/shaftsframe.cpp"
    "${PROJECT_SOURCE_DIR}/unitfilterdlg.cpp"
    "${PROJECT_SOURCE_DIR}/unitframe.cpp"
    "${PROJECT_SOURCE_DIR}/unitframefltr.cpp"
    "${PROJECT_SOURCE_DIR}/unitpane.cpp"
    "${PROJECT_SOURCE_DIR}/unitpanefltr.cpp"
    "${PROJECT_SOURCE_DIR}/unitsplitdlg.cpp"
    "${PROJECT_SOURCE_DIR}/utildlgs.cpp"
    "${PROJECT_SOURCE_DIR}/order_parser.cpp"
    "${PROJECT_SOURCE_DIR}/autologic.cpp"
    "${PROJECT_SOURCE_DIR}/autonaming.cpp"
    "${PROJECT_SOURCE_DIR}/unit_to_pane_control.cpp"
    "${PROJECT_SOURCE_DIR}/cfg_loader.cpp"
   )

set(AH_HEADERS
    "${PROJECT_SOURCE_DIR}/ahapp.h"
    "${PROJECT_SOURCE_DIR}/ah_control.h"
    "${PROJECT_SOURCE_DIR}/ahframe.h"
    "${PROJECT_SOURCE_DIR}/atlaparser.h"
    "${PROJECT_SOURCE_DIR}/bool.h"
    "${PROJECT_SOURCE_DIR}/cfgfile.h"
    "${PROJECT_SOURCE_DIR}/collection.h"
    "${PROJECT_SOURCE_DIR}/compat.h"
    "${PROJECT_SOURCE_DIR}/config.h"
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/consts_ah.h"
    "${PROJECT_SOURCE_DIR}/consts.h"
    "${PROJECT_SOURCE_DIR}/createnewunitdlg.h"
    "${PROJECT_SOURCE_DIR}/cstr.h"
    "${PROJECT_SOURCE_DIR}/data_control.h"
    "${PROJECT_SOURCE_DIR}/data.h"
    "${PROJECT_SOURCE_DIR}/editpane.h"
    "${PROJECT_SOURCE_DIR}/editsframe.h"
    "${PROJECT_SOURCE_DIR}/errs.h"
    "${PROJECT_SOURCE_DIR}/extend.h"
    "${PROJECT_SOURCE_DIR}/files.h"
    "${PROJECT_SOURCE_DIR}/flagsdlg.h"
    "${PROJECT_SOURCE_DIR}/hash.h"
    "${PROJECT_SOURCE_DIR}/hexfilterdlg.h"
    "${PROJECT_SOURCE_DIR}/listcoledit.h"
    "${PROJECT_SOURCE_DIR}/listpane.h"
    "${PROJECT_SOURCE_DIR}/mapframe.h"
    "${PROJECT_SOURCE_DIR}/mappane.h"
    "${PROJECT_SOURCE_DIR}/msgframe.h"
    "${PROJECT_SOURCE_DIR}/objs.h"
    "${PROJECT_SOURCE_DIR}/optionsdlg.h"
    "${PROJECT_SOURCE_DIR}/receivedlg.h"
    "${PROJECT_SOURCE_DIR}/routeplanner.h"
    "${PROJECT_SOURCE_DIR}/shaftsframe.h"
    "${PROJECT_SOURCE_DIR}/stdhdr.h"
    "${PROJECT_SOURCE_DIR}/unitfilterdlg.h"
    "${PROJECT_SOURCE_DIR}/unitframefltr.h"
    "${PROJECT_SOURCE_DIR}/unitframe.h"
    "${PROJECT_SOURCE_DIR}/unitpanefltr.h"
    "${PROJECT_SOURCE_DIR}/unitpane.h"
    "${PROJECT_SOURCE_DIR}/unitsplitdlg.h"
    "${PROJECT_SOURCE_DIR}/utildlgs.h"
    "${PROJECT_SOURCE_DIR}/order_parser.h"
    "${PROJECT_SOURCE_DIR}/autologic.h"
    "${PROJECT_SOURCE_DIR}/autonaming.h"
    "${PROJECT_SOURCE_DIR}/unit_to_pane_control.h"
    "${PROJECT_SOURCE_DIR}/cfg_loader.h"
   )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# -Wconversion
# -Wcast-qual
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -ftrapv -Wpointer-arith -Wuninitialized -Winit-self -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-fstrong-eval-order -Wcomma-subscript -fstack-protector-all")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-fstrong-eval-order -Wcomma-subscript")

option(USE_PYTHON "Python library" OFF)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
endif()

if(MSVC)
    add_definitions(/MP)
endif()

if(MSVC AND NOT WXSOURCE)
    include(${CMAKE_SOURCE_DIR}/winmsvc.cmake)
else()
    find_package(wxWidgets REQUIRED COMPONENTS net core base)
endif()

configure_file(config.h.in config.h)

set(AH_LINK_LIST
    ${wxWidgets_LIBRARIES}
)

if(USE_PYTHON)
    find_package(PythonLibs REQUIRED)
    include_directories(${PYTHON_INCLUDE_DIRS})
    list(APPEND AH_LINK_LIST ${PYTHON_LIBRARIES})
    message("Python: ${PYTHON_INCLUDE_DIRS}")
endif()

if(NOT MSVC OR WXSOURCE)
    include(${wxWidgets_USE_FILE})
endif()

# add the executable
if(WIN32)
message("WIN32: ${WIN32}")
add_executable(ah ${AH_CPP} ${AH_HEADERS} ${PROJECT_SOURCE_DIR}/ah.rc)
set_target_properties(${PROJECT_NAME} PROPERTIES
LINK_FLAGS /SUBSYSTEM:WINDOWS
)
else()
add_executable(ah ${AH_CPP} ${AH_HEADERS})
endif()
target_link_libraries(ah ${AH_LINK_LIST})
