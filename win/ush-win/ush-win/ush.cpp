#include "include/ush.h"
#include "include/parser.h"

using namespace std;

void ush::loadMBF() {
	typedef pair<string, builtin_cmd*> sbp;
	MBC.insert(sbp("test",new test()));
	MBC.insert(sbp("utime",new utime()));
	MBC.insert(sbp("ucp", new ucp()));
	MBC.insert(sbp("umkdir", new umkdir()));
}

ush::ush()
{
	loadMBF();
	while (gets_s(input_buf)) {
		int num = 0;
		char **s = Parser::run(input_buf, num);
		for (int i = 0; i < num; i++) {
			printf("%s\n",s[i]);
		}
		MBC[s[0]]->run(num,s);
	}
}


ush::~ush()
{
}

int main() {
	ush* u1 = new ush();
	system("pause");
}
