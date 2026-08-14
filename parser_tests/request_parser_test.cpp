#include "../includes/http/HttpRequest.hpp"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

const char *stateName(RequestState state)
{
	if (state == COMPLETE)
		return "COMPLETE";
	if (state == ERROR)
		return "ERROR";
	return "INCOMPLETE";
}

std::string escapeBytes(const std::string &value)
{
	std::ostringstream out;

	for (std::string::size_type i = 0; i < value.size(); ++i)
	{
		const unsigned char c = static_cast<unsigned char>(value[i]);

		if (c == '\\')
			out << "\\\\";
		else if (c == '"')
			out << "\\\"";
		else if (c == '\r')
			out << "\\r";
		else if (c == '\n')
			out << "\\n";
		else if (c == '\t')
			out << "\\t";
		else if (c >= 32 && c <= 126)
			out << static_cast<char>(c);
		else
			out << "\\x" << std::hex << std::setw(2) << std::setfill('0')
				<< static_cast<unsigned int>(c) << std::dec << std::setfill(' ');
	}
	return out.str();
}

std::string headerValue(const HttpRequest &request,
						const std::string &lower,
						const std::string &canonical)
{
	std::string value = request.getHeader(lower);

	if (value.empty())
		value = request.getHeader(canonical);
	return value;
}

void printHeader(const HttpRequest &request,
				 const std::string &lower,
				 const std::string &canonical)
{
	const std::string value = headerValue(request, lower, canonical);

	if (!value.empty())
		std::cout << "  " << lower << ": " << value << '\n';
}

void printRequest(const HttpRequest &request, unsigned int number)
{
	std::cout << "\n===== REQUEST #" << number << " =====\n";
	std::cout << "state      : " << stateName(request.getState()) << '\n';
	std::cout << "method     : " << request.getMethod() << '\n';
	std::cout << "path       : " << request.getPath() << '\n';
	std::cout << "query      : " << request.getQueryString() << '\n';
	std::cout << "version    : " << request.getVersion() << '\n';
	std::cout << "error-code : " << request.getErrorCode() << '\n';
	std::cout << "headers:\n";
	printHeader(request, "host", "Host");
	printHeader(request, "content-length", "Content-Length");
	printHeader(request, "transfer-encoding", "Transfer-Encoding");
	printHeader(request, "content-type", "Content-Type");
	printHeader(request, "connection", "Connection");
	std::cout << "body-size  : " << request.getBody().size() << '\n';
	std::cout << "body       : \"" << escapeBytes(request.getBody()) << "\"\n";
	std::cout << "buffered   : " << request.getBuffer().size() << " byte(s)\n";
}

void feedInChunks(HttpRequest &request, const std::string &raw,
				  std::string::size_type chunkSize)
{
	std::string::size_type offset = 0;

	if (chunkSize == 0)
		chunkSize = raw.size() == 0 ? 1 : raw.size();
	while (offset < raw.size() && request.getState() == INCOMPLETE)
	{
		std::string::size_type count = chunkSize;
		if (count > raw.size() - offset)
			count = raw.size() - offset;
		request.ft_parse_http_request(raw.substr(offset, count), 100000);
		offset += count;
	}
}

bool expect(bool condition, const std::string &message, std::string &reason)
{
	if (!condition)
	{
		reason = message;
		return false;
	}
	return true;
}

bool testSimpleGet(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"GET /index.html HTTP/1.1\r\n"
		"Host: localhost:8080\r\n"
		"Connection: keep-alive\r\n"
		"\r\n";

	feedInChunks(request, raw, raw.size());
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == COMPLETE, "la requete reste incomplete", reason)
		&& expect(request.getMethod() == "GET", "methode attendue: GET", reason)
		&& expect(request.getPath() == "/index.html", "chemin attendu: /index.html", reason)
		&& expect(request.getVersion() == "HTTP/1.1", "version attendue: HTTP/1.1", reason)
		&& expect(headerValue(request, "host", "Host") == "localhost:8080",
			"header Host absent ou incorrect", reason)
		&& expect(request.getBody().empty(), "un GET sans body doit avoir un body vide", reason);
}

bool testByteByByteQuery(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"GET /search?q=webserv&lang=fr HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"\r\n";

	feedInChunks(request, raw, 1);
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == COMPLETE,
		"la requete envoyee octet par octet reste incomplete", reason)
		&& expect(request.getPath() == "/search", "chemin attendu: /search", reason)
		&& expect(request.getQueryString() == "q=webserv&lang=fr",
			"query string incorrecte", reason);
}

