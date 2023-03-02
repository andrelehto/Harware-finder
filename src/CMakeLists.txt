set(CMAKE_AUTOMOC ON)

add_executable(Hardware-finder
    main.cpp
    wmi_api.h
    wmi_api.cpp
)

target_link_libraries(Hardware-finder PRIVATE Qt6::Core)

IF (WIN32)
add_custom_command(TARGET Hardware-finder POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Hardware-finder> $<TARGET_FILE_DIR:Hardware-finder>
  COMMAND_EXPAND_LISTS
  )
ENDIF()