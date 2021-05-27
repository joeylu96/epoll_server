#include "header.h"
#include "global_work.h"

int main()
{
	int ret = CGlobalWork::Init();
	if(ret!=0)
	{
		cout<<"CGlobalWork::Init |error |ret="<<ret<<endl;
		exit(0);
	}

	ret=CGlobalWork::Start();
	if(ret!=0)
	{
		cout<<"CGlobalWork::Work |error |ret="<<ret<<endl;
		exit(0);
	}

	while(1)
	{
		//每一秒check一次，进行update svr操作，且判断是否停止
		sleep(1);
	}
	
	return 0;	
}
