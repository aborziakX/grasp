#include "Poly.h"
#include "Utils.h"


namespace Grasp {

Poly::~Poly() {
  lasttime = 0.0;
  Clear();
}

// название
void Poly::GetName(char buf[33])
{
    int sz = 33;// sizeof(buf)=8;
    snprintf(buf, sz, "%s id %d", u8"Полимер,", GetIndex());
}

// получить тип следующей лексемы
int Poly::NextLexema(const char* sProg, int& ip, int len, char* buf)
{
	int i;
	buf[0] = 0;
	while (ip < len)
	{
		if (sProg[ip] == ' ' || sProg[ip] == '\t' || sProg[ip] == '\n' || sProg[ip] == '\r')
		{	//skip spaces
			ip++;
			continue;
		}

		if (sProg[ip] == '#')
		{ // skip comment #
			ip++;
			while (ip < len && (sProg[ip] != '\n' && sProg[ip] != '\r'))
			{
				ip++;
			}
			continue;
		}

		if (sProg[ip] == '(')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::BRACKET_BEG;
		}
		if (sProg[ip] == ')')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::BRACKET_END;
		}

		if (sProg[ip] == '[')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::SQ_BRACKET_BEG;
		}
		if (sProg[ip] == ']')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::SQ_BRACKET_END;
		}

		if (sProg[ip] == '{')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::CURLY_BEG;
		}
		if (sProg[ip] == '}')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::CURLY_END;
		}

		if (sProg[ip] == '+')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::UN_PLUS;
		}
		if (sProg[ip] == '-')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::UN_MINUS;
		}
		if (sProg[ip] == ',')
		{
			buf[0] = sProg[ip++];
			buf[1] = 0;
			return (int)eLex::COMMA;
		}

		if (isalpha(sProg[ip]))
		{	//keyword or function
			buf[0] = sProg[ip++];
			for (i = 1; ip < len; )
			{
				if (!isalnum(sProg[ip]) && sProg[ip] != '_') break;
				buf[i++] = sProg[ip++];
			}
			buf[i] = 0;
			if (strcmp(buf, "MaterialBinding") == 0 ||
				strcmp(buf, "value") == 0 ||
				strcmp(buf, "PER_VERTEX_INDEXED") == 0 ||
				strcmp(buf, "Background") == 0 ||
				strcmp(buf, "skyColor") == 0 ||
				strcmp(buf, "Coordinate3") == 0 ||
				strcmp(buf, "point") == 0 ||
				strcmp(buf, "Material") == 0 ||
				strcmp(buf, "emissiveColor") == 0 ||
				strcmp(buf, "IndexedFaceSet") == 0 ||
				strcmp(buf, "coordIndex") == 0) return (int)eLex::KEYWORD;
			return -5;	//bad keyword      
		}

		if (isdigit(sProg[ip]))
		{	//looks like number
			buf[0] = sProg[ip++];
			int iDot = 0, iE = 0, iEpos = -1;
			for (i = 1; ip < len; )
			{
				if (!isdigit(sProg[ip]) && sProg[ip] != '.' && 
					sProg[ip] != 'E' && sProg[ip] != '+' && sProg[ip] != '-') break;
				if (sProg[ip] == '.') iDot++;
				if (sProg[ip] == 'E')
				{
					iE++;
					iEpos = ip;
				}
				if (iDot > 1 || iE > 1 || (sProg[ip] == '+' && ip != iEpos + 1) 
					|| (sProg[ip] == '-' && ip != iEpos + 1))
				{	//error, only one dot allowed
					return -3;
				}
				buf[i++] = sProg[ip++];
			}
			buf[i] = 0;
			return (int)eLex::NUMBER;	//number
		}

		if (sProg[ip] == '"' || sProg[ip] == '\'')
		{	//looks like string
			char start = sProg[ip++];
			int end = 0;
			for (i = 0; ip < len; )
			{
				if (sProg[ip] == start) { ip++; end = 1; break; }
				buf[i++] = sProg[ip++];
			}
			if (!end) return -8;	//unterminated
			buf[i] = 0;
			return (int)eLex::STRING;	//string
		}

		return sProg[ip++];	//one character lexema
	}
	return (int)eLex::EOL;
}

