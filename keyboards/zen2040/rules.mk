OLED_TRANSPORT = custom
SRC += oled_sw_i2c.c

# Upstream oled_driver.c leaves its weak transport stubs without a return value
# when OLED_TRANSPORT=custom (they are overridden in oled_sw_i2c.c). Relax the
# warning for that one object only.
$(INTERMEDIATE_OUTPUT)/oled_driver.o: CFLAGS += -Wno-return-type
