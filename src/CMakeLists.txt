file(GLOB HEADER_LIST CONFIGURE_DEPENDS ${meh_SOURCE_DIR}/include/meh/*.hpp)

find_library(LIB3MF 3mf ${meh_SOURCE_DIR}/extern/lib3mf/bin)

add_library(shims shims.cxx ${HEADER_LIST})
target_link_libraries(shims PRIVATE ${LIB3MF} pmp)

add_library(logic logic.cxx ${HEADER_LIST})
target_link_libraries(logic PRIVATE ${LIB3MF} pmp shims)
