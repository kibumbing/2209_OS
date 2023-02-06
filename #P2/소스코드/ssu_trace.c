#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int mask = atoi(argv[1]);
  trace(mask);

  char *argv2[argc-2];

  for(int i=0; i<argc-1; i++){
	argv2[i] = argv[i+2];
  }

  exec(argv[2], argv2);

  exit(); 
}
