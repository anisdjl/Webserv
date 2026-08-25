*This project has been created as part of the 42 curriculum by adjelili, eprieur and ymoumene.*

# Webserv

## Description

Webserv is a  HTTP/1.1 server written in C++98 . The goal of the project is to understand how a web server works internally by implementing socket creation, listening, request parsing, response construction and connection management without relying on an existing web-server framework.

The server can serve static files from configured document roots and route requests according to a configuration file. It supports multiple virtual servers, URL locations, configurable HTTP methods, custom error pages, directory listings, redirects, file uploads and request-body size limits. The project also contains CGI-related configuration and implementation work for running scripts through configured interpreters.

The event-driven network layer uses Linux sockets and `epoll` to handle several client connections.

## Features

- HTTP/1.1 request-line, header and body parsing.
- `GET`, `POST` and `DELETE` request handling.
- Multiple servers and listening ports in one configuration file.
- Location-based routing with document roots and index files.
- Custom `404`, `500`, `502` and `503` error pages.
- Optional directory auto-indexing.
- HTTP redirects with the `return` directive.
- File upload directories and configurable maximum request-body sizes.
- Content-Type detection for served files.
- CGI configuration by file extension and interpreter.
- Non-blocking event handling through `epoll`.

## Requirements

- Linux or another Unix-like environment with socket and `epoll` support.
- A C++ compiler available as `c++`.
- `make`.

The Makefile compiles the project with `-Wall -Wextra -Werror -std=c++98` flags.

## Instructions

### Compilation

From the repository root:

```sh
make
```

Useful Makefile targets are:

```sh
make clean   # Remove object files
make fclean  # Remove object files and the executable
make re      # Clean and rebuild
```

### Execution

Without an argument, the server loads `default_config.conf`:

```sh
./webserv
```

A custom configuration file can be supplied as the only argument:

```sh
./webserv server.conf
```

The process keeps running and listens on the ports declared by the configuration. Stop it with `Ctrl+C`.

### Configuration example

The configuration uses `server` and `location` blocks:

```conf
server {
	listen 8080;
	host 127.0.0.1;
	server_name localhost;
	client_max_body_size 1000000;

	location / {
		root ./www;
		index index.html;
		allow_methods GET POST DELETE;
		autoindex off;
	}
}
```

Available directives include `listen`, `host`, `server_name`, `client_max_body_size`, `error_page`, `root`, `index`, `allow_methods`, `autoindex`, `return`, `upload_store` and `cgi_pass`. Paths are interpreted by the server as written, so make sure the document roots and upload directories exist and have the required permissions.

## Testing the server

With the default configuration, the example website is available on port `2500`:

```sh
curl http://127.0.0.1:2500/
curl -i http://127.0.0.1:2500/index.html
curl -i -X DELETE http://127.0.0.1:2500/file.txt
```

Advanced Test

Install siege :
```shell
gcl https://github.com/JoeDog/siege.git siege
cd siege
utils/bootstrap
./configure 
make
make install
```
Siege configuration :

```shell
cat << 'EOF' > ~/.siegerc
benchmark = true
concurrents = 50
connection-timeout = 10
connection = keep-alive
protocol = HTTP/1.1
verbose = true
show-fields = true
EOF
```
Launch the test :
```shell
# Test define URL
siege -b -c 100 -t 30s -f urls.txt
# Test the entire website
 siege -b -c 100 -t 30s http://127.0.0.1:2500/
# By default siege only use get, test with POST :
siege -b -c 100 -t 30s "http://127.0.0.1:2500/upload POST data=test_payload"
```
**/!\ The test result depends on the config file**
### Some curl test
#### Post Test :

Invalid post
```
# Invalid path
curl -i -X POST -d "data" http://127.0.0.1:2500/invalid-store
# Invalid Length
curl -i -X POST -H "Content-Length: abc" -d "data" http://127.0.0.1:2500/uploads
curl -i -X POST -H "Content-Length: -42" -d "data" http://127.0.0.1:2500/uploads
# Upload file
curl -i -X POST -d "File content" http://127.0.0.1:2500/uploads
```
#### Get Test :

Invalid Get test
```
# Get invalid path
curl -i http://127.0.0.1:2500/invalid-path
# Get index
curl -i http://127.0.0.1:2500/
curl -i http://127.0.0.1:2500/htmlup
```

