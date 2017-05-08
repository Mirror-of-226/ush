#include "include/ush.h"
#include "include/parser.h"

using namespace std;

void ush::loadMBF() {
	//MBC.insert(pair<string, builtin_cmd*>("test",new test()));
	MBC.insert(pair<string, builtin_cmd*>("ucd", new ucd()));
	MBC.insert(pair<string, builtin_cmd*>("ucp", new ucp()));
    MBC.insert(pair<string, builtin_cmd*>("upwd", new upwd()));
    MBC.insert(pair<string, builtin_cmd*>("uecho", new uecho()));
    MBC.insert(pair<string, builtin_cmd*>("umkdir", new umkdir()));
    MBC.insert(pair<string, builtin_cmd*>("uls", new uls()));
    MBC.insert(pair<string, builtin_cmd*>("ukill", new ukill()));
    MBC.insert(pair<string, builtin_cmd*>("ups", new ups()));
    MBC.insert(pair<string, builtin_cmd*>("urm", new urm()));
    MBC.insert(pair<string, builtin_cmd*>("uhelp", new uhelp(getMBC())));
}

std::map<std::string, builtin_cmd*>& ush::getMBC() {
    return MBC;
}

ush::ush()
{
	loadMBF();
	while (true) {

        printf("> ");
        gets(input_buf);
		int num = 0;
		char **s = Parser::run(input_buf, num);

        if (strcmp(s[0], "uexit") == 0) {
            break;
        }
        std::map<std::string, builtin_cmd*>::iterator iter;
        iter = MBC.find(s[0]);

        if (iter != MBC.end()) {
            MBC[s[0]]->run(num, s);
        }
        else {
            printf("ush: command not found: %s\n", s[0]);
        }
	}
}

ush::~ush()
{
}

int main()
{
	ush* USH = new ush();
	return 0;
}
