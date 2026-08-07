#include <iostream>
#include "includes/http/HttpRequest.hpp"
#include "includes/http/HttpResponse.hpp"
#include "includes/config/Config.hpp"

void runTest(const std::string& testName, HttpRequest& req, ServerConfig& servConf)
{
    std::cout << "========================================" << std::endl;
    std::cout << "test attendu : " << testName << std::endl;
    std::cout << "=== requete émise ===" << std::endl;
    std::cout << req.getMethod() << " " << req.getPath() << " " << req.getVersion() << std::endl;
    std::cout << "========================================" << std::endl;

    HttpResponse response;
    response.buildResponse(req, servConf);

    std::cout << "================= Test =================" << std::endl;
    std::cout << response.getResponse() << std::endl;
    std::cout << "========================================\n" << std::endl;
}

int main()
{
    std::srand(std::time(0));
    ServerConfig servConf;
    servConf.setRoot("./www");

    LocationConfig locDefault;
    locDefault.setPath("/");
    locDefault.setRoot("./www");
    locDefault.setAutoIndex(false);
    locDefault.addMethod("GET");
    servConf.addLocation(locDefault);

    LocationConfig locCssAutoOn;
    locCssAutoOn.setPath("/css/");
    locCssAutoOn.setRoot("./www");
    locCssAutoOn.setAutoIndex(true);
    locCssAutoOn.addMethod("GET");

    LocationConfig locCssAutoOff;
    locCssAutoOff.setPath("/css/");
    locCssAutoOff.setRoot("./www");
    locCssAutoOff.setAutoIndex(false);
    locCssAutoOff.addMethod("GET");
    {
        HttpRequest req;
        req.setMethod("GET");
        req.setPath("/html/Error.html");
        req.setVersion("HTTP/1.1");
        req.setState(COMPLETE);

        runTest("Test 200 ok", req, servConf);
    }
    {
        HttpRequest req;
        req.setMethod("GET");
        req.setPath("/fichier_qui_n_existe_pas.html");
        req.setVersion("HTTP/1.1");
        req.setState(COMPLETE);

        runTest("Introuvable, un 404 Not Found", req, servConf);
    }
    {
        ServerConfig servConfAutoOn = servConf;
        servConfAutoOn.addLocation(locCssAutoOn);

        HttpRequest req;
        req.setMethod("GET");
        req.setPath("/css/");
        req.setVersion("HTTP/1.1");
        req.setState(COMPLETE);

        runTest("Aucun index trouvé mais l'autoindx est On", req, servConfAutoOn);
    }
    {
        ServerConfig servConfAutoOff = servConf;
        servConfAutoOff.addLocation(locCssAutoOff);

        HttpRequest req;
        req.setMethod("GET");
        req.setPath("/css/");
        req.setVersion("HTTP/1.1");
        req.setState(COMPLETE);

        runTest("Sans index et l'Autoindex OFF donc 403 Forbidden", req, servConfAutoOff);
    }

    return 0;
}