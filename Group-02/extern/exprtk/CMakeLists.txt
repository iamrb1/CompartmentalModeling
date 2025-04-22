#
# **************************************************************
# *         C++ Mathematical Expression Toolkit Library        *
# *                                                            *
# * Author: Arash Partow (1999-2024)                           *
# * URL: https://www.partow.net/programming/exprtk/index.html  *
# *                                                            *
# * Copyright notice:                                          *
# * Free use of the Mathematical Expression Toolkit Library is *
# * permitted under the guidelines and in accordance with the  *
# * most current version of the MIT License.                   *
# * https://www.opensource.org/licenses/MIT                    *
# * SPDX-License-Identifier: MIT                               *
# *                                                            *
# **************************************************************
#

cmake_minimum_required(VERSION 3.15)

# define ExprTk header-only library
add_library(
    exprtk
    INTERFACE
    include/exprtk.hpp
)

add_library(exprtk::exprtk ALIAS exprtk)

target_include_directories(
    exprtk
    INTERFACE
    ./include
)

target_compile_features(exprtk INTERFACE cxx_std_11)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "ExprTkConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)