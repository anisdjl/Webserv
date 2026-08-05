#include "../../includes/config/Config.hpp"

void	CheckConfig(Config &config)
{
	if (config.getServer().empty())
		throw std::runtime_error("Error: there must be at least one server block");
	std::vector<ServerConfig> servers = config.getServer();
	for (size_t i = 0; i < servers.size(); ++i)
		CheckServer(servers[i]);
}

void	CheckServer(ServerConfig &server)
{
	if (server.getListen().empty())
		throw std::runtime_error("Error: Server missing 'listen' directive");
	if (server.getLocations().empty())
		throw std::runtime_error("Error: Server must have at least one location block");
	
	std::vector<LocationConfig> locs = server.getLocations();
    for (size_t i = 0; i < locs.size(); ++i)
        if (locs[i].getRoot().empty() && locs[i].getReturn().empty())
            throw std::runtime_error("Error: location must have a root and a return");
}
