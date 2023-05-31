# CMake generated Testfile for 
# Source directory: /home/nvidia/code/armVideo
# Build directory: /home/nvidia/code/armVideo
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(help "./v4l2rtspserver" "-h")
set_tests_properties(help PROPERTIES  _BACKTRACE_TRIPLES "/home/nvidia/code/armVideo/CMakeLists.txt;159;add_test;/home/nvidia/code/armVideo/CMakeLists.txt;0;")
subdirs("libv4l2cpp")
