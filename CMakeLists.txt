cmake_minimum_required(VERSION 2.8)

project(GamedevGR)

SET(OPIFEX_ENGINE_REPOSITORY CACHE STRING "../OPengine/")

# Sets up all of your application needs
# it then sets up all of the OPifex Engine links
include(CMake/application.cmake)

add_marketplace_addons_projects()



#########################################
# LAYER 9 - Application
#########################################

# Include Directories
include_directories(
	${PROJECT_SOURCE_DIR}
	.
	..
)

# Include directories for the engine itself
# Plus all of the external libraries needed
add_external_opifex_includes()


# Application Source Files
# If you change the src folder or add another, you'll need to change it here
add_source_group_recurse("${PROJECT_SOURCE_DIR}/src" "" APPLICATION_SRC)
add_asset_dir(APPLICATION_SRC)

# Application BUILD
add_opifex_executable(GamedevGR ${APPLICATION_SRC})

add_opifex_definitions(GamedevGR "")
add_opifex_defines()
set_target_properties(GamedevGR PROPERTIES OUTPUT_NAME "GamedevGR")
add_opifex_libraries(GamedevGR)
add_opifex_assets(GamedevGR)

if(${OPIFEX_OPTION_RELEASE})
  SET(BINARY_RELEASE_MODE "release")
else()
  SET(BINARY_RELEASE_MODE "debug")
endif()
add_marketplace_addons(GamedevGR "${BINARY_RELEASE_MODE}/")

opifex_engine_status_messages()
