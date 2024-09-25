# Nombre del ejecutable
EXECUTABLE = irc_app

# Archivos fuente
SERVER_SRC = irc_server.cpp
CLIENT_SRC = irc_client.cpp
MAIN_SRC = main.cpp

# Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Regla por defecto
all: $(EXECUTABLE)

# Regla para compilar el ejecutable
$(EXECUTABLE): $(SERVER_SRC) $(CLIENT_SRC) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SERVER_SRC) $(CLIENT_SRC) $(MAIN_SRC)

# Regla para limpiar los archivos generados
clean:
	rm -f $(EXECUTABLE)

# Regla para limpieza completa
fclean: clean

# Regla para recompilar todo
re: fclean all

.PHONY: all clean fclean re
