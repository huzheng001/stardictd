#include <glib.h>
#include <string.h>
#include <stdio.h>

#include <string>
#include <list>

struct DictInfo {
	std::string path;
	std::string uid;
};

void GenXMLFiles(const char *dirname)
{
	std::list<std::string> subdirs;
	std::list<DictInfo> dicts;
	GDir *dir = g_dir_open(dirname, 0, NULL);
	if (dir) {
		const gchar *filename;
		while ((filename = g_dir_read_name(dir))!=NULL) {
			std::string fullfilename;
			fullfilename = dirname;
			fullfilename += G_DIR_SEPARATOR;
			fullfilename += filename;
			if (g_file_test(fullfilename.c_str(), G_FILE_TEST_IS_DIR)) {
				GDir *dir2 = g_dir_open(fullfilename.c_str(), 0, NULL);
				if (dir2) {
					const gchar *filename2;
					bool hasifofile = false;
					while ((filename2 = g_dir_read_name(dir2))!=NULL) {
						if (g_str_has_suffix(filename2, ".ifo")) {
							hasifofile = true;
							DictInfo info;
							info.path = filename;
							info.path += G_DIR_SEPARATOR;
							info.path += filename2;
							info.uid.assign(filename2, strlen(filename2)-4);
							dicts.push_back(info);
						}
					}
					g_dir_close(dir2);
					if (!hasifofile) {
						subdirs.push_back(filename);
						GenXMLFiles(fullfilename.c_str());
					}
				}
			}
		}
		g_dir_close(dir);
	}			
	std::string path;
	path = dirname;
	path += G_DIR_SEPARATOR_S "stardictd.xml";
	if (!g_file_test(path.c_str(), G_FILE_TEST_EXISTS)) {
		std::string content;
		content = "<dirname>";
		gchar *basename = g_path_get_basename(dirname);
		content += basename;
		g_free(basename);
		content += "</dirname>\n";
		for (std::list<std::string>::iterator i=subdirs.begin(); i!=subdirs.end(); ++i) {
			content += "<subdir>";
			content += *i;
			content += "</subdir>\n";
		}
		for (std::list<DictInfo>::iterator i=dicts.begin(); i!=dicts.end(); ++i) {
			content += "<dict>\n<path>";
			content += (*i).path;
			content += "</path>\n<uid>";
			content += (*i).uid;
			content += "</uid>\n</dict>\n";
		}
		FILE *file;
		file = fopen(path.c_str(), "w");
		if (file) {
			printf("Generate: %s\n", path.c_str());
			fprintf(file, "%s", content.c_str());
			fclose(file);
		} else {
			printf("Open %s for write failed!\n", path.c_str());
		}
	}
}

int main(int argc,char * argv [])
{
	GenXMLFiles("/usr/share/stardict/dic");
	return FALSE;
}
