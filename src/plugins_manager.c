#include "plugins_manager.h"

void get_output(char* nom_plugin, const char* options, char output[1024]){
  (void) options;
  FILE *fp;
  char* default_path = "plugins/";

  int total_size = strlen(default_path)+ strlen(nom_plugin) + 1;
  char* exec = (char*)malloc(total_size * sizeof(char));
  ASSERT(exec, "Cannot allocate memory\n")

  strcpy(exec, default_path);
  strcat(exec, nom_plugin);
  fp = popen(exec, "r");
  fgets(output,1024, fp);
  pclose(fp);
}
