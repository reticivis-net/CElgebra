# ----------------------------
# Makefile Options
# ----------------------------

NAME = CELGEBRA
ICON = icon.png
DESCRIPTION = "A true algebra calculator for the TI-84+CE"
COMPRESSED = NO
ARCHIVED = NO

# Oz is best for size, 03 is best for the speed
CFLAGS = -Wall -Wextra -O3
CXXFLAGS = -Wall -Wextra -O3

# ----------------------------

include $(shell cedev-config --makefile)
