add_executable(HW13IMU
        I2C_MPU60502.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(HW13IMU pico_stdlib hardware_i2c)
# enable usb output, disable uart output
pico_enable_stdio_usb(HW13IMU 1)
pico_enable_stdio_uart(HW13IMU 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW13IMU)
