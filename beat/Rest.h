#ifndef REST_H
#define REST_H
#include <string>

namespace Grasp {

/**
@class   Rest
@brief   класс для запросов через REST-протокол и просто к веб-ресурсам
*/

class Rest {

public:
	Rest();

  virtual ~Rest();

  /** GET запрос, веб-адрес ресурса */
  static std::string Get(std::string host, std::string port, std::string target, int version=11);

protected:

private:
};

} // namespace Grasp
#endif