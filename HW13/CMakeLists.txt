add_executable(HW13
        #I2C_MPU6050.c
        I2C_MPU6050_USB.c
        usb_descriptors.c
        )
# Make sure TinyUSB can find tusb_config.h
#target_include_directories(HW13 PUBLIC
        #${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies and additional i2c hardware support
target_link_libraries(HW13 pico_stdlib hardware_i2c pico_unique_id tinyusb_device tinyusb_board)
# enable usb output, disable uart output
#pico_enable_stdio_usb(HW14 1)
#pico_enable_stdio_uart(HW14 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW13)
