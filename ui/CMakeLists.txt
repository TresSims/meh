find_package(OpenGL REQUIRED)

add_executable(meh_gui meh_gui.cxx MehViewer.cxx MehViewer.hpp)
find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/bin)
target_link_libraries(meh_gui ${LIB3MF} pmp_vis igfl logic)
