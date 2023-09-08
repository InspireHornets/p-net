#********************************************************************
#        _       _         _
#  _ __ | |_  _ | |  __ _ | |__   ___
# | '__|| __|(_)| | / _` || '_ \ / __|
# | |   | |_  _ | || (_| || |_) |\__ \
# |_|    \__|(_)|_| \__,_||_.__/ |___/
#
# http://www.rt-labs.com
# Copyright 2017 rt-labs AB, Sweden.
# See LICENSE file in the project root for full license information.
#*******************************************************************/

add_executable(setpoint_interface "")
set_target_properties(profinet setpoint_interface
        PROPERTIES
        C_STANDARD 99
        )

target_include_directories(setpoint_interface
        PRIVATE
        .
        ${PROFINET_SOURCE_DIR}/src/ports/linux
        ${PROFINET_SOURCE_DIR}/src
        ${PROFINET_BINARY_DIR}/src
        )

target_sources(setpoint_interface
        PRIVATE
        app_data.c
        app_gsdml.c
        app_log.c
        app_utils.c
        sampleapp_common.c
        utils.c
        sampleapp_main.c
        commands.c
        udp_server.c
        )

target_compile_options(setpoint_interface
        PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wno-unused-parameter
        -ffunction-sections
        -fdata-sections
        )

target_link_options(setpoint_interface
        PRIVATE
        -Wl,--gc-sections
        )

file(COPY ${PROFINET_SOURCE_DIR}/src/ports/linux/set_network_parameters DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

target_link_libraries(setpoint_interface PUBLIC profinet)

install(TARGETS setpoint_interface DESTINATION bin)
