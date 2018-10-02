find_package(Doxygen)

if(NOT DOXYGEN_DOT_FOUND) 
  message(WARNING "Graphviz doesn't seem to be installed. Doxygen will not be able to generate graphs. Consider installing this package.")
endif(NOT DOXYGEN_DOT_FOUND) 

if(DOXYGEN_FOUND)
    configure_file(documentation-config.doxygen.in ${CMAKE_CURRENT_BINARY_DIR}/documentation-config.doxygen @ONLY)
    add_custom_target(doc${PROJECT_NAME}
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/documentation-config.doxygen
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation using doxygen for ${PROJECT_NAME}" VERBATIM
        SOURCES welcome.dox user-guide.dox technical-overview.dox)

    set(INSTALL_DOC_DIR ${CMAKE_BINARY_DIR}/doc/${PROJECT_NAME}/html)
    file(MAKE_DIRECTORY ${INSTALL_DOC_DIR}) # needed for install

    install(DIRECTORY ${INSTALL_DOC_DIR} DESTINATION doc/${PROJECT_NAME} COMPONENT doc${PROJECT_NAME})
endif(DOXYGEN_FOUND)
