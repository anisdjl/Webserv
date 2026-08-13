NAME := request_parser_test
CXX ?= c++
CPPFLAGS := -I../includes
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -MMD -MP

ROOT := ..
OBJ_DIR := .obj
CORE_OBJ_DIR := .core-obj

PARSER_SRCS := request_parser_test.cpp $(ROOT)/srcs/http/HttpRequest.cpp
PARSER_OBJS := $(OBJ_DIR)/request_parser_test.o $(OBJ_DIR)/HttpRequest.o
PARSER_DEPS := $(PARSER_OBJS:.o=.d)

# Tous les fichiers du projet sauf la reponse HTTP et les CGI.
# core-check compile chaque unite sans les lier, afin que les references aux
# composants volontairement exclus ne bloquent pas le controle.
CORE_SRCS := $(patsubst $(ROOT)/%,%,$(wildcard \
	$(ROOT)/*.cpp \
	$(ROOT)/srcs/*.cpp \
	$(ROOT)/srcs/*/*.cpp))
CORE_SRCS := $(filter-out \
	srcs/http/HttpResponse.cpp \
	srcs/sockets/Cgi.cpp,$(CORE_SRCS))
CORE_OBJS := $(addprefix $(CORE_OBJ_DIR)/,$(CORE_SRCS:.cpp=.o))
CORE_DEPS := $(CORE_OBJS:.o=.d)

CHUNK ?= 8192
TESTER := ../../tester

.PHONY: all test verbose inspect core-check tester-info official-tester clean fclean re help

all: $(NAME)

$(NAME): $(PARSER_OBJS)
	$(CXX) $(CXXFLAGS) $(PARSER_OBJS) -o $@

$(OBJ_DIR)/request_parser_test.o: request_parser_test.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/HttpRequest.o: $(ROOT)/srcs/http/HttpRequest.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

test: $(NAME)
	./$(NAME)

verbose: $(NAME)
	./$(NAME) --verbose

inspect: $(NAME)
ifdef FILE
	./$(NAME) --file "$(FILE)" "$(CHUNK)"
else
	./$(NAME) --stdin "$(CHUNK)"
endif

core-check: $(CORE_OBJS)
	@echo "OK: toutes les unites hors HttpResponse.cpp et Cgi.cpp compilent."

$(CORE_OBJ_DIR)/%.o: $(ROOT)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

tester-info:
	@if [ ! -f "$(TESTER)" ]; then \
		echo "Binaire introuvable: $(TESTER)"; \
		exit 1; \
	fi
	@file "$(TESTER)"
	@ls -l "$(TESTER)"
	@echo "Ce testeur controle le serveur complet (reponses et CGI inclus), pas le parseur seul."

official-tester: tester-info
	@if [ "`uname -s`" != "Linux" ]; then \
		echo "Erreur: le binaire tester fourni est un executable Linux x86-64."; \
		exit 1; \
	fi
	@if [ ! -x "$(TESTER)" ]; then \
		echo "Rendez-le executable avec: chmod +x $(TESTER)"; \
		exit 1; \
	fi
	@echo "Le webserv complet doit deja tourner dans un autre terminal."
	"$(TESTER)"

help:
	@echo "make                 compile le test du parseur"
	@echo "make test            lance les tests automatiques"
	@echo "make verbose         lance les tests et affiche chaque requete parse"
	@echo "make inspect FILE=x  parse un fichier HTTP brut (CHUNK=1 pour simuler recv)"
	@echo "make inspect          lit une requete HTTP brute sur stdin"
	@echo "make core-check      compile tout sauf reponse HTTP et CGI, sans edition de liens"
	@echo "make tester-info     inspecte le binaire tester fourni"
	@echo "make official-tester lance ce binaire quand le serveur complet est pret"

clean:
	rm -rf $(OBJ_DIR) $(CORE_OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(PARSER_DEPS) $(CORE_DEPS)
