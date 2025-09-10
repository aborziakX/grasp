// POptimizer.h: интерфейс для POptimizer.cpp.
// Это класс для оптимизации методом градиентного спуска
// пространство (a1,...,an) 

#if !defined(AFX_POptimizer_H__B4AF65D7_55F3_4BE3_AA0D_03B17D2FEC3D__INCLUDED_)
#define AFX_POptimizer_H__B4AF65D7_55F3_4BE3_AA0D_03B17D2FEC3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//максимальное число параметров
#define PO_SIZE 10
//максимальная размерность данных
#define DATA_SIZE 10000

class POptimizer  
{
public:
	POptimizer();
	virtual ~POptimizer();

    int load( const char * fname ); //загрузить данные из файла
	int run( int iMode, int NRANDOM, int NTIMES ); //сделать оптимизацию

	double TRESHOLD; //если разница функционалов меньше, то прервать
	double DT; //начальный шаг
	double DIAMETER = 1.0; //диаметр для случайного разброса параметров

	int			m_numParam;	    //число параметров (до PO_SIZE)
	double		m_dParams[ PO_SIZE ];//массив для параметров
	double (*m_funcExternal) ( double * dParams );	//указатель на функцию вычисления функционала
	bool (*m_funcOverlimits) ( double * dParams );	//указатель на функцию проверки на ограничения

	void (*m_funcGridValue) ( int i, int mod, double * dParams );	//указатель на функцию для заполнения dParams

private:

    double*     m_dData[ PO_SIZE ];  //массив указателей на данные
	int			m_numData;	    //размер данных

    double functional( double * dParams );	//встроенный функционал для линейного разделения

	double doGradient( int NRANDOM, int NTIMES );
	double doGrid( int NTIMES );
};

#endif // !defined(AFX_POptimizer_H__B4AF65D7_55F3_4BE3_AA0D_03B17D2FEC3D__INCLUDED_)
