#include "ush.h"
#include <stdio.h>
#include <map>

using namespace std;

ush::ush()
{	
	while (gets_s(input_buf)) {
		printf("%s\n",input_buf);
	}
}


ush::~ush()
{
}

int main() {
	ush* u1 = new ush();
}
