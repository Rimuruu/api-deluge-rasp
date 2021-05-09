#include <iostream>
#include <regex>
#include <string>
#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "dto/torrent.h"
#include "manage/manager.h"

#define PASSWORD "123"



using namespace std;
/** 
 * Custom Request Handler
 */
class Handler : public oatpp::web::server::HttpRequestHandler {
public:



	bool isJsonRequest (const std::shared_ptr<IncomingRequest>& request){
		if(request->getHeader("Content-Type")){
			
			return request->getHeader("Content-Type")->equals("application/json");
		}
		else return false;
	}

	bool isMagnet(string magnet){
	string pattern = "magnet:\\?xt=urn:btih:(.)*";
	regex reg(pattern);
	cout << regex_match(magnet,reg) <<endl;
	return regex_match(magnet,reg);
	}

	bool isTorrent(oatpp::Object<Torrent> info){
		return (info->magnet && info->password);
	}

  /**
   * Handle incoming request and return outgoing response.
   */
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
  	if(isJsonRequest(request)){

  		OATPP_LOGI("Deluge API","Request %s",request->getHeader("Content-Type")->getData());
	  	try{
	  		auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
			auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

			deserializeConfig->allowUnknownFields = true;
			serializeConfig->useBeautifier = true;

	  		auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializeConfig, deserializeConfig);
	  		oatpp::String json = request->readBodyToString();

	  		OATPP_LOGD("json", "value='%s'", json->c_str());
	  		OATPP_LOGI("Deluge API","Request Decode %s",json);

	  		oatpp::Object<Torrent> info = jsonObjectMapper->readFromString<oatpp::Object<Torrent>>(json);
	  		if(isTorrent(info)){

	  		 OATPP_LOGD("Deluge API", "strJson='%s'", info->magnet->getData());
	  		 addTorrent(info->magnet->std_str(),info->password->std_str());

	  		}
	  		else{
	  				return ResponseFactory::createResponse(Status::CODE_403, "NOT MAGNET OR PASSWORD\n");
	  			}
	  	}catch(oatpp::parser::ParsingError e){

	  		OATPP_LOGI("Deluge API","Request %s",e.getMessage()->getData());
	  		return ResponseFactory::createResponse(Status::CODE_403, "JSON PARSE ERROR\n");

	  	}
	  	catch(...){

	  		OATPP_LOGI("Deluge API","Request %s","Other error");
	  		return ResponseFactory::createResponse(Status::CODE_403, "UNKNOWN ERROR\n");

	  	}

	    return ResponseFactory::createResponse(Status::CODE_200, "Hello World!\n");
	}
	else{
		
		return ResponseFactory::createResponse(Status::CODE_403, "NOT A JSON REQUEST\n");

	}

  }

};









void run() {

  /* Create Router for HTTP requests routing */
  auto router = oatpp::web::server::HttpRouter::createShared();

  /* Route POST - "/addTorrent" requests to Handler */
  router->route("POST", "/addTorrent", std::make_shared<Handler>());

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