// создать лексему и добавить в стек
void Poly::AddLexema2Stack(int _lex, const char* buf, double val)
{
	Lexema* lex = new Lexema();
	lex->lex = _lex;
	lex->sVal = buf;
	lex->val = val;
	vecLex.push_back(lex);
}
// удалить n лексем из стека
void Poly::RemoveLexemaStack(int n)
{
	int len = (int)vecLex.size();
	if (len == 0 || n < 1) return;
	if (n > len) n = len;
	for (int j = len - 1; len - j <= n; j--)
	{ //цикл по лексемам
		Lexema* f = vecLex[j];
		delete f;
		vecLex[j] = NULL;
	}
	vecLex.resize(len - n);
}

// удалить лексем из стека включая позицию n
void Poly::RemoveLexemaUntil(int n)
{
	int len = (int)vecLex.size();
	if (len == 0 || n < 0) return;

	for (int j = len - 1; j >= n; j--)
	{ //цикл по лексемам
		Lexema* f = vecLex[j];
		delete f;
		vecLex[j] = NULL;
	}
	vecLex.resize(n);
}

// найти KEYWORD лексему
void Poly::FindKeyword(int& ip, string& kw)
{
	int len = (int)vecLex.size();
	for (int j = len - 1; j >= 0; j--)
	{ //цикл по лексемам
		Lexema* f = vecLex[j];
		if (f->lex == (int)eLex::KEYWORD)
		{
			ip = j;
			kw = f->sVal.c_str();
			return;
		}
	}
	ip = -1;
	kw = "";
}

