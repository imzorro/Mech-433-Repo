add_executable(HW2
        blink.c       # Name of c file
        )

# pull in common dependencies
target_link_libraries(HW2 pico_stdlib)  # First name of c file + pico_stdlib

# create map/bin/hex file etc.
pico_add_extra_outputs(HW2)