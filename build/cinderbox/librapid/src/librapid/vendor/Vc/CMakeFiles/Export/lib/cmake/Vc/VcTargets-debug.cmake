#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Vc::Vc" for configuration "Debug"
set_property(TARGET Vc::Vc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Vc::Vc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/Vc.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Vc::Vc )
list(APPEND _IMPORT_CHECK_FILES_FOR_Vc::Vc "${_IMPORT_PREFIX}/lib/Vc.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
