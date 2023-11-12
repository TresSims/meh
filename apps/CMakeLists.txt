add_executable(meh meh.cxx)

include_directories(${PROJECT_SOURCE_DIR}/extern/lib3mf/Bindings)

find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/Bin REQUIRED)
target_link_libraries(meh PRIVATE 3mf logic)
