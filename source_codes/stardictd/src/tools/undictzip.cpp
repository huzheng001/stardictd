// This tool make all the .dict.dz files become .dict. When there are too many dictionaries, as stardictd will mmap all the .dict.dz file into virtual memory, if the memory size of stardictd process become bigger than 4G, then will be problem. So use .dict file can solve this problem.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

void undictzipfile(const char *filename)
{
	char gzfilename[256];
	strcpy(gzfilename, filename);
	int len = strlen(filename);
	gzfilename[len-2] = 'g';
	char command[512];
	sprintf(command, "mv %s %s", filename, gzfilename);
	int result;
	result = system(command);
	if (result == -1) {
		g_print("system() error!\n");
	}
	sprintf(command, "gunzip %s", gzfilename);
	if (system(command)!=0) {
		sprintf(command, "mv %s %s", gzfilename, filename);
		result = system(command);
		if (result == -1) {
			g_print("system() error!\n");
		}
	}
}

void undictzipdir(const char *dirname)
{
	GDir *dir = g_dir_open(dirname, 0, NULL);
	if (!dir) {
		g_print("Open %s failed!", dirname);
		return;
	}
	const gchar *filename;
	gchar fullfilename[256];
	while ((filename = g_dir_read_name(dir))!=NULL) {
		sprintf(fullfilename, "%s/%s", dirname, filename);
		if (g_file_test(fullfilename, G_FILE_TEST_IS_DIR)) {
			undictzipdir(fullfilename);
		} else if (g_str_has_suffix(filename, ".dict.dz")) {
			undictzipfile(fullfilename);
		}
	}
	g_dir_close(dir);
}

int main(int argc,char * argv [])
{
	undictzipdir("/usr/share/stardict/dic");
	return FALSE;
}
