// chat server using Beej's guide code as an example
// all functions are static; server.c runs on its own

// to compile, must define _POSIX_C_SOURCE=200112L
// use: gcc $(CFLAGS) -D_POSIX_C_SOURCE=200112L -o server server.c
// this allows compilation on ssh because otherwise getaddrinfo is no recognized

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define TRUE 1
#define ERROR -1

#define LISTENQ 8
#define MAXLINE 256
#define SEPARATE ','

// select will time out after 2 minutes
#define TIMEOUT_SEC 120
#define TIMEOUT_USEC 0

/**
 * Called by main to run the whole server
 * Sets up a listener, and uses select to accept connections or data from sockets
 *
 * @return EXIT_SUCCESS if select times out
 */
static int server_start();

/**
 * Gets the sockaddr in IPv4 or IPv6
 *
 * @param struct sockaddr *sa the sockaddr of which you want to get the address
 */
static void *get_in_addr(struct sockaddr *sa);

/**
 * Sets up the listener using more general specifications - more reliable
 * Binds the socket and returns the file descriptor
 * 
 * @param char* server port number
 * @return socket file descriptor on success, or -1 on an error
 */
static int server_listen_tcp_local(char* server_port);

/**
 * Accepts a new connection from a socket
 * 
 * @param int listener the file descriptor of the listener itself
 * @param int connfd the file descriptor that just received the connection
 * @return int file descriptor of the new socket
 */
static int server_accept(int listener, int connfd);

/**
 * Receives data from a socket, and sends it to all of the other sockets except
 * the listener
 * Removes the file descriptor from the master set if there is a disconnection
 *
 * @param fd_set master that holds all of the current file descriptors
 * @param int listener the file descriptor of the listener itself
 * @param int connfd the file descriptor that just received the connection
 * @param int* fdmax pointer to the highest number file descriptor in master
 * @return updated fd_set with the file descriptor of the new socket
 */
static fd_set server_recv_send(fd_set master, int listener, int connfd, int fdmax);

/**
 * Hardcoded level coordinates containing static lines and a couple boxes
 * Sends the coordinates as encoded strings to clients as their connection is accepted
 * if SEND is true
 *
 * @param int sockfd file descriptor that will be sent the coordinates
 * @return int 1 on success and -1 on error
 */
static int level_send_all(int sockfd);

/**
 * Encodes an integer array into a string with a flag as the first character to indicate
 * the type of coordinates being sent
 * The type should be a negative integer to be read as coordinates
 * 	-128 for static bodies, -121 for collision bodies
 * A positive integer will be read as a chat message
 *
 * @param char* buf empty buffer to be filled with the encoded string
 * @param const int* level integer array
 * @param int type the first character that will be encoded in the string
 * @return int number of bytes
 */
static int server_encode(char *buf, const int *level, int type);


int main(int argc, char **argv) {
    return server_start();
}


static int server_start() {
    // master file descriptor list, temp fd list for select()
    fd_set master, read_fds;
    
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    int fdmax; // maximum file descriptor number
    
    // set timeout for select
    struct timeval wait;
    wait.tv_sec = TIMEOUT_SEC;
    wait.tv_usec = TIMEOUT_USEC;
    int sel;
    
    // set up the listener
    int listener = server_listen_tcp_local("5993"); // default port number for all of our files
    // could have made port number input and read in argv[1] but this is more reliable
    if (listener < 0)
        return EXIT_FAILURE;

    // add listener to list of file descriptors because accept is blocking
    FD_SET(listener, &master);
    fdmax = listener; //listener is the largest file descriptor right now
    
    int SEND = TRUE; // TRUE will send the default level to the clients as they connect
    
    printf("waiting to accept a socket connection request...\n");
    
    while (TRUE) {
	// copy master list
        read_fds = master;
	
	// select for different clients connecting or sending data
	sel = select(fdmax+1, &read_fds, NULL, NULL, &wait);

        if (sel == -1) {
            fprintf(stderr, "select failed\n");
            return EXIT_FAILURE;
        }
	else if (sel == 0) {
	    printf("select timed out\n");
            break; // close the program
	}
	
        // loop through sockets
        for(int connfd = 0; connfd <= fdmax; connfd++) {
	    
            if ( FD_ISSET(connfd, &read_fds) ) {
		// socket is the listener, accept the new connection
                if (connfd == listener) {
		    int newfd;
		    
		    if ( (newfd = server_accept(listener, connfd)) != -1) {
			FD_SET(newfd, &master); // add newfd to the master set
			
			if (newfd > fdmax)  // update the greatest fd
			    fdmax = newfd;
		    }
		    else {
			fprintf(stderr, "accept failed\n");
		    }
		    
		    if (SEND) { // send default level to newfd
			if (level_send_all(newfd) == -1)
			    fprintf(stderr, "sending level failed\n");
		    }
		    
                }
                
		// socket is a client
                else {
		    master = server_recv_send(master, listener, connfd, fdmax);
                }
		
            }
        }
	
    }
    return EXIT_SUCCESS;
}

static void *get_in_addr(struct sockaddr *sa) {
    
    // checks for INET4 or INET6
    if (sa->sa_family == AF_INET)
	return &(( (struct sockaddr_in*) sa)->sin_addr);
    
    return &(( (struct sockaddr_in6*)sa )->sin6_addr);
}

