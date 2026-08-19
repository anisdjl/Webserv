NAME := webserv

CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g

OBJ_DIR := .obj

SRCS := \
	main.cpp \
	srcs/Webserv.cpp \
	srcs/http/HttpRequest/HttpRequest.cpp \
	srcs/http/HttpRequest/HttpRequestBody.cpp \
	srcs/http/HttpRequest/HttpRequestHeader.cpp \
	srcs/http/HttpRequest/HttpRequestLine.cpp \
	srcs/http/HttpRequest/HttpRequestUtils.cpp \
	srcs/http/HttpResponse/HttpResponse.cpp \
	srcs/http/HttpResponse/HttpBuild.cpp \
	srcs/http/HttpResponse/HttpGetReponse.cpp \
	srcs/http/HttpResponse/HttpsPostReponse.cpp \
	srcs/http/HttpResponse/HttpDeleteResponse.cpp \
	srcs/http/HttpResponse/HttpErrorResponse.cpp \
	srcs/http/HttpResponse/HttpUtils.cpp \
	srcs/config/Config.cpp \
	srcs/config/ConfigCheck.cpp \
	srcs/config/ConfigParsing.cpp \
	srcs/config/LocationConfig.cpp \
	srcs/config/LocationParsingUtils.cpp \
	srcs/config/ServerConfig.cpp \
	srcs/config/ServerParsingUtils.cpp \
	srcs/sockets/CloseSockets.cpp \
	srcs/sockets/Listener.cpp \
	srcs/sockets/TreatSocket.cpp \
	srcs/sockets/Cgi.cpp

OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

help:
	@echo "make             compile webserv sans CGI"
	@echo "make clean       supprime les objets"
	@echo "make fclean      supprime les objets et webserv"
	@echo "make re          recompile entierement"
