# CMakeLists.txt
#
# Wireshark - Network traffic analyzer
# By Gerald Combs <gerald@wireshark.org>
# Copyright 1998 Gerald Combs
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

# This info is from moduleinfo.nmake
set(PACKAGE unistim)
set(MODULE_VERSION_MAJOR 0)
set(MODULE_VERSION_MINOR 0)
set(MODULE_VERSION_MICRO 2)
set(MODULE_VERSION_EXTRA 0)
set(MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_MICRO}.${MODULE_VERSION_EXTRA}")
set(RC_MODULE_VERSION "${MODULE_VERSION_MAJOR},${MODULE_VERSION_MINOR},${MODULE_VERSION_MICRO},${MODULE_VERSION_EXTRA}")

# This info is from Makefile.common
set(PLUGIN_NAME ${PACKAGE})

# This info is from config.nmake
set(MSVC_VARIANT "${CMAKE_GENERATOR}")

# The rc.in requires a plain VERSION variable
set(VERSION ${PROJECT_VERSION})

# Create the plugin.rc file from the template
configure_file(plugin.rc.in plugin.rc @ONLY)

set(DISSECTOR_SRC
	packet-unistim.c
)

set(PLUGIN_FILES
	plugin.c
	${DISSECTOR_SRC}
)

set(CLEAN_FILES
	${PLUGIN_FILES}
)

if (WERROR)
	set_source_files_properties(
		${CLEAN_FILES}
		PROPERTIES
		COMPILE_FLAGS -Werror
	)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

register_dissector_files(plugin.c
	plugin
	${DISSECTOR_SRC}
)

add_library(unistim ${LINK_MODE_MODULE}
	${PLUGIN_FILES}
	${CMAKE_CURRENT_BINARY_DIR}/plugin.rc
)

set_target_properties(unistim PROPERTIES
	PREFIX ""
	LINK_FLAGS "${WS_LINK_FLAGS}"
	FOLDER "Plugins"
)

if(ENABLE_APPLICATION_BUNDLE)
	set_target_properties(unistim PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/run/Wireshark.app/Contents/PlugIns/wireshark
	)
endif()

target_link_libraries(unistim epan)

install(TARGETS unistim
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/@CPACK_PACKAGE_NAME@/plugins/${CPACK_PACKAGE_VERSION} NAMELINK_SKIP
	RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/@CPACK_PACKAGE_NAME@/plugins/${CPACK_PACKAGE_VERSION}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/@CPACK_PACKAGE_NAME@/plugins/${CPACK_PACKAGE_VERSION}
)

