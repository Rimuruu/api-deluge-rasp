#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"





#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section

class Torrent : public oatpp::DTO {

  DTO_INIT(Torrent, DTO /* extends */)

  DTO_FIELD(String, magnet,"magnet");
  DTO_FIELD(String, password,"password");

};

#include OATPP_CODEGEN_END(DTO) ///< End DTO codegen section