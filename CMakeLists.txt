cmake_minimum_required(VERSION 3.31)
project(basic C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(basic
        common.h
        config.c
        config.h
        incident.c
        incident.h
        log.c
        log.h
        map.c
        map.h
        simulation.h
        unit.c
        unit.h
        utils.c
        utils.h
        simulation.c
        main.c)
