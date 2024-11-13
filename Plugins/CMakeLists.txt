if (ENABLE_PLUGINS_AUDIO)
	message("-- Enable audio module.")
	file(GLOB PLUGINS_AUDIO_SOURCE_FILES 
		./Audio/*.cpp
	)

	add_library(plugin_audio SHARED ${PLUGINS_AUDIO_SOURCE_FILES})
	target_compile_definitions(plugin_audio PUBLIC -DEXPORT) 
	set_target_properties(plugin_audio PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )

	target_include_directories(plugin_audio PUBLIC engine)
	target_include_directories(plugin_audio PUBLIC ./Audio)

	target_link_libraries(plugin_audio PUBLIC engine)
	target_link_libraries(plugin_audio PUBLIC OpenAL)
	target_link_libraries(plugin_audio PUBLIC sndfile)
endif()