foreach(name ringbuf fft svf sample_io)
  add_executable(${name}_test ${name}_test.cpp)
  target_link_libraries(${name}_test PRIVATE maximilian)
  target_compile_definitions(${name}_test PRIVATE
    "MAXIMILIAN_SAMPLES_DIR=\"${PROJECT_SOURCE_DIR}/cpp/commandline\"")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${name}_test PRIVATE -Wall)
  endif()
  # Run in the build tree: sample_io_test writes wav files to its working directory.
  add_test(NAME ${name} COMMAND ${name}_test WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
