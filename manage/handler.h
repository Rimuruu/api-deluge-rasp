#include <iostream>
#include <regex>
#include <string>

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "../dto/torrent.h"
#include "manager.h"

using namespace std;
/** 
 * Custom Request Handler
 */
class HandlerAdd : public oatpp::web::server::HttpRequestHandler {
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
	  		oatpp::Object<Torrent> info = jsonObjectMapper->readFromString<oatpp::Object<Torrent>>(json);
	  		if(isTorrent(info)){
	  		 string magnet = info->magnet->std_str();
	  		 OATPP_LOGD("Deluge API", "strJson='%s'", info->magnet->getData());
	  		 if(isMagnet(magnet)){
	  		 	OATPP_LOGD("Deluge API", "magnet receive '%s'", "is correct");
	  		 	string r = addTorrent(info->magnet->std_str(),info->password->std_str());
	  		 	OATPP_LOGD("Deluge API", "cmd return '%s'", r.c_str());
	  		 	if(!(r.find("Failed") != std::string::npos) && r.find("Torrent added!") != std::string::npos) {
	  		 		OATPP_LOGD("Deluge API", "addTorrent respond'", info->magnet->getData());
	  		 		return ResponseFactory::createResponse(Status::CODE_200, "TORRENT ADDED\n");
	  		 	}
	  		 	else {
	  		 		return ResponseFactory::createResponse(Status::CODE_403, "UNKNOWN ERROR\n");	
	  		 	}
	  		 }
	  		 else{
	  		 	OATPP_LOGD("Deluge API", "magnet receive '%s'", "is not correct");
	  		 	return ResponseFactory::createResponse(Status::CODE_403, "MAGNET IS INCORRET\n");
	  		 }
	  		 

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
	}
	else{
		
		return ResponseFactory::createResponse(Status::CODE_403, "NOT A JSON REQUEST\n");

	}
	return ResponseFactory::createResponse(Status::CODE_403, "UNREACHABLE\n");

  }

};


class HandlerGet : public oatpp::web::server::HttpRequestHandler {
public:





 
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
  	string info_s = getInfo();
  	oatpp::String info = oatpp::String(getInfo().c_str()); 
  	return ResponseFactory::createResponse(Status::CODE_200, info);

  }

};