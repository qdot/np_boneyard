/****************************************************************************
** $Id: qt/life.h   3.3.5   edited Aug 31 12:17 $
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/


#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#define TRUE 1
#define FALSE 0

using namespace std;

class LifeWidget
{
public:
	LifeWidget( int s = 10, const char *name = 0 );

	void        setPoint( int i, int j );

	int         maxCol() { return maxi; }
	int         maxRow() { return maxj; }
	void        nextGeneration();
	void        clear();
	void        randomize();

private:
	enum { MAXSIZE = 50, MINSIZE = 10, BORDER = 5 };

	bool        cells[2][MAXSIZE + 2][MAXSIZE + 2];
	int         current;
	int         maxi, maxj;

	int pos2index( int x )
	{
		return ( x - BORDER ) / SCALE + 1;
	}
	int index2pos( int i )
	{
		return  ( i - 1 ) * SCALE + BORDER;
	}

	ofstream outFile;
	int SCALE;
};

LifeWidget::LifeWidget( int s, const char *name ) : outFile("testOut.txt")
{
	SCALE = s;
	maxi = maxj = 50;
	srand(time(NULL));
	clear();
	randomize();
	
}

void LifeWidget::clear()
{
	current = 0;
	for ( int t = 0; t < 2; t++ )
		for ( int i = 0; i < MAXSIZE + 2; i++ )
			for ( int j = 0; j < MAXSIZE + 2; j++ )
				cells[t][i][j] = FALSE;
}

void LifeWidget::randomize()
{
	current = 0;
	for ( int t = 0; t < 2; t++ )
		for ( int i = 0; i < MAXSIZE + 2; i++ )
			for ( int j = 0; j < MAXSIZE + 2; j++ )
				rand() % 2 ? cells[t][i][j] = FALSE : cells[t][i][j] = TRUE;
}


void LifeWidget::setPoint( int i, int j )
{
	if ( i < 1 || i > maxi || j < 1 || j > maxi )
		return;
	cells[current][i][j] = TRUE;
}

void LifeWidget::nextGeneration()
{
	for ( int i = 1; i <= MAXSIZE; i++ ) {
		for ( int j = 1; j <= MAXSIZE; j++ ) {
			int t = cells[current][i - 1][j - 1]
			+ cells[current][i - 1][j]
			+ cells[current][i - 1][j + 1]
			+ cells[current][i][j - 1]
			+ cells[current][i][j + 1]
			+ cells[current][i + 1][j - 1]
			+ cells[current][i + 1][j]
			+ cells[current][i + 1][j + 1];

			cells[!current][i][j] = ( t == 3 ||
				t == 2 && cells[current][i][j] );
		}
	}
	string strHexSetup;
	char hexNum[9];
	for(int i = 0; i < 32; ++i)
	{
		int lineMap = 0;
		for(int j = 0; j < 32; ++j)
		{
			lineMap |= (cells[current][i][j] & 1) << 31-j;
		}
		sprintf(hexNum, "%.8x", lineMap);
		outFile << hexNum;
	}
	outFile << "\n";
	current = !current;
}

void usage()
{
	cout << "Usage: life [-scale scale]" << endl;
}

int main( int argc, char **argv )
{
	int scale = 10;

	for ( int i = 1; i < argc; i++ ){
		char* arg = argv[i];
		if ( arg == "-scale" )
			scale = atoi( argv[++i] );
		else {
			usage();
			exit(1);
		}
	}

	if ( scale < 2 )
		scale = 2;

	LifeWidget *life = new LifeWidget( scale );
	for(int i = 0; i < 64; ++i)
	{
		life->nextGeneration();
	}
	delete life;
	return 0;
}
