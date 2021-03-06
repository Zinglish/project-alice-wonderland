/**
 * Author:  Terence-Lee 'Zinglish' Davis
 * License: MIT
 * Email:   zinglish[at]gmail.com
 * Twitter: @zingatsu
 */

/*
 * @author: Terence-Lee 'Zinglish' Davis
 * 
 * Designed to handle UNIX sockets and its clients.
 */

#ifndef SERVER_H
#define	SERVER_H

#include <string>
#include <vector>
#include <mutex>

#include "ipc_event.h"

class RabbitHole;
class IPCCoD4Event;
class Limbo;

class IPCServer {
public:
	/**
	 * Constructs a listener UNIX socket at `path` and starts a new thread.
	 * 
     * @param wid .
     */
	IPCServer(char* wid);
	
	IPCServer(const IPCServer& orig);
	virtual ~IPCServer();
	
/*===============================================================*\
 * THREADS
\*===============================================================*/
	
	/**
	 * Separate thread to monitor incoming standard transmissions.
	 * 
     * @param serverPtr Usually described as `this`.
     * @return 
     */
	static void* ThreadedCommAllocator(void* serverPtr);
	
	/**
	 * Thread deals with direct communications with the CoD4 service.
	 * 
     * @param serverPtr
     * @return 
     */
	static void* ThreadedComm(void* serverPtr);
	
/*===============================================================*\
 * FUNCTIONS
\*===============================================================*/
	
	/**
	 * Recvs data up to chunkSize amount.
	 * 
     * @return Payload
     */
	char* RecvChunk(int socket, u_int32_t chunkSize);

	/**
	 * Flushes the socket to get rid of excess data in the packet to
	 * stop recv() hanging.
	 */
	void FlushSocket(int socket);
	
	/**
	 * Parses receiving transmissions that occur in the manager.
	 * 
     * @param pkt Packet from the manager.
     * @return Packet to send back to the client.
     */
	void ResponseHandler(int socket, char* pkt);
	
	/**
	 * Assigns/Creates a new communication path.
	 * 
     * @return Newly created communication path/name.
     */
	unsigned int CreateNewComm();
	
	/**
	 * Adds an event broadcast on to the queue then signals all outbound
	 * comms to send the event to Alice.
	 * 
     * @param event
     */
	static void SetEventForBroadcast(IPCCoD4Event* event);

	/**
	 * Removes the specified event from the broadcasting stack.
	 * WARN: This is NOT thread safe, to make thread safe wrap
	 * in a lock (IPCServer::bcastEventStackLock).
	 * 
     * @param event
     */
	static void DestroyEvent(IPCCoD4Event* event);
	
	/**
	 * Sets a Limbo for denial. Non-destructive, reason is copied.
	 * 
     * @param ip
     * @param reason
     */
	static void LimboDeny(char* ip, uint32_t qPort, char* reason);
	
	/**
	 * Sets a Limbo for acceptance.
     * @param ip
     */
	static void LimboAccept(char* ip, uint32_t qPort);

/*===============================================================*\
 * VARIABLES
\*===============================================================*/
	
	// Tracks client comms
	static std::vector<RabbitHole*> rabbitHoles;

	// Holds event type packets which should be broadcasted across
	// all connected clients.
	static std::vector<IPCCoD4Event*> broadcastEvents;
	
	// Holds limbo structs
	static std::vector<Limbo*> limbo;
	
	static std::mutex bcastEventStackLock; // TODO: Refactor to name: bcastEventStackLock
	
/*===============================================================*\
 * GTORS & STORS
\*===============================================================*/
	
	static void Initialized();
	
	static int IsServerInitialized();
	
private:
	pthread_t listener;
	
	std::string rabbitHolePrefix;
	std::string rabbitHolePath;
	
	static int serverInitialized; // Changed to 1 once the server's initialized correctly

	char* wid; // Wonderland ID
};

#endif	/* SERVER_H */