bool testContentLengthBody(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Type: text/plain\r\n"
		"Content-Length: 5\r\n"
		"\r\n"
		"Hello";

	feedInChunks(request, raw, 3);
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == COMPLETE, "POST incomplet apres les 5 octets", reason)
		&& expect(request.getMethod() == "POST", "methode attendue: POST", reason)
		&& expect(request.getBody() == "Hello", "body attendu: Hello", reason);
}

bool testCaseInsensitiveHeaders(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"POST /empty HTTP/1.1\r\n"
		"hOsT: localhost\r\n"
		"CoNtEnT-LeNgTh: 0\r\n"
		"\r\n";

	feedInChunks(request, raw, 4);
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == COMPLETE,
		"les noms de headers doivent etre insensibles a la casse", reason)
		&& expect(request.getHeader("host") == "localhost",
			"les noms de headers devraient etre normalises en minuscules", reason)
		&& expect(request.getHeader("content-length") == "0",
			"Content-Length melange n'est pas retrouve", reason);
}

bool testChunkedBody(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"POST /chunked HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"4\r\nWiki\r\n"
		"5\r\npedia\r\n"
		"0\r\n\r\n";

	feedInChunks(request, raw, 2);
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == COMPLETE, "body chunked incomplet", reason)
		&& expect(request.getBody() == "Wikipedia",
			"le body chunked doit etre decode en Wikipedia", reason);
}

bool testIncompleteBody(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"POST /upload HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 5\r\n"
		"\r\n"
		"Hel";

	feedInChunks(request, raw, raw.size());
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == INCOMPLETE,
		"une requete avec seulement 3 octets sur 5 ne doit pas etre COMPLETE", reason);
}

bool testMissingHost(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw = "GET / HTTP/1.1\r\n\r\n";

	feedInChunks(request, raw, raw.size());
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == ERROR, "Host est obligatoire en HTTP/1.1", reason)
		&& expect(request.getErrorCode() == 400, "code attendu pour Host absent: 400", reason);
}

bool testConflictingLengthHeaders(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"POST / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Content-Length: 4\r\n"
		"Transfer-Encoding: chunked\r\n"
		"\r\n"
		"0\r\n\r\n";

	feedInChunks(request, raw, raw.size());
	if (verbose)
		printRequest(request, 1);
	return expect(request.getState() == ERROR,
		"Content-Length et Transfer-Encoding ensemble doivent etre rejetes", reason)
		&& expect(request.getErrorCode() == 400,
			"code attendu pour des longueurs conflictuelles: 400", reason);
}

bool testPipelinedRequests(std::string &reason, bool verbose)
{
	HttpRequest request;
	const std::string raw =
		"GET /one HTTP/1.1\r\nHost: localhost\r\n\r\n"
		"GET /two HTTP/1.1\r\nHost: localhost\r\n\r\n";

	request.ft_parse_http_request(raw, 100000);
	if (verbose)
		printRequest(request, 1);
	if (!expect(request.getState() == COMPLETE, "la premiere requete n'est pas COMPLETE", reason)
		|| !expect(request.getPath() == "/one", "premier chemin attendu: /one", reason)
		|| !expect(!request.getBuffer().empty(),
			"les octets de la seconde requete doivent rester dans le buffer", reason))
		return false;

	request.resetRequest();
	request.ft_parse_http_request("", 100000);
	if (verbose)
		printRequest(request, 2);
	return expect(request.getState() == COMPLETE, "la seconde requete n'est pas COMPLETE", reason)
		&& expect(request.getPath() == "/two", "second chemin attendu: /two", reason)
		&& expect(request.getBuffer().empty(), "des octets restent apres la seconde requete", reason);
}

typedef bool (*TestFunction)(std::string &, bool);

struct TestCase
{
	const char *name;
	TestFunction function;
};

int runSuite(bool verbose)
{
	const TestCase tests[] = {
		{"GET simple", testSimpleGet},
		{"recv octet par octet + query string", testByteByByteQuery},
		{"POST avec Content-Length", testContentLengthBody},
		{"headers insensibles a la casse", testCaseInsensitiveHeaders},
		{"Transfer-Encoding: chunked", testChunkedBody},
		{"body Content-Length incomplet", testIncompleteBody},
		{"Host absent", testMissingHost},
		{"Content-Length + Transfer-Encoding", testConflictingLengthHeaders},
		{"deux requetes dans le meme recv", testPipelinedRequests}
	};
	const unsigned int count = sizeof(tests) / sizeof(tests[0]);
	unsigned int passed = 0;

	for (unsigned int i = 0; i < count; ++i)
	{
		std::string reason;
		const bool ok = tests[i].function(reason, verbose);

		if (ok)
		{
			++passed;
			std::cout << "[OK]   " << tests[i].name << '\n';
		}
		else
			std::cout << "[FAIL] " << tests[i].name << " -- " << reason << '\n';
	}
	std::cout << "\nResultat: " << passed << '/' << count << " test(s) reussi(s).\n";
	return passed == count ? 0 : 1;
}

