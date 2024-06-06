N_DIM ?= 3

# Build tools
AR := ar rcs
CC := gcc
CFLAGS = -iquote include -DN_DIM=$(N_DIM)

# Directory to put built files
BUILD := build
DIM_BUILD = $(addsuffix d, $(addprefix $(BUILD)/, $(N_DIM)))
SRC_SRCS := $(addprefix src/,		\
		vector.c		\
)
SRC_OBJS := $(patsubst %.c,$(DIM_BUILD)/%.o, $(SRC_SRCS))

# build of the library
$(DIM_BUILD)/nerm: $(SRC_OBJS)
	$(AR) $(DIM_BUILD)/libnerm.a $(SRC_OBJS)

# General rule for object files
$(DIM_BUILD)/src/%.o: src/%.c $(DIM_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the various build directories
$(BUILD):
	mkdir -p $@

$(DIM_BUILD): $(BUILD)
	mkdir -p $@ $@/src

# Clean up everything
clean:
	rm -rf $(BUILD)/



# Tests

TEST = $(BUILD)/tests
TEST_SRCS := $(addprefix tests/,	\
		vector_test3.c		\
)
TEST_OBJS := $(patsubst %.c,$(BUILD)/%.o, $(TEST_SRCS))
TEST_EXES := $(patsubst %.c,$(BUILD)/%, $(TEST_SRCS))
LDFLAGS := -L$(BUILD)/$(N_DIM)d

# build and run all tests
test: $(TEST_EXES)
	@./$(TEST)/vector_test3
# General rule for test object files
$(TEST_EXES): $(TEST)/%: tests/%.c $(TEST) $(DIM_BUILD)/nerm
	$(CC) $(CFLAGS) $(LDFLAGS) -lnerm $< -o $@

# General rule for test object files
$(TEST)/%3.o: tests/%.c $(TEST) build/3/nerm
	$(CC) $(CFLAGS) $(LDFLAGS) -lnerm -c $< -o $@

$(TEST): $(BUILD)
	@mkdir -p $(TEST)

