#include <iostream>
#include <regex>
#include <string>
#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "manage/handler.h"




using namespace std;









void run() {

  /* Create Router for HTTP requests routing */
  auto router = oatpp::web::server::HttpRouter::createShared();

  /* Route POST - "/addTorrent" & GET - "/getInfo" requests to Handler */
  router->route("POST", "/addTorrent", std::make_shared<HandlerAdd>());
  router->route("GET", "/getInfo", std::make_shared<HandlerGet>());

  /* Create HTTP connection handler with router */
  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  /* Create TCP connection provider */
  auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 7567, oatpp::network::Address::IP_4});

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  OATPP_LOGI("Deluge API", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server.run();
}



int main(int argc, char const *argv[])
{

	

		 /* Init oatpp Environment */
	  oatpp::base::Environment::init();

	  /* Run App */
	  run();

	  /* Destroy oatpp Environment */
	  oatpp::base::Environment::destroy();


	

	






	return 0;
}

