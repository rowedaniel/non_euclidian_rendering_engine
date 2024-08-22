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

TEST := $(BUILD)/tests
TEST_COMMON := $(TEST)/common
TEST_COMMON_SRCS = $(shell find tests/common/*)
TEST_COMMON_OBJS = $(patsubst %.c,$(BUILD)/%.o, $(TEST_COMMON_SRCS))
TEST_COMMON_EXES = $(patsubst %.c,$(BUILD)/%, $(TEST_COMMON_SRCS))

# dimension-specific tests
TEST_DIM = $(TEST)/$(N_DIM)d
TEST_SRCS = $(shell find tests/$(N_DIM)d/*)
TEST_OBJS = $(patsubst %.c,$(BUILD)/%.o, $(TEST_SRCS))
TEST_EXES = $(patsubst %.c,$(BUILD)/%, $(TEST_SRCS))
LDFLAGS = -L$(BUILD)/$(N_DIM)d -lnerm

# all tests
test: test_common
	@echo "testing 2d"
	@make -s N_DIM=2 test_dim
	@echo "testing 3d"
	@make -s N_DIM=3 test_dim
	@echo "testing 4d"
	@make -s N_DIM=4 test_dim

test_common: $(TEST_COMMON_EXES)
	@echo "testing common"
	@$(TEST_COMMON_EXES) | tests/format_test_output.sh

test_dim: $(TEST_EXES)
	@$(TEST_EXES) | tests/format_test_output.sh

# For common tests
$(TEST_COMMON_EXES): $(TEST_COMMON)/%: $(TEST_COMMON)/%.o
	$(CC) $< $(LDFLAGS) -o $@
$(TEST_COMMON_OBJS): $(TEST_COMMON)/%.o: tests/common/%.c $(TEST_COMMON) $(DIM_BUILD)/nerm
	$(CC) $(CFLAGS) -c $< -o $@

# For dimension-specific tests:
$(TEST_EXES): $(TEST_DIM)/%: $(TEST_DIM)/%.o
	$(CC) $< $(LDFLAGS) -o $@
$(TEST_OBJS): $(TEST_DIM)/%.o: tests/$(N_DIM)d/%.c $(TEST_DIM) $(DIM_BUILD)/nerm
	$(CC) $(CFLAGS) -c $< -o $@


# directory tree
$(TEST_DIM): $(TEST)
	@mkdir -p $(TEST_DIM)
$(TEST_COMMON): $(TEST)
	@mkdir -p $(TEST_COMMON)
$(TEST): $(BUILD)
	@mkdir -p $(TEST) $(TEST)/common

