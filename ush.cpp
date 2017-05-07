#include "include/ush.h"
#include "include/parser.h"

using namespace std;

void ush::loadMBF() {
	//MBC.insert(pair<string, builtin_cmd*>("test",new test()));
	MBC.insert(pair<string, builtin_cmd*>("ucd", new ucd()));
	MBC.insert(pair<string, builtin_cmd*>("ucp", new ucp()));
}

ush::ush()
{
	loadMBF();
	while (gets(input_buf)) {
		int num = 0;
		char **s = Parser::run(input_buf, num);
		for (int i = 0; i < num; i++) {
			printf("%s\n",s[i]);
		}
		MBC[s[0]]->run(num, s);
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
