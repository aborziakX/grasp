#ifndef BEAT_Poly_H
#define BEAT_Poly_H

#include <vector>
#include <string>
#include "Vec3.h"
#include "GeOb.h"

using namespace std;

namespace Grasp {
/**
  типы лексемы из более 1 символа и специальные случаи
*/
enum class eLex {
    EOL = 1000, BOTTOM, KEYWORD, NUMBER, STRING, BRACKET_BEG, BRACKET_END,
    SQ_BRACKET_BEG, SQ_BRACKET_END, CURLY_BEG, CURLY_END, UN_PLUS, UN_MINUS, COMMA
};

/**
  структура для лексемы
*/
struct Lexema
{
    int lex; //!< тип лексемы
    double val; //!< для чисел
    string sVal; //!< для строк
};


/**
@class   Poly
@brief   Геометрический объект Полимер (superclass - GeOb)
*/
class Poly : public GeOb {
public:
  /**
  Конструктор Полимер с центром в (0, 0, 0)
  */
  Poly(const char * _fname)
    : GeOb(){
    geom_type = geom_type_enum::GO_POLY;
    fname = _fname;
    Init();
  };

  ~Poly();

  /**
  создание граней
  */
  void Init() override;

  /**
  название
  */
  void GetName(char buf[33]) override;

protected:
private:
    std::vector<Lexema*> vecLex; /** стек лексем */

    int NextLexema(const char* sProg, int& ip, int len, char* buf); //!< получить тип следующей лексемы
    void AddLexema2Stack(int lex, const char* buf, double val); //!< создать лексему и добавить в стек
    void RemoveLexemaStack(int n); //!< удалить n лексем из стека
    void RemoveLexemaUntil(int n); //!< удалить лексем из стека включая позицию n 
    void FindKeyword(int & ip, string & kw); //!< найти KEYWORD лексему 
};


} // namespace Grasp
#endif