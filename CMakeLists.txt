PROJECT( ktroublenotify )
cmake_minimum_required(VERSION 2.6)
set (QT_MIN_VERSION "4.7.0")
find_package(Qt4 REQUIRED) 
FIND_PACKAGE(KDE4 REQUIRED)
find_package(KDE4Workspace REQUIRED) 
find_package(CURL REQUIRED) 
INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} )

FIND_PATH(TIDY_INCLUDE_DIR NAMES tidy.h PATHS /usr/include /usr/include/tidy )
mark_as_advanced(TIDY_INCLUDE_DIR)

INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TIDY DEFAULT_MSG TIDY_INCLUDE_DIR)

if(TIDY_FOUND)
  SET(TIDY_INCLUDE_DIRS ${TIDY_INCLUDE_DIR})
endif(TIDY_FOUND)

INCLUDE_DIRECTORIES( ${KDE4_INCLUDES} ${TIDY_INCLUDE_DIRS})

SET(KDE4ProjectSources ktroubletray.cpp ktroubledialog.cpp main.cpp ktroublelogin.c ktroublenotify.cpp)

KDE4_ADD_EXECUTABLE(ktroublenotify ${KDE4ProjectSources} )

TARGET_LINK_LIBRARIES(ktroublenotify ${KDE4_KDEUI_LIBS} ${KDE4WORKSPACE_KWORKSPACE_LIBRARY} curl tidy)

install(TARGETS ktroublenotify DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES ktroublenotify.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES ktroublenotify.notifyrc DESTINATION ${DATA_INSTALL_DIR}/ktroublenotify )
