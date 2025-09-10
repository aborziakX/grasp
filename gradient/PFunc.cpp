// PFunc.cpp - разные фукционалы и функция main
#include "stdafx.h"
#include "POptimizer.h"

double ONE_DIV_SQRT_2PI = 1.0 / ( sqrt( 2.0 * 3.1415926 ) );	//~0.4

//get integral from 0 to +infin. approximate by sum
double getKB( double medInt, double sigmaInt )
{
	int k, iTotal = 12;
	double dx = ( 8.0 * sigmaInt ) / ( iTotal );	//+- 4 sigma
	double dx2 = dx * 0.5;
	double x = medInt - 4.0 * sigmaInt + dx2, kb = 0.0;
	for( k = 0; k < iTotal; k++, x += dx )
	{
		if( x < 0 ) continue;

		kb += ONE_DIV_SQRT_2PI / sigmaInt * exp( - ( medInt - x ) * ( medInt - x ) / ( 2.0 * sigmaInt * sigmaInt ) );
	}
	kb = kb * dx;
	if( kb == 0.0 )
	{
		return 100.0;
	}
	return 1.0 / kb;
}

//случайное число от 0 до 1
double rand0_1()
{
	 double d = ( (double) rand() ) / RAND_MAX;
	 if( d > 1.0 ) d = 1.0;
	 if( d < 0.0 ) d = 0.0;
	 return d;
}

//случайное число с нормальным распределением, среднее - dAver, сигма (стандартное отклонение) - dSigma
double rand_norm( double dAver, double dSigma )
{
	double d = 0.0;
	for( int i = 0; i < 12; i++ )
	{
		d += ( 12.0 * dSigma * rand0_1() );
	}
	return ( d - 6.0 * 12.0 * dSigma ) / 12 + dAver;
}

//test 1 - парабаллоид
double functional_test1( double * dParams )
{
	return ( dParams[ 0 ] - 5.0 ) * ( dParams[ 0 ] - 5.0 ) + ( dParams[ 1 ] - 2.0 ) * ( dParams[ 1 ] - 2.0 ) ;
}

//test 2 - седло
double functional_test2( double * dParams )
{
	return dParams[0] * dParams[0] - dParams[1] * dParams[1];
}

//ограничения на парметры
bool limits_test2(double * dParams)
{
	if (dParams[0] < -1 || dParams[0] > 1 || dParams[1] < -0.5 || dParams[1] > 1.0 ) return true;
	return false;
}

//test 3 - парабаллоид
double functional_test3( double * dParams )
{
	return ( dParams[ 0 ] - 0.5 ) * ( dParams[ 0 ] - 0.5 ) + ( dParams[ 1 ] - 0.7 ) * ( dParams[ 1 ] - 0.7 ) 
		+ ( dParams[ 2 ] - 1.0 ) * ( dParams[ 2 ] - 1.0 );
}

//выбор точки в пространстве
void grid3( int num, int mod, double * dParams )
{
	int i, j, k;
	i = num % mod;
	num = num / mod;
	j = num % mod;
	k = num / mod;
	
	dParams[ 0 ] = 0.05 + i * 0.05;
	dParams[ 1 ] = 0.05 + j * 0.05;
	dParams[ 2 ] = 0.05 + k * 0.05;
}

int main(int argc, char* argv[])
{
    int rc = 1, i;
	int mode = ( argc > 1 && atoi( argv[ 1 ] ) > 0 ) ? atoi( argv[ 1 ] ) : 0;
    int NRANDOM = ( argc > 2 && atoi( argv[ 2 ] ) > 0 ) ? atoi( argv[ 2 ] ) : 200;
   	int NTIMES = ( argc > 3 && atoi( argv[ 3 ] ) > 0 ) ? atoi( argv[ 3 ] ) : 500;

	if( mode < 0 || mode > 3 )
	{
		printf( "Optimizer.exe [mode [NRANDOM [NTIMES [file]]]]\nUse mode=0 for linear separation" );
		getchar();
		return -1;
	}

	printf( "Optimizer стартовал\n" );
	srand( (unsigned)time( NULL ) );
	
    POptimizer * pOpt = new POptimizer();
	if( mode == 1 )
	{	//test 1
		pOpt->m_funcExternal = functional_test1;
		pOpt->m_numParam = 2;
	}
	if( mode == 2 )
	{	//test 2
		pOpt->m_funcExternal = functional_test2;
		pOpt->m_funcOverlimits = limits_test2;
		pOpt->m_numParam = 2;
	}
	if( mode == 3 )
	{	//test 3
		pOpt->m_funcExternal = functional_test3;
		pOpt->m_funcGridValue = grid3;
		pOpt->m_numParam = 3;
	}

    if( mode == 0 ) rc = pOpt->load( argc > 4 ? argv[ 4 ] : "separate.points" );
    if( rc > 0 ) 
	{
		if( NRANDOM > 1 ) 
			pOpt->run( 1, NRANDOM, NTIMES );	//градиентный спуск
		else pOpt->run( 2, NRANDOM, NTIMES );	//поиск в пространстве
	}

	delete pOpt;

	printf( "Нажать <Enter> завершить.\n" );
	getchar();
	return 0;
}

