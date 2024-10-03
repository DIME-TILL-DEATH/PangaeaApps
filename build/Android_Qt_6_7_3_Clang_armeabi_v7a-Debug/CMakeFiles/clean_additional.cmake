# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "WavConverterLib\\CMakeFiles\\converter-lib_autogen.dir\\AutogenUsed.txt"
  "WavConverterLib\\CMakeFiles\\converter-lib_autogen.dir\\ParseCache.txt"
  "WavConverterLib\\converter-lib_autogen"
  "pangaea-backend\\CMakeFiles\\pangaea-backend_autogen.dir\\AutogenUsed.txt"
  "pangaea-backend\\CMakeFiles\\pangaea-backend_autogen.dir\\ParseCache.txt"
  "pangaea-backend\\pangaea-backend_autogen"
  "pangaea-mobile\\CMakeFiles\\Pangaea-mobile_autogen.dir\\AutogenUsed.txt"
  "pangaea-mobile\\CMakeFiles\\Pangaea-mobile_autogen.dir\\ParseCache.txt"
  "pangaea-mobile\\Pangaea-mobile_autogen"
  )
endif()
