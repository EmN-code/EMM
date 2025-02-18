cmake_minimum_required(VERSION 3.20.0)
set(BOARD esp32_devkitc_wroom)
set(CONF_FILE "prj.conf")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_zephyr_app)

target_sources(app PRIVATE src/main.c)