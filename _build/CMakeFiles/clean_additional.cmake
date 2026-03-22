# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/Perseus_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Perseus_autogen.dir/ParseCache.txt"
  "Perseus_autogen"
  )
endif()
