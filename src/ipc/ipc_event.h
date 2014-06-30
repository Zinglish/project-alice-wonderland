#ifndef IPC_EVENT_PKT_H
#define	IPC_EVENT_PKT_H

#include <string>
#include <typeinfo>

class IPCCoD4Event {
	static const char delim = 0x01;
public:
	IPCCoD4Event(char* a1);
	IPCCoD4Event(char* a1, char* a2);
	IPCCoD4Event(const IPCCoD4Event& orig);
	virtual ~IPCCoD4Event();
	
/*===============================================================*\
 * COMPILE TEMPLATES
\*===============================================================*/
	
	void Compile(unsigned long eventFuncLoc, char* a1, char* a2);
private:
	std::string packetString;
	
	char* a1;
	char* a2;
	char* a3;
	char* a4;
};

#endif	/* IPC_EVENT_PKT_H */

