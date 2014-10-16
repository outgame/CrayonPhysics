// chat server using Beej's guide code as an example
#include "server.h"

#ifdef DEBUG
static int server_listen_tcp_local(char *server_port);

int main(int argc, char **argv) {
    return server_start();
}
#endif

int server_start() {
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
    int listener = server_listen_tcp_local("5993");
    if (listener < 0)
        return EXIT_FAILURE;

    // add listener to list of file descriptors because accept is blocking
    FD_SET(listener, &master);
    fdmax = listener; //listener is the largest file descriptor right now
    
    printf("waiting to accept a socket connection request...\n");
    
    while (TRUE) {
	// copy master list
        read_fds = master;
	
	sel = select(fdmax+1, &read_fds, NULL, NULL, &wait);
	
        if (sel == -1) {
            fprintf(stderr, "select failed\n");
            return EXIT_FAILURE;
        }
	else if (sel == 0) {
	    printf("select timed out\n");
            break;
	}
	
        // loop through sockets
        for(int connfd = 0; connfd <= fdmax; connfd++) {
            if ( FD_ISSET(connfd, &read_fds) ) {
                
		// socket is the listener, accept the new connection
                if (connfd == listener) { 
		    master = server_accept(master, listener, connfd, &fdmax);
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

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    
    if (sa->sa_family == AF_INET)
	return &(( (struct sockaddr_in*) sa)->sin_addr);
    
    return &(( (struct sockaddr_in6*)sa )->sin6_addr);
}

int server_listen_tcp(int server_port) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: bind the socket to an address
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;  // INET family of protocols

    // use default address of this machine.  
    // make sure to convert to network byte order
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons(server_port);

    bind(listenfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));

    // step 3:  convert listenfd to a passive "listening" socket
    listen(listenfd, LISTENQ);
    return listenfd;
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
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenfd < 0) { 
            continue;
        }
        
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        
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
    
    if (listen(listenfd, LISTENQ) == -1) {
        fprintf(stderr, "listen failed\n");
        return ERROR;
    }
    
    return listenfd;
}

fd_set server_accept(fd_set master, int listener, int connfd, int *fdmax) {
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen = sizeof remoteaddr;
    
    char remoteIP[INET6_ADDRSTRLEN];
    
    int newfd = accept(listener, (struct sockaddr*) &remoteaddr, &addrlen);
    
    if (newfd == -1)
        fprintf(stderr, "accept failed\n");
	
    else { // if it didn't fail, add to the file descriptor set
	FD_SET(newfd, &master);
	
	if (newfd > *fdmax)  // update the greatest fd
	    *fdmax = newfd;
	    
	printf("new connection from %s on socket %d\n",
	    inet_ntop(remoteaddr.ss_family,
	    get_in_addr((struct sockaddr*) &remoteaddr), remoteIP, INET6_ADDRSTRLEN),
	    newfd);
    }
    
    return master;
}

fd_set server_recv_send(fd_set master, int listener, int connfd, int fdmax) {
    char buf[MAXLINE]; // buffer for client data
    int nbytes;
    
    // receive the data
    if ((nbytes = recv(connfd, buf, sizeof(buf), 0)) <= 0) {
	
	if (nbytes == 0) // client closed the connection
	    printf("socket %d hung up\n", connfd);
	else
	    fprintf(stderr, "recv failed\n");
	
	close(connfd);
	FD_CLR(connfd, &master); // remove from master set
    }
    
    else { // actual data came from the client
	
	for(int otherfd = 0; otherfd <= fdmax; otherfd++) {
	    if (FD_ISSET(otherfd, &master)) {
		
		// send to all connections except listener and sender
		if (otherfd != listener && otherfd != connfd) {
		    if ( send( otherfd, buf, nbytes, 0 ) == -1 )
			fprintf(stderr, "send failed\n");
		    
		    //printf(getpeername(), inet_ntop(), getnameinfo(), or gethostbyaddr())
		}
	    }
	}
    }
    
    return master;
}
