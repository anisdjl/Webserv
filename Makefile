CXX = c++
CXXFLAGS = -std=c++98
NAME = webserv
SRC = main.cpp \
	  srcs/http/HttpRequest.cpp \
	  srcs/http/HttpResponse.cpp
	  
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