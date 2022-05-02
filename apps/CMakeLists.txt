add_executable(meh meh.cxx)

find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/bin)
target_link_libraries(meh PRIVATE ${LIB3MF} pmp logic)
