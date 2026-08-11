CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98
NAME = webserv

SRC = main.cpp \
	  srcs/http/HttpRequest.cpp \
	  srcs/http/HttpResponse.cpp \
	  srcs/http/HttpUtils.cpp \
	  srcs/http/HttpErrorResponse.cpp \
	  srcs/http/HttpDeleteResponse.cpp \
	  srcs/config/Config.cpp \
      srcs/config/ServerConfig.cpp \
      srcs/config/LocationConfig.cpp

OBJ = $(SRC:.cpp=.o)

GREEN   := \033[0;32m
YELLOW  := \033[0;33m
CYAN    := \033[1;36m
RESET   := \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "\n$(GREEN)[Compilation] Compilation principale ...$(RESET)\n"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(ODIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@printf "\n$(YELLOW)[Nettoyage] Nettoyage fichiers objets ...$(RESET)\n"
	rm -rf $(OBJ)

fclean: clean
	@printf "\n$(YELLOW)[Nettoyage] Nettoyage global ...$(RESET)\n"
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re