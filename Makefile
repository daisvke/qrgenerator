# ==========================
# Build Configuration
# ==========================

NAME				=	qrgenerator
CXX					=	clang++
CXXFLAGS			=	-std=c++11 -Wall -Wextra -Werror
LDFLAGS				=	-lqrencode -lpng
RM					=	rm -rf

KEY_QRCODE_FILE		=	qrcode.png

 
# ==========================
# Source & Header Files
# ==========================

INCS		=	$(wildcard *.hpp)
SRCS		=	$(wildcard *.cpp)


# ==========================
# Object Files
# ==========================

OBJS_DIR		=	objs/
OBJS			=	$(SRCS:%.cpp=$(OBJS_DIR)%.o)


# ==========================
# Building
# ==========================

.PHONY: all clean fclean re

all: $(NAME)

# Main target
$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Object files
$(OBJS_DIR)%.o: %.cpp $(INCS)
	@mkdir -p $(dir $@)
		$(CXX) $(CXXFLAGS) -c $< -o $@


# ==========================
# Cleaning
# ==========================

clean:
	$(RM) $(OBJS_DIR_CORE) $(OBJS_DIR) $(KEY_QRCODE_FILE)

fclean: clean
	$(RM) $(NAME)

re: fclean all