bool readAll(std::istream &input, std::string &data)
{
	std::ostringstream buffer;

	buffer << input.rdbuf();
	if (input.bad())
		return false;
	data = buffer.str();
	return true;
}

bool parseChunkSize(const char *text, std::string::size_type &chunkSize)
{
	char *end = NULL;
	const unsigned long value = std::strtoul(text, &end, 10);

	if (text[0] == '\0' || *end != '\0' || value == 0)
		return false;
	chunkSize = static_cast<std::string::size_type>(value);
	return true;
}

int inspectRaw(const std::string &raw, std::string::size_type chunkSize)
{
	HttpRequest request;
	std::string::size_type offset = 0;
	unsigned int requestNumber = 0;
	unsigned int guard = 0;
	bool sawError = false;

	while (offset < raw.size() || request.getState() != INCOMPLETE)
	{
		if (++guard > 100000)
		{
			std::cerr << "Erreur: boucle de parsing bloquee (buffer non consomme).\n";
			return 1;
		}
		if (request.getState() == COMPLETE || request.getState() == ERROR)
		{
			if (request.getState() == ERROR)
				sawError = true;
			printRequest(request, ++requestNumber);
			request.resetRequest();
			request.ft_parse_http_request("", 100000);
			continue;
		}
		if (offset < raw.size())
		{
			std::string::size_type count = chunkSize;
			if (count > raw.size() - offset)
				count = raw.size() - offset;
			request.ft_parse_http_request(raw.substr(offset, count), 100000);
			offset += count;
		}
	}

	const bool partialRequest = request.getAvancement() != NOT_STARTED
		|| !request.getBuffer().empty()
		|| !request.getMethod().empty()
		|| !request.getPath().empty()
		|| !request.getBody().empty();
	if (requestNumber == 0 || partialRequest)
	{
		std::cout << "\n===== FIN DU FLUX =====\n";
		std::cout << "state    : " << stateName(request.getState()) << '\n';
		std::cout << "buffered : " << request.getBuffer().size() << " byte(s)\n";
		if (!request.getBuffer().empty())
			std::cout << "raw      : \"" << escapeBytes(request.getBuffer()) << "\"\n";
	}
	return (!sawError && requestNumber > 0 && !partialRequest) ? 0 : 1;
}

void printUsage(const char *program)
{
	std::cerr << "Usage:\n"
		<< "  " << program << " [--verbose]\n"
		<< "  " << program << " --stdin [taille-chunk]\n"
		<< "  " << program << " --file <requete.raw> [taille-chunk]\n";
}

} // namespace

int main(int argc, char **argv)
{
	if (argc == 1)
		return runSuite(false);
	if (argc == 2 && std::string(argv[1]) == "--verbose")
		return runSuite(true);
	if (std::string(argv[1]) == "--stdin")
	{
		std::string::size_type chunkSize = 8192;
		std::string raw;

		if (argc > 3 || (argc == 3 && !parseChunkSize(argv[2], chunkSize)))
		{
			printUsage(argv[0]);
			return 2;
		}
		if (!readAll(std::cin, raw))
		{
			std::cerr << "Impossible de lire l'entree standard.\n";
			return 2;
		}
		return inspectRaw(raw, chunkSize);
	}
	if (std::string(argv[1]) == "--file" && (argc == 3 || argc == 4))
	{
		std::string::size_type chunkSize = 8192;
		std::ifstream input(argv[2], std::ios::in | std::ios::binary);
		std::string raw;

		if (argc == 4 && !parseChunkSize(argv[3], chunkSize))
		{
			printUsage(argv[0]);
			return 2;
		}
		if (!input || !readAll(input, raw))
		{
			std::cerr << "Impossible de lire le fichier: " << argv[2] << '\n';
			return 2;
		}
		return inspectRaw(raw, chunkSize);
	}
	printUsage(argv[0]);
	return 2;
}
