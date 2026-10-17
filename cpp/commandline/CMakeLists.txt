# player.cpp defines main() and calls the program's setup() and play().
# Always static: a shared library cannot leave those two unresolved on macOS or Windows.
add_library(maximilian_player STATIC player.cpp RtAudio.cpp)
add_library(maximilian::player ALIAS maximilian_player)

target_include_directories(maximilian_player PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Threads REQUIRED)
target_link_libraries(maximilian_player PUBLIC maximilian Threads::Threads)

if(APPLE)
  target_compile_definitions(maximilian_player PRIVATE __MACOSX_CORE__)
  target_link_libraries(maximilian_player PUBLIC
    "-framework CoreAudio" "-framework CoreFoundation")
elseif(WIN32)
  target_compile_definitions(maximilian_player PRIVATE __WINDOWS_DS__)
  target_link_libraries(maximilian_player PUBLIC dsound ole32 winmm)
else()
  find_package(ALSA REQUIRED)
  target_compile_definitions(maximilian_player PRIVATE __LINUX_ALSA__)
  target_link_libraries(maximilian_player PUBLIC ALSA::ALSA)
endif()

function(maximilian_add_program name runner)
  add_executable(${name} ${ARGN})
  target_link_libraries(${name} PRIVATE ${runner})
  # Absolute path, so samples load regardless of the working directory.
  target_compile_definitions(${name} PRIVATE
    "MAXIMILIAN_SAMPLES_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${name} PRIVATE -Wall)
  endif()
endfunction()

maximilian_add_program(maximilian_main maximilian_player main.cpp)

file(GLOB example_dirs LIST_DIRECTORIES true
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/maximilian_examples
  ${CMAKE_CURRENT_SOURCE_DIR}/maximilian_examples/*)
foreach(dir IN LISTS example_dirs)
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/maximilian_examples/${dir}/main.cpp)
    string(MAKE_C_IDENTIFIER "example_${dir}" target)
    maximilian_add_program(${target} maximilian_player maximilian_examples/${dir}/main.cpp)
  endif()
endforeach()

# Smoke tests: each program runs offline for 10 seconds of audio and must not crash
# or emit NaN/inf. None of them asserts on values; see tests/ at the root for that.
if(MAXIMILIAN_BUILD_TESTS)
  # offline.cpp is the device-free counterpart of player.cpp.
  add_library(maximilian_offline STATIC offline.cpp)
  target_link_libraries(maximilian_offline PUBLIC maximilian)
  foreach(src ffttest/ffttest kuratest/kuratest mfcctest/mfcctest ringBufTest/main svftest/svftest)
    get_filename_component(dir ${src} DIRECTORY)
    maximilian_add_program(test_${dir} maximilian_offline tests/${src}.cpp)
    add_test(NAME smoke_${dir} COMMAND test_${dir})
  endforeach()

  # The same offline run for every example. 21.Recording writes its wav to the working directory.
  foreach(dir IN LISTS example_dirs)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/maximilian_examples/${dir}/main.cpp)
      string(MAKE_C_IDENTIFIER "example_${dir}" target)
      maximilian_add_program(${target}_offline maximilian_offline maximilian_examples/${dir}/main.cpp)
      add_test(NAME smoke_${target} COMMAND ${target}_offline
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()
  endforeach()
endif()
