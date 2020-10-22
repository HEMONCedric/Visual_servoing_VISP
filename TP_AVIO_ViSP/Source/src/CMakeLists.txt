
#----------------------------------------------------------------------
# Create rule to copy all the headers from src to include/${PROJECT_NAME}
#----------------------------------------------------------------------
# For each header, we create a rule
FILE(GLOB HEADERS . *.h)
SET(HEADER_IN_INCLUDE_DIR "")
FOREACH(header ${HEADERS})
  GET_FILENAME_COMPONENT(headerName ${header} NAME)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${headerName}
    COMMAND ${CMAKE_COMMAND} -E copy ${header} ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${headerName}
    DEPENDS ${header}
   )
  SET(HEADER_IN_INCLUDE_DIR ${HEADER_IN_INCLUDE_DIR} ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/${headerName})
ENDFOREACH(header)

#----------------------------------------------------------------------
# Create a top level rule to copy all the headers from src to include/${PROJECT_NAME}
#----------------------------------------------------------------------
ADD_CUSTOM_TARGET(headers
  DEPENDS ${HEADER_IN_INCLUDE_DIR}
  )

#----------------------------------------------------------------------
# build rule for the executables
#----------------------------------------------------------------------
FILE(GLOB SOURCES . *.cpp)
FOREACH(source ${SOURCES})

  GET_FILENAME_COMPONENT(binary ${source} NAME_WE)
  # From source compile the binary and add link rules
  ADD_EXECUTABLE(${binary} ${source})
  ADD_DEPENDENCIES(${binary} headers)

  TARGET_INCLUDE_DIRECTORIES(${binary} PUBLIC ${${PROJECT_NAME}_INTERN_INCLUDE_DIR} ${${PROJECT_NAME}_EXTERN_INCLUDE_DIR})

  TARGET_LINK_LIBRARIES(${binary} ${${PROJECT_NAME}_INTERN_LIBS} ${${PROJECT_NAME}_EXTERN_LIBS})
  # Add test
  #ADD_TEST(${binary} ${binary})
ENDFOREACH(source)


