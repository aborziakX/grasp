// POptimizer.cpp: реализация POptimizer класса.
// Это класс для оптимизации методом градиентного спуска
// пространство (a1,...,an) 

#include "stdafx.h"
#include "POptimizer.h"

static int s_iBad = 0; //число ошибок
static int iDebug = 0; //1 - отладочная печать

//конструктор класса
POptimizer::POptimizer()
{
    int k;
	m_numParam = 0; //число параметров (до PO_SIZE)
    for( k = 0; k < PO_SIZE; k++ ) 
    {
        m_dParams[ k ] = 0.0;
        m_dData[ k ] = new double[ DATA_SIZE ]; //выделить память под данные
    }
    m_numData = 0; //размер данных

	m_funcExternal = NULL; //указатель на функцию вычисления функционала
	m_funcOverlimits = NULL; //указатель на функцию проверки на ограничения
	m_funcGridValue = NULL; //указатель на функцию для заполнения dParams

	TRESHOLD = 0.00001; //если разница функционалов меньше, то прервать
	DT = 0.00001; //начальный шаг
}

//деструктор класса
POptimizer::~POptimizer()
{
    for( int k = 0; k < PO_SIZE; k++ ) delete [] m_dData[ k ];
	m_numParam = 0;
}

//загрузить данные из файла, последняя колонка - класс (0 or 1)
int POptimizer::load( const char * fname )
{
	FILE * fp; //дескриптор файла
	char buf[ 1024 ], * ch;
    double dTmp;
    int i = 0, k;
	//открыть файл
	if ( ( fp = fopen( fname, "r" ) ) == NULL )
	{
		printf( "POptimizer::load: ошибка открытия %s\n", fname );
		return -1;
	}
	//пока нет конца файла
    while( !feof( fp ) )
    {	//читать строку в буфер
		if( fgets( buf, sizeof( buf ), fp ) == NULL ) 
		{
			if( ferror( fp ) )
			{
                printf( "POptimizer::load: ошибка чтения в %s\n", fname );
				return -1;
			}
			break;
		}
		if( ( ch = strchr( buf, '#' ) ) != NULL ) *ch = 0;	//обрезать комментарий после #
        ch = buf;
        while( *ch != 0 && ( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++; //пропустить пробелы
		if( *ch == 0 ) continue;	//комментарий или пустая линия

        if( i++ == 0 )
        {   //найти число параметров
            for( m_numParam = 1; m_numParam < PO_SIZE; m_numParam ++ )
            {
                while( *ch != 0 && !( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++; //пропустить пробелы
                while( *ch != 0 && ( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++; //пропустить пробелы
		        if( *ch == 0 ) break;	//end of line
            }
            ch = buf;
            while( *ch != 0 && ( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++; //пропустить пробелы        
        }
     
        //анализ данных
        for( k = 0; k < m_numParam; k++ )
        {
            if( *ch != 0 ) sscanf( ch, "%lg", &dTmp );
            else dTmp = 0.0;
            m_dData[ k ][ m_numData ] = dTmp;
            while( *ch != 0 && !( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++;   //пропустить Число
            while( *ch != 0 && ( *ch == '\n' || *ch == '\r' || *ch == '\t' || *ch == ' ' ) ) ch++;    //пропустить пробелы
        }
        m_numData++;
        if( m_numData >= DATA_SIZE )
	    {
            printf( "POptimizer::load: слишком большой файл, данные обрезаны %s\n", fname );
		    break;
    	}
    }
    fclose( fp );

	if( i == 0 )
	{
        printf( "POptimizer::load: empty %s\n", fname );
		return -1;
	}
    printf( "m_numParam %d, m_numData %d\n", m_numParam, m_numData );
	/*for( i = 0; i < m_numData; i++ )
	{
    	for( k = 0; k < m_numParam; k++ )
        	printf( "m_dData[%d, %d]=%.4f\n", i, k, m_dData[k][i] );
		printf( "\n");
	}*/
    return 1;
}

//встроенный функционал для линейного разделения
double POptimizer::functional( double * dParams )
{
    s_iBad = 0;
    //предполагаем линейное разделение классов 0 и 1 гиперплоскостью:
    //a0 * x0 + a1 * x1 + ... + aN-1 * xN-1 = aN
	//класс 0 под плоскостью, класс 1 - над плоскостью
    int k, i;
	int last = m_numParam - 1; //число пространственных параметров, умножаемых на координаты точки
	//последний параметр - aN, в данных ему соответствует класс (0 или 1)
    double dBad = 0.0;
    for( i = 0; i < m_numData; i++ )
    {
        double dVal = - dParams[ last ];
        for( k = 0; k < last; k++ )
        {
//printf( "%d par[%d] %.4f, dat %.4f\n", i, k, dParams[ k ], m_dData[ k ][ i ] );
            dVal += dParams[ k ] * m_dData[ k ][ i ];
        }
//printf( "%d, dVal %.4f, class %.4f\n", i, dVal, m_dData[ last ][ i ] );
        if( ( dVal >= 0 && m_dData[ last ][ i ] == 0.0 ) ||
            ( dVal <= 0 && m_dData[ last ][ i ] > 0.0 ) )
        {	//неправильно определен класс точки
//printf( "%d, dVal %.4f, class %.4f\n", i, dVal, m_dData[ last ][ i ] );
            dVal = fabs( dVal );
            if( dVal == 0.0 ) dVal = 0.01;
//            else if( dVal > 1.0 ) dVal = log( dVal ) + 1.0;
            dBad += sqrt( dVal );
            s_iBad++;
        }
    }
//printf( "sBad=%d\n", s_iBad );
    return dBad;
}

//сделать оптимизацию
int POptimizer::run( int iMode, int NRANDOM, int NTIMES )
{
	int i;
    double rc, f = 0;
	if( m_numParam < 1 || m_numParam > PO_SIZE || NRANDOM < 1 || NRANDOM > 1000000 || NTIMES < 1 || NTIMES > 1000000) 
    {
		printf( "POptimizer::run: неверно инициализирован\n" );
        return -1;
    }
	if( iMode == 1 )
	{
		f = doGradient( NRANDOM, NTIMES );
	}
	else if( iMode == 2 )
	{	//grid
		if( m_funcGridValue == NULL ) return -1;
		f = doGrid( NTIMES );
	}
    else 
	{
		printf( "POptimizer::run: неверно iMode\n" );
        return -2;
    }
	//повторное вычисление
	rc = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );
	printf( "func %.6f, iBad %d, f1 %.6f\n", rc, s_iBad, f );
	for( i = 0; i < m_numParam; i++ ) printf( "param%d, %.4f\n", i, m_dParams[ i ] );
	return 0;
}

//оптимизация регулярным или случайным выбором точки в пространстве
double POptimizer::doGrid( int NTIMES )
{
	int NSPACE = m_numParam;
	int i, ivar;
	double funcOld, funcBest = 1e+100;
	double* dBest = new double[ m_numParam ]; //массив для текущих наилучших значений параметров

	int mod = (int)cbrt(NTIMES);

	for( i = 0; i < NTIMES; i++ )
	{	
		//новая точка в пространстве
		m_funcGridValue( i, mod, m_dParams );
        if( i % 1000 == 999 ) printf( "%d, par0 %.4f, par1 %.4f, par2 %.4f\n", 
			i, m_dParams[ 0 ], m_dParams[ 1 ], m_dParams[ 2 ] );

		funcOld = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );

		if( funcOld < funcBest )
		{	//запомнить
			char buf[ 100 ];
			sprintf( buf, "###i=%d, better distance=%g, par0=%g, par1=%g, par2=%g\n", 
				i, funcOld, m_dParams[ 0 ], m_dParams[ 1 ], m_dParams[ 2 ] );
			if( strstr( buf, "#IND" ) > 0 )
			{	//#IND - ошибка при выводе чисел, пропустить
				buf[ 0 ] = 0;
			}
			else
			{
				printf( "%s", buf );
				funcBest = funcOld;
				for( ivar = 0; ivar < NSPACE; ivar++ ) 
				{
					dBest[ ivar ] = m_dParams[ ivar ];
				}
			}
		}
	}

	for( ivar = 0; ivar < NSPACE; ivar++ ) 
	{
		m_dParams[ ivar ] = dBest[ ivar ];
	}
	delete [] dBest;

	return funcBest;
}

//метод градиентного спуска:
//вычислить функционал
//сделать небольшие изменения, найти частные производные DV
//двигаться в направлении против DV
//повторять пока функционал уменьшается
//NRANDOM - число случайных посевов точки старта
//NTIMES - максимальное число итераций в каждом посеве
double POptimizer::doGradient( int NRANDOM, int NTIMES )
{
	int NSPACE = m_numParam;
	int i, j, k, ivar, jSuc;
	double step, func, funcOld, funcStep;
	double funcBest = 1e+100;
	double* dBest = new double[ m_numParam ]; //массив для текущих наилучших значений параметров
	double* df = new double[ m_numParam ]; //массив для частных производных

	double r2 = 0.5 * RAND_MAX;

	for( i = 0; i < NRANDOM; i++ )
	{	//чтобы преодолеть локальные минимумы делаем NRANDOM раз
		if( i > 0 )
		for( k = 0; k < NSPACE; k++ ) 
		{
			m_dParams[ k ] = ( rand() - r2 ) * DIAMETER / RAND_MAX;
            if( iDebug ) printf( "init par %.4f\n", m_dParams[ k ] );
		}

		if( m_funcOverlimits != NULL && m_funcOverlimits( m_dParams ) )
			continue;

        if( i % 10 == 0 ) printf( "%d par0 %.4f par1 %.4f\n", i, m_dParams[ 0 ], m_dParams[ 1 ] );
		funcOld = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );
		if( iDebug ) printf( "start distance=%g\n", funcOld );

		for( k = 0; k < NTIMES; k++ )
		{		
			//вычислить градиент
			double gradlen = 0.0;
			for( ivar = 0; ivar < NSPACE; ivar++ )
			{
				m_dParams[ ivar ] += DT;	//небольшая вариация параметра
				funcStep = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );
				m_dParams[ ivar ] -= (DT + DT);	//2 шага назад
				func = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );
				m_dParams[ ivar ] += DT;	//вернуться к исходному значению параметра
				df[ ivar ] = -( funcStep - func );
				gradlen += df[ ivar ] * df[ ivar ];
			}

			//нормализовать
			gradlen = sqrt( gradlen );
			if( gradlen == 0.0 ) break;
			for( ivar = 0; ivar < NSPACE; ivar++ )
			{
				df[ ivar ] = df[ ivar ] / gradlen;
			}

			step = 3.0 * DT;
			funcStep = funcOld;
			jSuc = 0;
			//двигаемся в направлении анти-градиента с переменным шагом
			for( j = 0; j < 20 && step >= DT; j++ )
			{
				for( ivar = 0; ivar < NSPACE; ivar++ )
				{
					m_dParams[ ivar ] += step * df[ ivar ];
				}	
				func = m_funcExternal ? m_funcExternal( m_dParams ) : functional( m_dParams );
				if( funcStep - func < 0.0 || 
					( m_funcOverlimits != NULL && m_funcOverlimits( m_dParams ) ) ) 
				{	//назад
					for( ivar = 0; ivar < NSPACE; ivar++ )
					{
						m_dParams[ ivar ] -= step * df[ ivar ];
					}	
					step *= 0.7; //уменьшить шаг
				}
				else 
				{	//принято
					funcStep = func;
					step *= 2.0; //увеличить шаг
					jSuc = j;
				}
			}

		    if( iDebug ) printf( "distance=%.6f\n", funcStep );
			if( funcOld - funcStep < TRESHOLD ) break;
			//keep the new values
			funcOld = funcStep;
		}

		if( iDebug ) printf( "opt distance=%g\n", funcOld );
		if( funcOld < funcBest )
		{	//сохранить параметры
			char buf[ 100 ];
			sprintf( buf, "###i=%d, k=%d, jSuc=%d, better distance=%g, param0=%g\n", i, k, jSuc, funcOld, m_dParams[ 0 ] );
			if( strstr( buf, "#IND" ) > 0 )
			{	//#IND - ошибка при выводе чисел, пропустить
				buf[ 0 ] = 0;
			}
			else
			{
				printf( "%s", buf );
				funcBest = funcOld;
				for( ivar = 0; ivar < NSPACE; ivar++ ) 
				{
					dBest[ ivar ] = m_dParams[ ivar ];
				}
			}
		}
	}

	for( ivar = 0; ivar < NSPACE; ivar++ ) 
	{
		m_dParams[ ivar ] = dBest[ ivar ];
	}
	delete [] dBest;
	delete [] df;

	return funcBest;
}


