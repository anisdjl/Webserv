CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
NAME = webserv
SRC = main.cpp \
	  srcs/parsing/ConfigParsing.cpp \
	  srcs/parsing/Config.cpp \
	  srcs/parsing/LocationConfig.cpp \
	  srcs/parsing/ServerConfig.cpp \
	  srcs/parsing/ServerParsingUtils.cpp \
	  srcs/parsing/LocationParsingUtils.cpp \
	  srcs/parsing/ConfigCheck.cpp \
	  srcs/parsing/RequestParsing.cpp
	  
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re