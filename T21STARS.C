/* DB6, Belov Danil, 10-6, 30.10.21 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "glut.h"

#include "stars.h"

/* Time variables */
DBL DeltaTime, SyncTime, FPS;
int IsPause, IsTurbo;

/* Measure time function */
void Timer( void )
{
  long t;
  static long StartTime = -1, OldTime, PauseTime, FrameCount, FPSTime;

  t = clock();
  if (StartTime == -1)
    StartTime = FPSTime = OldTime = t;

  if (IsPause)
  {
    DeltaTime = 0;
    PauseTime += t - OldTime;
  }
  else
  {
    DeltaTime = (t - OldTime) / (double)CLOCKS_PER_SEC;
    OldTime = t;
    SyncTime = (t - PauseTime - StartTime) / (double)CLOCKS_PER_SEC;
  }
  
  FrameCount++;

  if (t - FPSTime > 1 * CLOCKS_PER_SEC)
  {
    FPS = FrameCount / ((t - FPSTime) / (double)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
  }
} /* End of 'Timer' function */

/* Frame variables */
HWND hWnd;
static int WinW, WinH;
static char Buf[100];
BYTE (*Frame)[3];
int FrameW = 800, FrameH = 800;
DBL FrameZoom = 1.0;

/* Set size of frame function */
static void FrameSetSize( int W, int H )
{
  if (Frame != NULL)
    free(Frame);
  FrameW = W, FrameH = H;
  Frame = malloc(FrameW * FrameH * 3);
  if (Frame == NULL)
    exit(0);
  ProjSetSize(FrameW, FrameH);
} /* End of 'FrameSetSize' function */


/* Draw pixel function */
void PutPixel( int X, int Y, BYTE Red, BYTE Green, BYTE Blue )
{
  if (X < 0 || Y < 0 || X >= FrameW || Y >= FrameH)
    return;

  Frame[Y * FrameW + X][0] = Blue;
  Frame[Y * FrameW + X][1] = Green;
  Frame[Y * FrameW + X][2] = Red;
} /* End of 'PutPixel' function */


/* GLUT Reshape function */
void Reshape( int NewW, int NewH )
{
  WinW = NewW;
  WinH = NewH;
  FrameSetSize(WinW, WinH);
  ProjSetSize(WinW / FrameZoom, WinH / FrameZoom);
  glViewport(0, 0, WinW, WinH);
} /* End of 'Reshape' function */


/* GLUT Display function */
void Display( void )
{
  int rot = 0;

  if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
    rot++;
  if (GetAsyncKeyState(VK_RSHIFT) & 0x8000)
    rot--;

  glClear(GL_COLOR_BUFFER_BIT);
 
  if (!IsPause)
  {
    memset(Frame, 0, FrameW * FrameH * 3);
    StarsMove(DeltaTime);
    StarsRotate(rot);
    StarsDraw();
    Timer();
  }
  
  sprintf(Buf, "FPS: %.3lf | Speed: %.0lf | Zoom: %.2lf", FPS, Speed, FrameZoom);
  SetWindowText(hWnd, Buf);

  glPixelZoom(FrameZoom, -FrameZoom);
  glRasterPos2d(-1, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glDrawPixels(FrameW, FrameH, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
} /* End of 'Display' function */


/* GLUT Keyboard function */
void Keyboard( unsigned char Key, int X, int Y )
{
  int Key2;

  switch (Key)
  {
  case 27:
    exit(30);
    break;

  case 32:
    IsPause = !IsPause;
    break;

  case '1':
    if (Speed < 50000000)
      Speed += 1000000;
    break;

  case '2':
    if (Speed > 1000)
      Speed -= 1000000;
    break;

  case '+':
    if (FrameZoom < 4)
    {
      FrameZoom += 0.25;
      FrameSetSize(WinW / FrameZoom, WinH / FrameZoom);
    }
    break;

  case '-':
    if (FrameZoom > 1)
    {
      FrameZoom -= 0.25;
      FrameSetSize(WinW / FrameZoom, WinH / FrameZoom);
    }
    break;

  case 't':
    IsTurbo = !IsTurbo;
    break;
  }
} /* End of 'Keyboard' function */


/* Program main function */
void main( int argc, char*argv[] )
{ 
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(FrameW, FrameH);
  glutCreateWindow("T21STARS");
  hWnd = FindWindow(NULL, "T21STARS");
 
  FrameSetSize(FrameW, FrameH);
  StarsInit();

  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  glClearColor(0.30, 0.47, 0.8, 1);

  glutMainLoop();
} /* End of 'main' function */
/* END OF "T21STARS" FILE */