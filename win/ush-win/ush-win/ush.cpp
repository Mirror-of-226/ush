#include "include/ush.h"
#include "include/parser.h"

using namespace std;

void ush::loadMBF() {
	typedef pair<string, builtin_cmd*> sbp;
	MBC.insert(sbp("test",new test()));
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
}

ush::ush()
{
	strcpy(now_path, "C:\\");
	loadMBF();
	printf("Welcome to use universal shell writen by Chen Jiangui and Chi Zewen.\nCopyright 2017 Chen Jiangui and Chi Zewen. All Rights Reserved.\n\n%s>", now_path);
	while (gets_s(input_buf)) {
		int num = 0;
		char **s = Parser::run(input_buf, num);
		if (num == 0) {
			printf("\n%s>", now_path);
			continue;
		}
		if(MBC.find(s[0]) != MBC.end()) MBC[s[0]]->run(num,s);
		else printf("order not found.\n");
		printf("\n%s>", now_path);
	}
}

std::map<std::string, builtin_cmd*>& ush::getMBC() {
	return MBC;
}

ush::~ush()
{
}

int main() {
	ush* u1 = new ush();
	system("pause");
}
