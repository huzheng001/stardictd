#include "servicemain.h"
#include "stdio.h"

ServiceMain *serviceMain;

ServiceMain::ServiceMain()
{
	serviceMain = this;
}

ServiceMain::~ServiceMain()
{
	serviceMain = NULL;
}
