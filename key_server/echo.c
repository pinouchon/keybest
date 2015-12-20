#include <ApplicationServices/ApplicationServices.h>

#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>                // for gettimeofday()
#include <math.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int bumpHSpeed = 0;
int bumpVSpeed = 0;
pthread_mutex_t lock;

void PostKeyWithModifiers(CGKeyCode key, CGEventFlags modifiers)
{
        CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);

        CGEventRef keyDown = CGEventCreateKeyboardEvent(source, key, TRUE);
        //CGEventSetFlags(keyDown, modifiers);
        CGEventRef keyUp = CGEventCreateKeyboardEvent(source, key, FALSE);

        CGEventPost(kCGAnnotatedSessionEventTap, keyDown);
        CGEventPost(kCGAnnotatedSessionEventTap, keyUp);

        CFRelease(keyUp);
        CFRelease(keyDown);
        CFRelease(source);
}

float getFrequency(int initialBump, double time) {
  float result;
  result = (float)initialBump * pow(0.7, time / 600 + 1.2);
//  printf("%f, %f\n", time / 1000 + 1, result);
  return result;
}

void* keyThread(void *threadid)
{
   int HSpeed = 0;
   int VSpeed = 0;
   int initialHSpeed = 0;
   int initialVSpeed = 0;
   long tid;
   tid = (long)threadid;
   struct timeval t1, t2, t3;
   double lastBumpSince = 0;
   double lastTickSince = 0;
   double frequency = 0;
   long long int i = 0;

   printf("Hello World! It's me, thread #%ld!\n", tid);
   gettimeofday(&t2, NULL);

   while(1) {
     pthread_mutex_lock(&lock);
     if (bumpHSpeed == 1) {
       puts("TO 1");
       gettimeofday(&t1, NULL);
       gettimeofday(&t3, NULL);
       HSpeed += 1200;
       initialHSpeed = HSpeed;
       bumpHSpeed = 0;
       frequency = 0;
     } else if (bumpHSpeed == 10) {
     puts("TO ZERO");
       HSpeed = 0;
       bumpHSpeed = 0;
       frequency = 0;
     }
     pthread_mutex_unlock(&lock);

     if (HSpeed > 100) {
       gettimeofday(&t2, NULL);
       lastBumpSince = (t2.tv_sec - t1.tv_sec) * 1000.0;
       lastBumpSince += (t2.tv_usec - t1.tv_usec) / 1000.0;

       lastTickSince = (t2.tv_sec - t3.tv_sec) * 1000.0;
       lastTickSince += (t2.tv_usec - t3.tv_usec) / 1000.0;


       if (lastTickSince > frequency) {
//         puts("Tick!");
         PostKeyWithModifiers(0x7b, 0x38);

         HSpeed = getFrequency(initialHSpeed, lastBumpSince);
         frequency = (float)10000 / (float)HSpeed;
         gettimeofday(&t3, NULL);
       }
     }
     usleep(10000);
     i++;
   }
   pthread_exit(NULL);
}

void startThread() {
    pthread_t my_thread;
    int rc;
    long t;

    puts("init mutex");
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        exit(-1);
    }
    puts("starting thread");
    rc = pthread_create(&my_thread, NULL, keyThread, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
    puts("done");
}

int main (int argc, char *argv[]) {
  if (argc < 2) on_error("Usage: %s [port]\n", argv[0]);

  startThread();

  int port = atoi(argv[1]);

  int server_fd, client_fd, err;
  struct sockaddr_in server, client;
  char buf[BUFFER_SIZE];

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) on_error("Could not create socket\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

  err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
  if (err < 0) on_error("Could not bind socket\n");

  err = listen(server_fd, 128);
  if (err < 0) on_error("Could not listen on socket\n");

  printf("Server is listening on %d\n", port);

  int i = 0;
  int j = 0;
  while (1) {
    socklen_t client_len = sizeof(client);
    client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

    if (client_fd < 0) on_error("Could not establish new connection\n");

    while (1) {
      int read = recv(client_fd, buf, BUFFER_SIZE, 0);

      if (!read) break; // done reading
      if (read < 0) on_error("Client read failed\n");
      err = send(client_fd, buf, read, 0);
      if (err < 0) on_error("Client write failed\n");

      puts("Received:");
      puts(buf);
      printf("%i\n", i);
      printf("%i\n", j);
      puts("====================");
      for(int n = 0; n < 100; n++){
        //PostKeyWithModifiers(4, 0);
      }
      pthread_mutex_lock(&lock);
      if (strcmp(buf, "left\n") == 0) {
        printf("LEFT: (%s)", buf);
        bumpHSpeed = 1;
      } else if (strcmp (buf, "right\n") == 0) {
        printf("RIGHT: (%s)", buf);
      } else if (strcmp (buf, "zero\n") == 0) {
        printf("ZERO: (%s)", buf);
        bumpHSpeed = 10;
      }

      pthread_mutex_unlock(&lock);

//      return 0;

      j++;
    }
    close(client_fd);
    i++;
  }

  return 0;
}