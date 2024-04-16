add_executable(HW5
        I2CMCP23008.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(HW5 pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW5)
