/* DB6, Belov Danil, 10-6, 30.10.21 */

#include <stdlib.h>
#include <math.h>
#include "stars.h"

#define INFINITY 100000000.0
#define NEAR_DIST 100
#define MAX 1000

static VEC Stars[MAX];
DBL ProjDist = 1.0, ProjSize = 1.0, Wp, Hp, Speed = 10000000.0, Angle;

int Ws = 800, Hs = 800;
extern DBL FrameZoom;

/* Get random number from -1 to 1 function */
static DBL R1( void )
{
  return 2.0 * rand() / RAND_MAX - 1.0; 
} /* End of 'R1' function */
 

/* Get random number from 0 to 1 function */
static DBL R0( void )
{
  return rand() / (DBL)RAND_MAX;
} /* End of 'R0' function */


/* One star initialization function */
static VEC InitStar( void )
{
  VEC S;

  S.X = R1() * INFINITY / 5;
  S.Y = R1() * INFINITY / 5;
  S.Z = R0() * INFINITY;
  S.Color = 0;
  return S;
} /* End of 'InitStar' function */

 /* All stars initialization function */
void StarsInit(void)
{
  int i;

  for (i = 0; i < MAX; i++)
    Stars[i] = InitStar();
} /* End of 'StarsInit' function */


/* Swap two variables function*/
static void Swap(VEC *A, VEC *B)
{
  VEC t = *B;
  *B = *A;
  *A = t;
} /* End of 'Swap' function*/


/* Stars QuickSort by Z coordinates function */
static void StarsQuickSort( VEC *A, int N)
{
  int b = 0, e = N - 1;
  DBL x;

  if (N < 2)
    return;

  x = A[N / 2].Z;

  while (b <= e)
  {
    while (A[b].Z < x)
      b++;
    while (A[e].Z > x)
      e--;
    if (b <= e)
    {
      if (b != e)
        Swap(&A[b], &A[e]);
      b++;
      e--;
    }
  }
  StarsQuickSort(A, e + 1);
  StarsQuickSort(A + b, N - b);
} /* End of 'QuickSort' function */


/* Get color of star function */
static BYTE GetColor( DBL Z )
{
  BYTE Color = 0;
  DBL n = (INFINITY - NEAR_DIST) / 254;

  if (Z < 100)
    Color = 255;
  else if (Z < INFINITY && Z > NEAR_DIST)
    Color = 255 - Z / (INFINITY - NEAR_DIST) * 254;
  return Color;
} /* End of 'GetColor' function */


/* Rotate relative to stars function */
void StarsRotate( DBL Angle )
{
  int i;
  DBL 
    a = Angle * 3.14149265358979323846 / 180,
    si = sin(a), co = cos(a);

  for (i = 0; i < MAX; i++)
  {
    DBL
      X = Stars[i].X * co - Stars[i].Y * si,
      Y = Stars[i].X * si + Stars[i].Y * co;

    Stars[i].X = X;
    Stars[i].Y = Y;
  }

} /* End of 'StrasRotate' function */


/* One star drawing function */
static void DrawStar( VEC S, int i )
{
  DBL Xp = S.X * ProjDist / S.Z, Yp = S.Y * ProjDist / S.Z;  
  int Xs, Ys;
  BYTE Color;
  
  Xs = (int)(Xp * Ws / Wp + Ws * 0.5);
  Ys = (int)(Hs * 0.5 - Yp * Hs / Hp);
  Stars[i].Color = Color = GetColor(S.Z);
  PutPixel(Xs, Ys, Color, Color, Color);
} /* End of 'DrawStar' function */


/* All stars drawing function */
void StarsDraw( void )
{
  int i;

  StarsQuickSort( Stars, MAX);
  for (i = 0; i < MAX; i++)
    DrawStar(Stars[i], i);
} /* End of 'StarsDraw' function */


/* All stars moving function */
void StarsMove( DBL DT )
{
  int i;

  for (i = 0; i < MAX; i++)
  {  
    Stars[i].Z -= Speed * DT;
    if (Stars[i].Z <= 0)
    {
      Stars[i].Color = 0;
      Stars[i].Z = InitStar().Z;
    }
  }
} /* End of 'StarsMove' function */


/* Set project coordinates function */
static void ProjSet(void)
{
  if (Ws > Hs)
    Wp = ProjSize * Ws / Hs, Hp = ProjSize;
  else
    Wp = ProjSize, Hp = ProjSize * Hs / Ws;
} /* End of 'ProjSet' function */


/* Set size of project function */
void ProjSetSize( int NewWs, int NewHs ) 
{
  ProjDist = FrameZoom;
  Ws = NewWs, Hs = NewHs;
  ProjSet();
} /* End of 'ProjSetSize' function */