//создание граней - простой разбор vrml 1.0 файла
void Poly::Init() {
    GeOb::Init();

    wstring data = Utils::readFile(fname.c_str());
    string d8 = Utils::wstring_to_utf8(data);
    int len = (int)d8.size();
    char buf[1000];
	stringstream ss;
	string sTemp, sLastKeyw;
	int lexType = -1, ip, iLastKeywPos = -1;
	double val;
	vector<Vec3*> vecPoint;
	vector<Vec3*> vecColor;
	vector<int> vecFac;

	for (ip = 0; lexType != (int)eLex::EOL; )
	{	// получить тип следующей лексемы и проанализировать
		lexType = NextLexema(d8.c_str(), ip, len, buf);
		int lenLex = (int)vecLex.size();
		if (lexType < 0)
		{
			ss.clear();
			ss << "Error: lexema code " << lexType << " at pos " << ip << endl;
			cout << ss.str();
			//return Utils::ErrorLog(Utils::utf8_to_wstring(ss.str()));

			// clean stack ??
		}
		else
		{ // good next lexema
			/*if(lexType == NUMBER )
			{ 
				val = atof(buf);
				cout << "code " << lexType << ", " << val << ", pos " << ip << endl;
			}
			else
			{
				cout << "code " << lexType << ", " << buf << ", pos " << ip << endl;
			}*/

			sTemp = buf;

			if (lenLex > 0)
			{
				Lexema* lexLast = vecLex[lenLex - 1];
				if (lexType == (int)eLex::NUMBER)
				{
					val = atof(buf);
					if (lexLast->lex == (int)eLex::UN_PLUS)
					{
						lexLast->lex = (int)eLex::NUMBER;
						lexLast->val = val;
					}
					else if (lexLast->lex == (int)eLex::UN_MINUS)
					{
						lexLast->lex = (int)eLex::NUMBER;
						lexLast->val = -1 * val;
					}
					else if(lexLast->lex == (int)eLex::SQ_BRACKET_BEG ||
						lexLast->lex == (int)eLex::CURLY_BEG ||
						lexLast->lex == (int)eLex::COMMA ||
						lexLast->lex == (int)eLex::NUMBER)
					{
						AddLexema2Stack((int)eLex::NUMBER, buf, val);
					}
					else
					{
						cout << "Error: wrong number " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::CURLY_BEG)
				{ // {
					if (lexLast->lex == (int)eLex::KEYWORD)
					{
						AddLexema2Stack((int)eLex::CURLY_BEG, buf, 0);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::SQ_BRACKET_BEG)
				{ // [
					if (lexLast->lex == (int)eLex::KEYWORD)
					{
						AddLexema2Stack((int)eLex::SQ_BRACKET_BEG, buf, 0);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::KEYWORD)
				{ // ключ.
					if (lexLast->lex == (int)eLex::CURLY_BEG &&
						(sTemp == "point" || sTemp == "emissiveColor" ||
						 sTemp == "value" || sTemp == "skyColor" ||
						 sTemp == "coordIndex"))
					{
						AddLexema2Stack((int)eLex::KEYWORD, buf, 0);
						if (sTemp == "point" || sTemp == "emissiveColor" || sTemp == "skyColor" ||
							sTemp == "coordIndex")
						{
							sLastKeyw = buf;
							iLastKeywPos = (int)vecLex.size() - 1;
						}
					}
					else if (lexLast->lex == (int)eLex::KEYWORD &&
						(sTemp == "PER_VERTEX_INDEXED"))
					{
						AddLexema2Stack((int)eLex::KEYWORD, buf, 0);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::SQ_BRACKET_END)
				{ // ]
					if (lexLast->lex == (int)eLex::SQ_BRACKET_BEG)
					{ // [
						RemoveLexemaStack(2); // удалить [ и ключевое
						FindKeyword(iLastKeywPos, sLastKeyw);
					}
					else if (lexLast->lex == (int)eLex::NUMBER && vecLex.size() >= 3)
					{
						if (sLastKeyw == "point")
						{
							Vec3* v = new Vec3(vecLex[vecLex.size() - 3]->val,
								vecLex[vecLex.size() - 2]->val,
								vecLex[vecLex.size() - 1]->val);
							vecPoint.push_back(v);
						}
						else if (sLastKeyw == "emissiveColor")
						{
							Vec3* v = new Vec3(vecLex[vecLex.size() - 3]->val,
								vecLex[vecLex.size() - 2]->val,
								vecLex[vecLex.size() - 1]->val);
							vecColor.push_back(v);
						}
						else if (sLastKeyw == "coordIndex")
						{
							int ind = (int)vecLex[vecLex.size() - 1]->val;
							vecFac.push_back(ind);
						}

						if (sLastKeyw == "point" || sLastKeyw == "skyColor" || sLastKeyw == "emissiveColor")
						{
							RemoveLexemaStack(3); // удалить 3 числа
						}
						if (sLastKeyw == "coordIndex")
						{
							RemoveLexemaStack(1); // удалить 1 числа
						}
						RemoveLexemaStack(2); // удалить [ и ключевое
						FindKeyword(iLastKeywPos, sLastKeyw);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::CURLY_END)
				{ // }
					if (lexLast->lex == (int)eLex::CURLY_BEG)
					{
						RemoveLexemaStack(2); // удалить { и ключевое
						FindKeyword(iLastKeywPos, sLastKeyw);
					}
					else if (lexLast->lex == (int)eLex::KEYWORD)
					{
						RemoveLexemaUntil(iLastKeywPos); // удалить { и ключевое
						FindKeyword(iLastKeywPos, sLastKeyw);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::COMMA)
				{ // ,
					if (lexLast->lex == (int)eLex::NUMBER && vecLex.size() > 3)
					{
						if (sLastKeyw == "point")
						{
							Vec3* v = new Vec3(vecLex[vecLex.size() - 3]->val,
								vecLex[vecLex.size() - 2]->val,
								vecLex[vecLex.size() - 1]->val);
							vecPoint.push_back(v);
						}
						else if (sLastKeyw == "emissiveColor")
						{
							Vec3* v = new Vec3(vecLex[vecLex.size() - 3]->val,
								vecLex[vecLex.size() - 2]->val,
								vecLex[vecLex.size() - 1]->val);
							vecColor.push_back(v);
						}
						else if (sLastKeyw == "coordIndex")
						{
							int ind = (int)vecLex[vecLex.size() - 1]->val;
							vecFac.push_back(ind);
						}

						if (sLastKeyw == "point" || sLastKeyw == "skyColor" || sLastKeyw == "emissiveColor")
						{
							RemoveLexemaStack(3); // удалить 3 числа
						}
						if (sLastKeyw == "coordIndex")
						{
							RemoveLexemaStack(1); // удалить 1 числа
						}

					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else if (lexType == (int)eLex::UN_MINUS || lexType == (int)eLex::UN_PLUS )
				{ // + -
					if (lexLast->lex == (int)eLex::SQ_BRACKET_BEG || lexLast->lex == (int)eLex::NUMBER )
					{
						AddLexema2Stack(lexType, buf, 0);
					}
					else
					{
						cout << "Error: in syntax " << buf << " at pos " << ip << endl;
					}
				}
				else
				{
					cout << "Error: in syntax " << buf << " at pos " << ip << endl;
				}
			}
			else
			{ // empty stack
				//only MaterialBinding, Background, Coordinate3, Material, IndexedFaceSet
				if (lexType == (int)eLex::KEYWORD &&
					(sTemp == "MaterialBinding" || sTemp == "Background" || sTemp == "Coordinate3" || 
					 sTemp == "Material" || sTemp == "IndexedFaceSet"))
				{
					AddLexema2Stack((int)eLex::KEYWORD, buf, 0);
					sLastKeyw = buf;
					iLastKeywPos = 0;
				}
				else if(sTemp != "")
				{
					cout << "Error: wrong root " << buf << " at pos " << ip << endl;
				}
			}
		}
	}

	RemoveLexemaStack((int)vecLex.size()); // clean

	// build facets
	Vec3* vf[4];
	int curPoint = 0;
	int _red = 0, _green = 0, _blue = 0;
	for (int j = 0; j < vecFac.size(); j++)
	{
		int k = vecFac[j];
		if (k == -1)
		{ //грани
			if( curPoint == 0 ) continue;
			Facet3* fac = new Facet3();
			fac->Add(new Vec3(*vf[0]));
			fac->Add(new Vec3(*vf[1]));
			fac->Add(new Vec3(*vf[2]));
			if (curPoint == 4)
				fac->Add(new Vec3(*vf[3]));
			fac->SetOwner(this);

			_red = 255 *_red / curPoint;
			_green = 255 * _green / curPoint;
			_blue = 255 * _blue / curPoint;
			if (_red > 255) _red = 255;
			if (_green > 255) _green = 255;
			if (_blue > 255) _blue = 255;
			if (_red < 0) _red = 0;
			if (_green < 0) _green = 0;
			if (_blue < 0) _blue = 0;
			fac->SetColor((unsigned char)_red, (unsigned char)_green, (unsigned char)_blue);
			vecFacet.push_back(fac);

			curPoint = 0;
			_red = 0;
			_green = 0;
			_blue = 0;
		}
		else
		{ // копить точки
			if (curPoint >= 4) continue;
			Vec3* f = vecPoint[k];
			vf[curPoint++] = f;

			Vec3* f2 = vecColor[k];
			_red += (int)f2->GetX();
			_green += (int)f2->GetY();
			_blue += (int)f2->GetZ();
		}
	}

	for (int j = 0; j < vecPoint.size(); j++)
	{ //цикл по точкам
		Vec3* f = vecPoint[j];
		delete f;
		vecPoint[j] = NULL;
	}
	vecPoint.resize(0);

	for (int j = 0; j < vecColor.size(); j++)
	{ //цикл по цветам
		Vec3* f = vecColor[j];
		delete f;
		vecColor[j] = NULL;
	}
	vecColor.resize(0);
}

} // namespace Grasp