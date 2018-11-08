
set( TARGET_VERSION ${${CMAKE_PROJECT_NAME}_VERSION} )
set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
set( TARGET_NAME simple_test )
set( TARGET_CATEGORY Test )
set( TARGET_LANGUAGE CXX )

add_executable( ${TARGET_NAME} ${TARGET_NAME}.cpp )
target_include_directories( ${TARGET_NAME} PUBLIC
  ${CMAKE_SOURCE_DIR}/src )
target_link_libraries( ${TARGET_NAME} )

include( ModuleInstall )
