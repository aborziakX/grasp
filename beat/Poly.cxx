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

/**
* enumerate special cases and lexemas that take more than 1 character
*/
enum elex {
	EOL = 1000, BOTTOM, KEYWORD, NUMBER, STRING, BRACKET_BEG, BRACKET_END,
	SQ_BRACKET_BEG, SQ_BRACKET_END, CURLY_BEG, CURLY_END, UN_PLUS, UN_MINUS, DOT, E_SIGN
};

//keywords
//MaterialBinding value PER_VERTEX_INDEXED Background skyColor Coordinate3
//point Material emissiveColor IndexedFaceSet coordIndex

// get next lexema
int lexema(const char* sProg, int& ip, int len, char* buf)
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
			return BRACKET_BEG;
		}
		if (sProg[ip] == ')')
		{
			buf[0] = sProg[ip++];
			return BRACKET_END;
		}

		if (sProg[ip] == '[')
		{
			buf[0] = sProg[ip++];
			return SQ_BRACKET_BEG;
		}
		if (sProg[ip] == ']')
		{
			buf[0] = sProg[ip++];
			return SQ_BRACKET_END;
		}

		if (sProg[ip] == '{')
		{
			buf[0] = sProg[ip++];
			return CURLY_BEG;
		}
		if (sProg[ip] == '}')
		{
			buf[0] = sProg[ip++];
			return CURLY_END;
		}

		if (sProg[ip] == '+')
		{
			buf[0] = sProg[ip++];
			return UN_PLUS;
		}
		if (sProg[ip] == '-')
		{
			buf[0] = sProg[ip++];
			return UN_MINUS;
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
				strcmp(buf, "coordIndex") == 0) return KEYWORD;
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
			return NUMBER;	//number
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
			return STRING;	//string
		}

		return sProg[ip++];	//one character lexema
	}
	return EOL;
}

//создание граней
void Poly::Init() {
    GeOb::Init();

    wstring data = Utils::readFile(fname.c_str());
    string d8 = Utils::wstring_to_utf8(data);
    int len = (int)d8.size();
    char buf[100];
	std::stringstream ss;
	int lex = -1, ip;
	for (ip = 0; lex != EOL; )
	{	//read next lexema and analyze
		lex = lexema(d8.c_str(), ip, len, buf);
		cout << "code " << lex << " at pos " << ip << endl;
		if (lex < 0)
		{
			ss.clear();
			ss << "Error code " << lex << " at pos " << ip << endl;
			return Utils::ErrorLog(Utils::utf8_to_wstring(ss.str()));
		}
	}

    double radius = 0.5;
    double x0, y0, z0 = 0.0, x1, y1, z1 = 2.0 * radius, size = 1.0;
    Vec3 v0, v1, v2, v3;
    double angle_step = (MY_2PI / nSide);
    double angle;
    double dH = size;

    //боковая поверхность
    double z = z0;
    for (int j = 0; j < 1; j++)
    {
        angle = 0;
        for (int i = 0; i < nSide; i++)
        {
            //вершины
            x0 = cos(angle) * radius;
            y0 = sin(angle) * radius;
            angle += angle_step;
            x1 = cos(angle) * radius;
            y1 = sin(angle) * radius;

            v0.Copy(x0, y0, z);
            v1.Copy(x1, y1, z);
            v2.Copy(0, 0, z + dH);

            //грани
            Facet3* f0 = new Facet3();
            f0->Add(new Vec3(v0));
            f0->Add(new Vec3(v1));
            f0->Add(new Vec3(v2));
            f0->SetOwner(this);
            vecFacet.push_back(f0);
        }
        z += dH;
    }

}

} // namespace Grasp