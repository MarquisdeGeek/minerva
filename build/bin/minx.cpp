#include <stdio.h>
#include <stdlib.h>
#include <string>

char *szUSBDevice = "/dev/ttyUSB0";
int main() {
char v, last;
int state;
std::string cmd;

FILE *fp = fopen(szUSBDevice, "r");
if (!fp) {
  printf("Failed to open USB...\n");
  return 1;
}

last = 0;
state = 0;
while(1) {
 if (fread(&v, 1, 1, fp)) {
   if (last == '<' && v == '|') {
      state = 1;
      cmd = "";
   } else if (state) {
      if (last == '|' && v == '>') {
         cmd.erase(cmd.length()-1);
         // Commands are in the format, xxyyyppp
         // xx=device code, yyy=instruction  
         // this invokes a file in $MIN_ROOT/conf/minx/xx/yyy with arguments
         cmd = "/usr/local/minerva/bin/minxrun " + cmd;
         system(cmd.c_str());
         state = 0;
      } else {
         cmd += v;
      }
   } else if (v == '<') {
      // ignore for now, in case it's a command string
   } else if (last == '<') {
      // Catch-up with the '<' we omitted last time. (&& v!='|' is implied)
      printf("<%c", v);
      fflush(stdout);
   } else {
      printf("%c", v);
      fflush(stdout);
   }
   last = v;
 }
}

return 0;
}

