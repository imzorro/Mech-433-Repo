add_executable(HW12
        RCServoPWM.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(HW12 pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(HW12)

