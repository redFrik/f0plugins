
#---------
# destination
#---------

if (NOT INSTALL_DESTINATION)
	set(INSTALL_DESTINATION "${CMAKE_BINARY_DIR}")
endif()
message(STATUS "Install destination set to: ${INSTALL_DESTINATION}")

if (APPLE)
	set(INSTALL_DESTINATION_SUBDIR "osx")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(INSTALL_DESTINATION_SUBDIR "linux")
elseif (WIN32)
	set(INSTALL_DESTINATION_SUBDIR "windows")
endif()

#---------
# includes
#---------

include_directories(${SC_PATH}/include/plugin_interface)
include_directories(${SC_PATH}/include/common)
include_directories(${SC_PATH}/external_libraries/libsndfile/)

#-------
# macros
#-------

macro(FIND_AND_BUILD_PLUGINS DIR)
    file(GLOB PLUGIN_SRC RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${DIR}/*.cpp")
    foreach(plugin ${PLUGIN_SRC})
        get_filename_component(basename ${plugin} NAME_WE)
        BUILD_PLUGIN(${basename} ${plugin} "" "${DIR}")
    endforeach(plugin)
endmacro()

macro(BUILD_PLUGIN PLUGIN_NAME PLUGIN_SOURCES PLUGIN_LIBS PLUGIN_DIR)
    add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SOURCES})
    list(APPEND plugins ${PLUGIN_NAME})
    target_link_libraries(${PLUGIN_NAME} ${PLUGIN_LIBS})

    install(TARGETS ${PLUGIN_NAME}
            DESTINATION ${INSTALL_DESTINATION}/f0plugins/${PLUGIN_DIR}/${INSTALL_DESTINATION_SUBDIR}
            PERMISSIONS ${PERMISSIONS})
endmacro()

#-----------------------
# plugins
#-----------------------

set(PLUGIN_DIRS
	Astrocade
	Atari2600
	AY8910
	Beep
	Nes2
	Pokey
	RedDPCM
	RedLbyl
	RedNoise
	RedPhasor
	Slub
	SN76489
)

set(PLUGIN_DIRS_EXTRA
	MZPokey
	SID6581f
)
#if(APPLE)
#	list(APPEND PLUGIN_DIRS_EXTRA RedInfo)
#endif()

set(plugins "")

#-----------------------
# build
#-----------------------

set(CMAKE_SHARED_MODULE_PREFIX "")
set(PLUGIN_EXTENSION ".so")
# FIXME not sure about permissions for win32
# yvan: add OWNER_WRITE because we don't install to FHS
set(PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE)
if (APPLE OR WIN32)
    set(CMAKE_SHARED_MODULE_SUFFIX ".scx")
    set(PLUGIN_EXTENSION ".scx")
endif()

foreach(DIR ${PLUGIN_DIRS})
    FIND_AND_BUILD_PLUGINS(${DIR})
endforeach(DIR)

BUILD_PLUGIN(MZPokey "MZPokey/mzpokeysnd.cpp;MZPokey/remez.cpp" "" "MZPokey")

#if(APPLE)
#	find_library(IOKIT_FRAMEWORK IOKit)
#	find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
#	##target_link_libraries(RedInfo ${IOKIT_FRAMEWORK})
#	BUILD_PLUGIN(RedInfo "RedInfo/RedInfo.cpp;RedInfo/unimotion.c" "${IOKIT_FRAMEWORK};${COREFOUNDATION_FRAMEWORK}" "RedInfo")
#endif()

BUILD_PLUGIN(SID6581f "SID6581f/sid.cpp;SID6581f/sid6581.cpp;SID6581f/sidenvel.cpp;SID6581f/sidvoice.cpp" "" "SID6581f")

#-----------------------
# install
#-----------------------

# install help and sc specific files that are found in 'PLUGIN_NAME/sc' folder
foreach (DIR ${PLUGIN_DIRS};${PLUGIN_DIRS_EXTRA})
    install(DIRECTORY "${DIR}/sc/" DESTINATION "${INSTALL_DESTINATION}/f0plugins/${DIR}"
            PATTERN "*"
            PERMISSIONS ${PERMISSIONS})
endforeach(DIR)
