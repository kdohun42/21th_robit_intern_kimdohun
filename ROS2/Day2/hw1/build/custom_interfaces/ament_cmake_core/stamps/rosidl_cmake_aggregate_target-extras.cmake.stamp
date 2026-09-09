# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target custom_interfaces::custom_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${custom_interfaces_TARGETS}.
if(custom_interfaces_TARGETS AND NOT TARGET custom_interfaces::custom_interfaces)
  add_library(custom_interfaces::custom_interfaces INTERFACE IMPORTED)
  set_target_properties(custom_interfaces::custom_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${custom_interfaces_TARGETS}")
endif()