#### Delete test
```
# Delete file
curl -i -X DELETE http://127.0.0.1:2500/uploads/file_name
curl -i -X DELETE http://127.0.0.1:2500/uploads/invalid-file
```

The actual port and document root depend on the configuration file selected at startup.

## Technical choices

- **epoll:** epoll provides event-driven monitoring of client sockets on Linux and avoids creating one blocking execution path per connection, we use epoll as  I/O Multiplexing because epoll is the standard used nowadays, he handle better a large number of connection in comparison  of other i/o Multiplexing like poll etc.

- **Configuration:** We choose to inspire our configuration of the nginx configuration, our parsing configuration follow the fail-fast principle : The configuration must be fully parsed and validated before the server starts, this prevents the server from running with an invalid configuration.

## Project structure

```sh
.
├── config_rules.cpp
├── default.conf
├── default_config.conf
├── includes
│   ├── config
│   │   └── Config.hpp
│   ├── http
│   │   ├── HttpRequest.hpp
│   │   └── HttpResponse.hpp
│   ├── socket
│   │   ├── Cgi.hpp
│   │   ├── Connection.hpp
│   │   ├── Listen.hpp
│   │   └── Socket.hpp
│   └── Webserv.hpp
├── main.cpp
├── Makefile
├── README.md
└──  srcs
     ├── config
     │   ├── ConfigCheck.cpp
     │   ├── Config.cpp
     │   ├── ConfigParsing.cpp
     │   ├── LocationConfig.cpp
     │   ├── LocationParsingUtils.cpp
     │   ├── Request.cpp
     │   ├── RequestParsing.cpp
     │   ├── ServerConfig.cpp
     │   └── ServerParsingUtils.cpp
     ├── http
     │   ├── HttpRequest
     │   │   ├── HttpRequestBody.cpp
     │   │   ├── HttpRequest.cpp
     │   │   ├── HttpRequestHeader.cpp
     │   │   ├── HttpRequestLine.cpp
     │   │   └── HttpRequestUtils.cpp
     │   └── HttpResponse
     │       ├── HttpBuild.cpp
     │       ├── HttpDeleteResponse.cpp
     │       ├── HttpErrorResponse.cpp
     │       ├── HttpGetReponse.cpp
     │       ├── HttpResponse.cpp
     │       ├── HttpsPostReponse.cpp
     │       └── HttpUtils.cpp
     ├── sockets
     │   ├── Cgi.cpp
     │   ├── CloseSockets.cpp
     │   ├── Listener.cpp
     │   └── TreatSocket.cpp
     └── Webserv.cpp
```

- `main.cpp`: validates arguments, loads the configuration and starts the server.
- `includes/`: public headers for configuration, HTTP and socket components.
- `srcs/config/`: configuration parsing and validation.
- `srcs/http/`: HTTP request parsing and response generation.
- `srcs/sockets/`: listeners, event processing, connection closing and CGI support.
- `www/`: example websites and error pages served by the project.
- `*.conf`: example server configurations.

## Resources

- [nginx - Beginners guides](https://nginx.org/en/docs/beginners_guide.html): nginx configuration file tutorial.
- [RFC 9110 - HTTP Semantics](https://www.rfc-editor.org/rfc/rfc9110): HTTP methods, status codes and general Semantics
- [How to use epoll? A complete example in C]( https://web.archive.org/web/20160303233233/https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/) : epoll tutorial.
- [RFC 9112 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112): HTTP/1.1 message syntax and connection rules.
- [MDN HTTP overview](https://developer.mozilla.org/en-US/docs/Web/HTTP): practical explanations of HTTP requests, responses and methods.
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/): socket programming concepts and examples.
- [`epoll(7)` Linux manual page](https://man7.org/linux/man-pages/man7/epoll.7.html): Epoll man.
- [Design and Implementation of a High-Performance Web Server Based on C++ and Epoll](https://www.theseus.fi/bitstream/handle/10024/893354/Zheng_Yan.pdf) :  implementation tutorial of epoll for a web server
- [RFC 3875 - CGI](https://www.rfc-editor.org/rfc/rfc3875): CGI protocol and environment conventions.

### Use of artificial intelligence

Artificial intelligence was used as a support tool during the project for:

- clarifying HTTP, socket, `epoll` and CGI documentation;
- suggesting manual test cases with `curl` and configuration examples;
- reviewing and improving the project documentation.
