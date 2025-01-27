#include "BackEnd/BackEnd.h"

int main()
{
  BackEnd::InitializeProgram();
  BackEnd::MainLoop();
  BackEnd::CleanUp();
  return 0;
}