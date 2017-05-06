#include "include/ush.h"
#include "include/parser.h"

using namespace std;

void ush::loadMBF() {
	typedef pair<string, builtin_cmd*> sbp;
	MBC.insert(sbp("test",new test()));
	MBC.insert(sbp("utime", new utime()));
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
	/*builtin_cmd* bt = new test();
	int a = 1;
	char *a1[] = { "China","French","America","German" };
	bt->run(a, a1);*/
	ush* u1 = new ush();
	/*test* t1 = new test();
	char *a[] = { "China","French","America","German" };
	t1->run(1, a);
	system("pause");*/
	system("pause");
}
