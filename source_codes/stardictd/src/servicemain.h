#ifndef _SERVICE_MAIN_H
#define _SERVICE_MAIN_H

class ServiceMain {
public:
	ServiceMain();
	virtual ~ServiceMain();

	virtual bool init(int argc, char *argv[]) = 0;
	virtual void run() = 0;
};

#endif
