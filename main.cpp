#include <iostream>
#include <regex>
#include <string>
#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"


using namespace std;



/** 
 * Custom Request Handler
 */
class Handler : public oatpp::web::server::HttpRequestHandler {
public:

  /**
   * Handle incoming request and return outgoing response.
   */
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
    return ResponseFactory::createResponse(Status::CODE_200, "Hello World!");
  }

};

bool isMagnet(string magnet){
	string pattern = "magnet:\\?xt=urn:btih:(.)*";
	regex reg(pattern);
	cout << regex_match(magnet,reg) <<endl;
	return regex_match(magnet,reg);


}





void run() {

  /* Create Router for HTTP requests routing */
  auto router = oatpp::web::server::HttpRouter::createShared();

  /* Route GET - "/hello" requests to Handler */
  router->route("GET", "/hello", std::make_shared<Handler>());

  /* Create HTTP connection handler with router */
  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  /* Create TCP connection provider */
  auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 7567, oatpp::network::Address::IP_4});

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server.run();
}



int main(int argc, char const *argv[])
{

	cout << argc << endl;
	cout << argv[1] << endl;

	if (argc < 2)
	{
		cout << "No argument" << endl;
		return -1;
	}
	else if(argc > 2){
		cout << "Too much argument" << endl;
		return -1;
	}
	else if(!isMagnet(argv[1])){
		cout << "Not a magnet link" << endl;
		return -1;
	}

		 /* Init oatpp Environment */
	  oatpp::base::Environment::init();

	  /* Run App */
	  run();

	  /* Destroy oatpp Environment */
	  oatpp::base::Environment::destroy();


	

	






	return 0;
}

