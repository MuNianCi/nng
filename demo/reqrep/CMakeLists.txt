#
# Copyright 2025 Staysail Systems, Inc. <info@staysail.tech>
# Copyright 2018 Capitar IT Group BV <info@capitar.com>
#
# This software is supplied under the terms of the MIT License, a
# copy of which should be located in the distribution where this
# file was obtained (LICENSE.txt).  A copy of the license may also be
# found online at https://opensource.org/licenses/MIT.

# This CMakeLists file assumes you have INSTALLED NNG.
# It does not work from the NNG source tree directly.

cmake_minimum_required (VERSION 3.15)

project(reqrep)

find_package(nng CONFIG REQUIRED)

add_executable(reqrep reqrep.c)
target_link_libraries(reqrep nng::nng)
target_compile_definitions(reqrep PRIVATE NNG_ELIDE_DEPRECATED)
