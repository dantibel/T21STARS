/* DB6, Belov Danil, 10-6, 30.10.21 */

typedef double DBL;
typedef unsigned char BYTE;
typedef struct tagVec
{
  DBL X, Y, Z; /* Coordinates of Star */
  BYTE Color; 
} VEC;

extern DBL Speed;

void StarQuickSort( float *A, int N );
void PutPixel( int X, int Y, BYTE Red, BYTE Green, BYTE Blue );

void StarsInit( void );
void StarsDraw( void );
void StarsMove( DBL DT );
void StarsRotate( DBL Angle );
void ProjSetSize( int NewWs, int NewHs );