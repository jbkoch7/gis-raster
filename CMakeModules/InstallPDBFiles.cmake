
#On windows provide the user with the pdb files for debugging if they are present
if( MSVC )
  #Debug symbols are only created for executable and shared library targets
  if( ${TARGET_CATEGORY} STREQUAL "Swig" )
    get_target_property( TARGET_TYPE ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME} TYPE )
    get_target_property( PDB ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME} PDB_NAME )
  else()
    get_target_property( TARGET_TYPE ${TARGET_NAME} TYPE )
    get_target_property( PDB ${TARGET_NAME} PDB_NAME )
  endif()
  if( ${TARGET_TYPE} STREQUAL "STATIC_LIBRARY" )
    return()
  endif()
  if( ${PDB} STREQUAL "PDB-NOTFOUND" )
    if( ${TARGET_CATEGORY} STREQUAL "Swig" )
      set( PDB ${SWIG_MODULE_${TARGET_NAME}_REAL_NAME} )
    else()
      set( PDB ${TARGET_NAME} )
    endif()
  endif()
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/${PDB}.pdb
    DESTINATION ${INSTALL_BINDIR}
    CONFIGURATIONS RelWithDebInfo Debug )
endif()
