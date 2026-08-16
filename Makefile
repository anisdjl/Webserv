NAME := webserv

CXX := c++
CPPFLAGS := -Iincludes
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -MMD -MP

OBJ_DIR := .obj

SRCS := \
	main.cpp \
	srcs/Webserv.cpp \
	srcs/http/HttpRequest.cpp \
	srcs/http/HttpResponse.cpp \
	srcs/http/HttpBuild.cpp \
	srcs/http/HttpGetReponse.cpp \
	srcs/http/HttpsPostReponse.cpp \
	srcs/http/HttpDeleteResponse.cpp \
	srcs/http/HttpErrorResponse.cpp \
	srcs/http/HttpUtils.cpp \
	srcs/config/Config.cpp \
	srcs/config/ConfigCheck.cpp \
	srcs/config/ConfigParsing.cpp \
	srcs/config/LocationConfig.cpp \
	srcs/config/LocationParsingUtils.cpp \
	srcs/config/ServerConfig.cpp \
	srcs/config/ServerParsingUtils.cpp \
	srcs/sockets/CloseSockets.cpp \
	srcs/sockets/Listener.cpp \
	srcs/sockets/TreatSocket.cpp

OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean fclean re parser-test help

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

parser-test:
	$(MAKE) -C parser_tests test

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C parser_tests clean

fclean: clean
	rm -f $(NAME)

re: fclean all

help:
	@echo "make             compile webserv sans CGI"
	@echo "make parser-test compile et lance les tests du parseur"
	@echo "make clean       supprime les objets"
	@echo "make fclean      supprime les objets et webserv"
	@echo "make re          recompile entierement"

-include $(DEPS)
