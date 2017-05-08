#include "include/ush.h"
#include "include/parser.h"

using namespace std;

#ifdef _WIN32
#define gets gets_s
#endif

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
void ush::setNowPath() {
    getcwd(now_path, sizeof(now_path));

}
#endif

void ush::loadMBF() {
	typedef pair<string, builtin_cmd*> sbp;
	MBC.insert(sbp("utime",new utime()));
	MBC.insert(sbp("ucp", new ucp()));
	MBC.insert(sbp("umkdir", new umkdir()));
	MBC.insert(sbp("ups", new ups()));
	MBC.insert(sbp("ukill", new ukill()));
	MBC.insert(sbp("utype", new utype(getMBC())));
	MBC.insert(sbp("uls", new uls(now_path)));
	MBC.insert(sbp("ucd", new ucd(now_path)));
	MBC.insert(sbp("upwd", new upwd(now_path)));
	MBC.insert(sbp("urm", new urm(now_path)));
	MBC.insert(sbp("uhelp", new uhelp(getMBC())));
	MBC.insert(sbp("uecho", new uecho()));
}

ush::ush()
{
#ifdef _WIN32
	strcpy(now_path, "C:\\");
#endif // _WIN32
	loadMBF();
#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
    setNowPath();
#endif
	printf("Welcome to use universal shell writen by Chen Jiangui and Chi Zewen.\n");
    printf("Copyright 2017 Chen Jiangui and Chi Zewen. All Rights Reserved.\n\n");
#ifdef _WIN32
    printf("\n%s> ", now_path);
#elif defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
    printf("%s$ ", now_path);
#endif
	while (gets(input_buf)) {
		int num = 0;
		char **s = Parser::run(input_buf, num);
		if (num == 0) {
#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
            setNowPath();
#endif
#ifdef _WIN32
            printf("\n%s> ", now_path);
#elif defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
            printf("%s$ ", now_path);
#endif
			continue;
		}
		if (strcmp(s[0], "uexit") == 0) {
			break;
		}

		if(MBC.find(s[0]) != MBC.end()) MBC[s[0]]->run(num,s);
		else printf("ush: command not found.\n");

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
        setNowPath();
#endif
#ifdef _WIN32
        printf("\n%s> ", now_path);
#elif defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
        printf("%s$ ", now_path);
#endif
	}
}

std::map<std::string, builtin_cmd*>& ush::getMBC() {
	return MBC;
}

ush::~ush()
{
}

int main() {
	ush* USH = new ush();
	system("pause");
	return 0;
}
