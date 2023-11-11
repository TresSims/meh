file(GLOB HEADER_LIST CONFIGURE_DEPENDS ${meh_SOURCE_DIR}/include/meh/*.hpp)

include_directories(${PROJECT_SOURCE_DIR}/extern/lib3mf/Bindings)

find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/Bin)

add_library(shims shims.cxx ${HEADER_LIST})
target_link_libraries(shims PRIVATE 3mf pmp)

add_library(logic logic.cxx ${HEADER_LIST})
target_link_libraries(logic PRIVATE pmp shims)
