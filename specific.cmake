find_package(fmt CONFIG QUIET)
if(fmt_FOUND)
  message(STATUS "Found system fmt: ${fmt_DIR}")
  # Tell CPM that fmt is already handled (CPM checks CPM_PACKAGES list). Write the CACHE variable
  # directly: list(APPEND ...) creates a normal-variable shadow that does not propagate into
  # FetchContent subdirectory scopes.
  if(NOT fmt IN_LIST CPM_PACKAGES)
    set(CPM_PACKAGES
        "${CPM_PACKAGES};fmt"
        CACHE INTERNAL "" FORCE
    )
  endif()
else()
  CPMAddPackage(
    NAME fmt
    GIT_TAG 12.1.0
    GITHUB_REPOSITORY fmtlib/fmt
    OPTIONS "FMT_INSTALL YES" # create an installable target
  )
endif()

CPMAddPackage(
  NAME Py2Cpp
  GIT_TAG v1.6.3
  GITHUB_REPOSITORY luk036/py2cpp
  OPTIONS "INSTALL_ONLY YES" # create an installable target
)

set(SPECIFIC_LIBS Py2Cpp::Py2Cpp fmt::fmt)
