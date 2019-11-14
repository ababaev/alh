cmake_minimum_required(VERSION 3.10)
project(ah, VERSION 1.0)

set(AH_CPP
    "${PROJECT_SOURCE_DIR}/ahapp.cpp"
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
    "${PROJECT_SOURCE_DIR}/version.h"
   )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#option(HAVE_PYTHON "Python library" OFF)

find_package(wxWidgets REQUIRED)
find_package(PythonLibs REQUIRED)

#if(HAVE_PYTHON)
#endif()
message("${PYTHON_INCLUDE_DIRS} ${wxWidgets_USE_FILE}")

configure_file(config.h.in config.h)

include_directories(${PYTHON_INCLUDE_DIRS})
include(${wxWidgets_USE_FILE})

# add the executable
add_executable(ah ${AH_CPP} ${AH_HEADERS})
target_link_libraries(ah ${wxWidgets_LIBRARIES} ${PYTHON_LIBRARIES})