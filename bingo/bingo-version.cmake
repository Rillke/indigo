SET(BINGO_VERSION "1.7.10-dev2" CACHE INTERNAL "versions" FORCE)

IF($ENV{BUILD_NUMBER})
   SET(BINGO_BUILD_VERSION $ENV{BUILD_NUMBER})
ELSE()
   SET(BINGO_BUILD_VERSION 0)
ENDIF()

SET(BINGO_VERSION_EXT "${BINGO_VERSION}.${BINGO_BUILD_VERSION} ${PACKAGE_SUFFIX}" CACHE INTERNAL "versions" FORCE)
