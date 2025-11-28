CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage(
  NAME Py2Cpp
  GIT_TAG 1.5.1
  GITHUB_REPOSITORY luk036/py2cpp
  OPTIONS "INSTALL_ONLY YES" # create an installable target
)

set(SPECIFIC_LIBS Py2Cpp::Py2Cpp fmt::fmt)
