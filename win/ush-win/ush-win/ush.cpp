#include "include\ush.h"
#include "include\parser.h"

using namespace std;

void ush::loadMBF() {

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
	}
}


ush::~ush()
{
}

int main() {
	ush* u1 = new ush();
	/*test* t1 = new test();
	char *a[] = { "China","French","America","German" };
	t1->run(1, a);
	system("pause");*/
}
