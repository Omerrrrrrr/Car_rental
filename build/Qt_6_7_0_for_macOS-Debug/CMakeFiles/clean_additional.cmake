# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Car_rental_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Car_rental_autogen.dir/ParseCache.txt"
  "Car_rental_autogen"
  )
endif()
