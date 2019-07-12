MACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)
    # Macro to implement copy_if_different for a list of files
    # Arguments -
    #   FROM_DIR - this is the source directory
    #   TO_DIR   - this is the destination directory
    #   FILES    - names of the files to copy
    #              TODO: add globing.
    #   TARGETS  - List of targets
    #   TAGS     - Since only the file name is used
    #              to generate rules, pre-pend a user
    #              supplied tag to prevent duplicate rule errors.
    SET(AddTargets "")
    FOREACH(SRC ${FILES})
        GET_FILENAME_COMPONENT(SRCFILE ${SRC} NAME)
        # Command to copy the files to ${STEP1}/src, if changed.
        SET(TARGET  "${TAGS}/${SRCFILE}")
        IF("${FROM_DIR}" STREQUAL "")
            SET(FROM ${SRC})
        ELSE("${FROM_DIR}" STREQUAL "")
            SET(FROM ${FROM_DIR}/${SRC})
        ENDIF("${FROM_DIR}" STREQUAL "")
        IF("${TO_DIR}" STREQUAL "")
            SET(TO ${SRCFILE})
        ELSE("${TO_DIR}" STREQUAL "")
            SET(TO   ${TO_DIR}/${SRCFILE})
        ENDIF("${TO_DIR}" STREQUAL "")
        ADD_CUSTOM_COMMAND(
                OUTPUT  ${TARGET}
                COMMAND ${CMAKE_COMMAND}
                ARGS    -E copy_if_different ${FROM} ${TO}
                COMMENT "Copying ${SRCFILE} ${TO_DIR}"
        )
        SET(AddTargets ${AddTargets} ${TARGET})
    ENDFOREACH(SRC ${FILES})
    SET(${TARGETS} ${AddTargets})
ENDMACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)


macro(copy_shared_lib APP LIBRARY DLL)
	set(librayLocationVar 			${DLL}Location)
	set(librayLocationVarDebug   	${DLL}LocationDebug)
	set(librayTarget					${DLL})
	IF(NOT "${LIBRARY}" STREQUAL "")
		set(librayLocationVar 			${LIBRARY}_${DLL}Location)
		set(librayLocationVarDebug  	${LIBRARY}_${DLL}LocationDebug)
		set(librayTarget				${LIBRARY}::${DLL})
	ENDIF(NOT "${LIBRARY}" STREQUAL "")
	
		# find the release *.dll file
		get_target_property(${librayLocationVar} ${librayTarget} LOCATION)
		# find the debug *d.dll file
		get_target_property(${librayLocationVarDebug} ${librayTarget} IMPORTED_LOCATION_DEBUG)

    add_custom_command(TARGET ${APP} POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${${librayLocationVarDebug}}> $<$<NOT:$<CONFIG:Debug>>:${${librayLocationVar}}> $<TARGET_FILE_DIR:${APP}>)
endmacro()
