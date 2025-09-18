# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-src"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-build"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/tmp"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/src"
  "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/User/Desktop/Dev/F125_UDP_Parser_And_Telemetry/build/_deps/asio-subbuild/asio-populate-prefix/src/asio-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
