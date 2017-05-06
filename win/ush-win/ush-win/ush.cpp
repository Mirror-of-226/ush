#include "ush.h"
#include <stdio.h>
#include <map>
#include "include\builtin_cmd.h"

using namespace std;

void ush::loadMBF() {

}

ush::ush()
{
	loadMBF();
	while (gets_s(input_buf)) {
		
	}
}


ush::~ush()
{
}

int main() {
	//ush* u1 = new ush();
	test* t1 = new test();
	char *a[] = { "China","French","America","German" };
	t1->run(1, a);
	system("pause");
}
