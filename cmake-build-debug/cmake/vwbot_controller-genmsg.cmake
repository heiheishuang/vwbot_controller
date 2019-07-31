# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "vwbot_controller: 1 messages, 0 services")

set(MSG_I_FLAGS "-Ivwbot_controller:/home/heihei/vwbot_ws/src/vwbot_controller/msg;-Imove_base_msgs:/opt/ros/kinetic/share/move_base_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Iactionlib_msgs:/opt/ros/kinetic/share/actionlib_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(vwbot_controller_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_custom_target(_vwbot_controller_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "vwbot_controller" "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" "geometry_msgs/Quaternion:geometry_msgs/PoseStamped:geometry_msgs/Pose:std_msgs/Header:geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(vwbot_controller
  "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/vwbot_controller
)

### Generating Services

### Generating Module File
_generate_module_cpp(vwbot_controller
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/vwbot_controller
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(vwbot_controller_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(vwbot_controller_generate_messages vwbot_controller_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_dependencies(vwbot_controller_generate_messages_cpp _vwbot_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(vwbot_controller_gencpp)
add_dependencies(vwbot_controller_gencpp vwbot_controller_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS vwbot_controller_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(vwbot_controller
  "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/vwbot_controller
)

### Generating Services

### Generating Module File
_generate_module_eus(vwbot_controller
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/vwbot_controller
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(vwbot_controller_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(vwbot_controller_generate_messages vwbot_controller_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_dependencies(vwbot_controller_generate_messages_eus _vwbot_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(vwbot_controller_geneus)
add_dependencies(vwbot_controller_geneus vwbot_controller_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS vwbot_controller_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(vwbot_controller
  "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/vwbot_controller
)

### Generating Services

### Generating Module File
_generate_module_lisp(vwbot_controller
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/vwbot_controller
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(vwbot_controller_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(vwbot_controller_generate_messages vwbot_controller_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_dependencies(vwbot_controller_generate_messages_lisp _vwbot_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(vwbot_controller_genlisp)
add_dependencies(vwbot_controller_genlisp vwbot_controller_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS vwbot_controller_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(vwbot_controller
  "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/vwbot_controller
)

### Generating Services

### Generating Module File
_generate_module_nodejs(vwbot_controller
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/vwbot_controller
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(vwbot_controller_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(vwbot_controller_generate_messages vwbot_controller_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_dependencies(vwbot_controller_generate_messages_nodejs _vwbot_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(vwbot_controller_gennodejs)
add_dependencies(vwbot_controller_gennodejs vwbot_controller_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS vwbot_controller_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(vwbot_controller
  "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/vwbot_controller
)

### Generating Services

### Generating Module File
_generate_module_py(vwbot_controller
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/vwbot_controller
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(vwbot_controller_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(vwbot_controller_generate_messages vwbot_controller_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/heihei/vwbot_ws/src/vwbot_controller/msg/PoseAndColor.msg" NAME_WE)
add_dependencies(vwbot_controller_generate_messages_py _vwbot_controller_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(vwbot_controller_genpy)
add_dependencies(vwbot_controller_genpy vwbot_controller_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS vwbot_controller_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/vwbot_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/vwbot_controller
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET move_base_msgs_generate_messages_cpp)
  add_dependencies(vwbot_controller_generate_messages_cpp move_base_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(vwbot_controller_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(vwbot_controller_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/vwbot_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/vwbot_controller
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET move_base_msgs_generate_messages_eus)
  add_dependencies(vwbot_controller_generate_messages_eus move_base_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(vwbot_controller_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(vwbot_controller_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/vwbot_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/vwbot_controller
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET move_base_msgs_generate_messages_lisp)
  add_dependencies(vwbot_controller_generate_messages_lisp move_base_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(vwbot_controller_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(vwbot_controller_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/vwbot_controller)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/vwbot_controller
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET move_base_msgs_generate_messages_nodejs)
  add_dependencies(vwbot_controller_generate_messages_nodejs move_base_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(vwbot_controller_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(vwbot_controller_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/vwbot_controller)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/vwbot_controller\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/vwbot_controller
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET move_base_msgs_generate_messages_py)
  add_dependencies(vwbot_controller_generate_messages_py move_base_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(vwbot_controller_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(vwbot_controller_generate_messages_py std_msgs_generate_messages_py)
endif()
