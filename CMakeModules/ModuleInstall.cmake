#Required variables:
  #TARGET_NAME
  #TARGET_VERSION
  #TARGET_EXPORT
  #TARGET_CATEGORY - App/Lib/Swig
  #TARGET_LANGUAGE - C/CXX/Fortran/...
  #PRIVATE_HEADERS - must be relative
  #PUBLIC_HEADERS - must be relative
  #PRIVATE_SOURCES - must be relative
  #INCDIR_NAME

#
if( NOT TARGET_LANGUAGE )
  set( TARGET_LANGUAGE CXX )
endif()

#
if( ${TARGET_CATEGORY} STREQUAL "Swig" )
  set_target_properties( ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME}
    PROPERTIES PROJECT_LABEL "${TARGET_CATEGORY} ${TARGET_NAME}" )
else()
  set_target_properties( ${TARGET_NAME}
    PROPERTIES PROJECT_LABEL "${TARGET_CATEGORY} ${TARGET_NAME}" )
endif()

#
if( WIN32 )
  foreach( HDR ${PRIVATE_HEADERS} )
    get_filename_component( PATH ${HDR} PATH )
    file( TO_NATIVE_PATH "${PATH}" PATH )
    source_group( "Header Files\\${PATH}" FILES ${HDR} )
  endforeach()

  foreach( SRC ${PRIVATE_SOURCES} )
    get_filename_component( PATH ${SRC} PATH )
    file( TO_NATIVE_PATH "${PATH}" PATH )
    source_group( "Source Files\\${PATH}" FILES ${SRC} )
  endforeach()

  include( InstallPDBFiles )
endif()

if( ${TARGET_CATEGORY} STREQUAL "App" OR ${TARGET_CATEGORY} STREQUAL "Test" )
  install(
    TARGETS ${TARGET_NAME}
    DESTINATION ${INSTALL_BINDIR} )
elseif( ${TARGET_CATEGORY} STREQUAL "Lib" )
  set_target_properties( ${TARGET_NAME} PROPERTIES VERSION ${TARGET_VERSION} )
  set_target_properties( ${TARGET_NAME} PROPERTIES SOVERSION ${TARGET_VERSION} )

  install(
    TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_EXPORT}
    RUNTIME DESTINATION ${INSTALL_BINDIR}
    LIBRARY DESTINATION ${INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${INSTALL_LIBDIR} )

  if( INCDIR_NAME )
    foreach( HDR ${PUBLIC_HEADERS} )
      get_filename_component( PATH ${HDR} PATH )
      install(
        FILES ${HDR}
        DESTINATION ${INSTALL_INCDIR}/${INCDIR_NAME}/${PATH} )
    endforeach()
  endif()
elseif( ${TARGET_CATEGORY} STREQUAL "Swig" )
  if( WIN32 )
    #Output dlls to bin directory for modules
    set( MODULE_OUTPUT_DIRECTORY ${INSTALL_BINDIR} )
  else()
    set( MODULE_OUTPUT_DIRECTORY ${INSTALL_LIBDIR} )
  endif()

  install(
    TARGETS ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME}
    EXPORT ${TARGET_EXPORT}
    RUNTIME DESTINATION ${INSTALL_BINDIR}
    LIBRARY DESTINATION ${MODULE_OUTPUT_DIRECTORY}
    ARCHIVE DESTINATION ${INSTALL_LIBDIR} )

  #Install interface files
  file( RELATIVE_PATH
    RELTOSRC "${CMAKE_SOURCE_DIR}/src" ${CMAKE_CURRENT_SOURCE_DIR} )
  file( GLOB INTERFACE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.i" )
  foreach( INTF ${INTERFACE_FILES} )
    install(
      FILES ${INTF}
      DESTINATION ${INSTALL_INCDIR}/${RELTOSRC} )
  endforeach()

  if( ${SWIG_LANGUAGE} STREQUAL "PYTHON" )
    install(
      DIRECTORY ${CMAKE_SWIG_OUTDIR}/
      DESTINATION ${MODULE_OUTPUT_DIRECTORY}
      FILES_MATCHING PATTERN "*.py" )
  elseif(  ${SWIG_LANGUAGE} STREQUAL "RUBY" )
    set_target_properties( ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME}
      PROPERTIES PREFIX "" )
  endif()
endif()