static int server_listen_tcp_local(char *server_port) {
    struct addrinfo hints, *ai, *p;
    
    // get a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if (getaddrinfo(NULL, server_port, &hints, &ai) != 0) {
        fprintf(stderr, "selectserver getaddrinfo\n");
        return ERROR;
    }
    
    int listenfd, yes = 1;
    for(p = ai; p != NULL; p = p->ai_next) {
	// check all types to attempt to bind the socket
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) { 
            continue;
        }
        
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        
	// if bind failed, keep trying
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) < 0) {
	    close(listenfd);
	    continue;
	}
	
	break;
    }

    if (p == NULL) { // socket didn't bind
            fprintf(stderr, "selectserver: failed to bind\n");
            return ERROR;
    }
    
    freeaddrinfo(ai);
    
    // set up the listener
    if (listen(listenfd, LISTENQ) == -1) {
        fprintf(stderr, "listen failed\n");
        return ERROR;
    }
    
    return listenfd;
}

static int server_accept(int listener, int connfd) {
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen = sizeof remoteaddr;
    
    char remoteIP[INET6_ADDRSTRLEN];
    
    // accept the connection from the client
    int newfd = accept(listener, (struct sockaddr*) &remoteaddr, &addrlen);
    
    if (newfd != -1) {
	// if it didn't fail, print the connection and return the new fd
	printf("new connection from %s on socket %d\n",
	    inet_ntop(remoteaddr.ss_family,
	    get_in_addr((struct sockaddr*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN),
	    newfd);
    }
    
    return newfd;
}

static fd_set server_recv_send(fd_set master, int listener, int connfd, int fdmax) {
    char buf[MAXLINE]; // buffer for client data
    int nbytes;
    
    // receive the data
    if ((nbytes = recv(connfd, buf, sizeof(buf), 0)) <= 0) {
	
	if (nbytes == 0) // client closed the connection
	    printf("socket %d hung up\n", connfd);
	else
	    fprintf(stderr, "recv failed\n");
	
	close(connfd); // close socket
	FD_CLR(connfd, &master); // remove from master set
    }
    
    else { // actual data came from the client
      fflush(stdout);
      
	for(int otherfd = 0; otherfd <= fdmax; otherfd++) {
	    if (FD_ISSET(otherfd, &master)) {
		
		// send to all connections except listener
		if (otherfd != listener) {
		    if ( send( otherfd, buf, nbytes, 0 ) == -1 )
			fprintf(stderr, "send failed\n");
		    
		    //printf(getpeername(), inet_ntop(), getnameinfo(), or gethostbyaddr())
		    // for information about sender
		}
	    }
	}
    }
    
    return master;
}

static int level_send_all(int sockfd) {
    // hardcoded default level - draws a bunch of static lines in mouse coordinates with (0, 0) in bottom right
    // statics[0] = length of the array, the rest are grouped as x1,y1,x2,y2
    int statics[] = {45, 0,0,0,500, 750,0,750,500, 100,10,200,10, 250,210,200,10, 100,10,50,210, 250,210,325,190, 750,210,500,10, 325,190,400,10, 350,250,675,325, 350,450,100,333, 350,450,475,392};
    
    // prepare buffer with room for ints with 3 digits plus 1 SEPARATE char
    char lv[ (statics[0] * 4) + 1 ];
    lv[0] = '\0'; // initialize as ""
    
    // encode the static lines as -128
    int lvbytes = server_encode(lv, statics, -128);
    
    // send to the client
    if (send( sockfd, lv, lvbytes, 0 ) == -1)
	return ERROR; // return -1 if any of the sends fail
    
    sleep(3); // recv is threaded, so sending all of the data at once segfaults because the buffer becomes bigger than the length specified at the beginning of the array
    
    // draw a box made of 4 segments that falls into the hoop
    int box[] = {17, 150,100,150,150, 150,150,200,150, 200,150,200,100, 200,100,150,100};
    char bx[ (box[0] * 4) + 1 ];
    bx[0] = '\0';
    
    // encode the box as -121 to become a collision body
    int bxbytes = server_encode(bx, box, -121);

    if (send( sockfd, bx, bxbytes, 0 ) == -1)
	return ERROR;
    
    sleep(3);
    
    // box that starts off the screen and falls onto the level
    int box2[] = {17, 350,850,350,900, 350,900,400,900, 400,900,400,850, 400,850,350,850};
    char bx2[ (box2[0] * 5) + 1 ];
    bx2[0] = '\0';
    
    bxbytes = server_encode(bx2, box2, -121);
    
    if (send( sockfd, bx2, bxbytes, 0 ) == -1)
	return ERROR;
    
    return 1;
}

static int server_encode(char *buf, const int *level, int type) { 
    char form[12]; // largest possible int, plus a char and \0
    
    // initialize the first buffer with the flag char and a null pointer
    char first[] = {(char) type, '\0'}; 
    strcat(buf, first);
    
    for (int index = 0; index < level[0]; index++) {
	// add the integer and a SEPARATE char to the temporary buffer and append to the final buffer
        sprintf(form, "%d%c", level[index], SEPARATE);
        strcat(buf, form);
    }
    
    return strlen(buf); // return number of bytes in the string
}