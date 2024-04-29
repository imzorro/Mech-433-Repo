add_executable(HW7
        uart_advanced.c
        ssd1306fuctions.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(HW7 pico_stdlib hardware_uart hardware_i2c)
# create map/bin/hex file etc.
pico_add_extra_outputs(HW7)

