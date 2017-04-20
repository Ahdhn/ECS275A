set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
PROJECT( Specter )
cmake_minimum_required(VERSION 3.6)
# Find the source code and build the program.  Note that for the command
# line tools, when you add new source files, you may want to re-run cmake
# to scan for them and update the Makefile.  The generated Visual Studio
# project should do this and update itself automatically -- click yes to
# let it reload.
FILE( GLOB SRC_FILES *.cc *.h )
ADD_EXECUTABLE( specter ${SRC_FILES} )

# If this is the first time that CMake has been run, then pick a reasonable
# set of optimization flags appropriate for the compiler we've found.
IF ( NOT FLAGS_SET )
  SET( FLAGS_SET ON CACHE INTERNAL "Whether we've already forced flags before" FORCE )
  GET_FILENAME_COMPONENT( COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME_WE )
  # GCC or Intel C++...
  IF( COMPILER_NAME STREQUAL "g++" OR COMPILER_NAME STREQUAL "icpc" )
    SET( CMAKE_CXX_FLAGS_RELEASE "-Wall -g -O2" CACHE STRING "Optimized flags" FORCE )
  ENDIF( COMPILER_NAME STREQUAL "g++" OR COMPILER_NAME STREQUAL "icpc" )
  # Visual Studio ...
  IF( COMPILER_NAME STREQUAL "cl" )
    SET( CMAKE_CXX_FLAGS_RELEASE "/Ox /GL /nologo /c /TP" CACHE STRING "Optimized flags" FORCE )
    SET( CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG" CACHE STRING "Optimized flags" FORCE )
  ENDIF( COMPILER_NAME STREQUAL "cl" )
ENDIF( NOT FLAGS_SET )