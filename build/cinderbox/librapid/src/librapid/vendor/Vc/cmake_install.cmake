# Install script for directory: C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/FractalRenderer")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/Debug/Vc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/Release/Vc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/MinSizeRel/Vc.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/RelWithDebInfo/Vc.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Vc" TYPE DIRECTORY FILES "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/" FILES_MATCHING REGEX "/*.(h|tcc|def)$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Vc" TYPE FILE FILES
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/Allocator"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/IO"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/Memory"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/SimdArray"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/Utils"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/Vc"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/algorithm"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/array"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/iterators"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/limits"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/simdize"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/span"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/type_traits"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/Vc/vector"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc/VcTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc/VcTargets.cmake"
         "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc/VcTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc/VcTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/CMakeFiles/Export/lib/cmake/Vc/VcTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/Vc" TYPE FILE FILES
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/UserWarning.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/VcMacros.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/AddCompilerFlag.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/CheckCCompilerFlag.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/CheckCXXCompilerFlag.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/OptimizeForArchitecture.cmake"
    "C:/dev/FractalRenderer/cinderbox/librapid/src/librapid/vendor/Vc/cmake/FindVc.cmake"
    "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/cmake/VcConfig.cmake"
    "C:/dev/FractalRenderer/build/cinderbox/librapid/src/librapid/vendor/Vc/cmake/VcConfigVersion.cmake"
    )
endif()

