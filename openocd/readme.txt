
dk6yf@precision:~/stm32/openocd$ src/./openocd -f tcl/interface/stlink-v2.cfg -f tcl/target/stm32f4x.cfg -d3

Besser: sudo src/./openocd -s tcl -f tcl/interface/stlink.cfg -f tcl/target/stm32f4x.cfg -d2



