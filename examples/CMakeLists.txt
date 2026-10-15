add_executable(sigproc_demo demo.cpp)
target_link_libraries(sigproc_demo PRIVATE sigproc)
sigproc_apply_options(sigproc_demo)
