#include <ApplicationServices/ApplicationServices.h>

#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>                // for gettimeofday()
#include <math.h>

#define BUFFER_SIZE 1024
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

int bumpHSpeed = 0;
int bumpVSpeed = 0;
pthread_mutex_t lock;
pthread_t my_thread;

int LeftArrow = 0x7B;
int RightArrow = 0x7C;
int DownArrow = 0x7D;
int UpArrow = 0x7E;
int Direction = NULL;
int ArrowKeyPressed = 0;

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
  result = (float)initialBump * pow(0.2, time / 2000 + 1.2);
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
       //puts("TO 1");
       gettimeofday(&t1, NULL);
       gettimeofday(&t3, NULL);
       HSpeed += 8000;
       initialHSpeed = HSpeed;
       bumpHSpeed = 0;
       frequency = 0;
     } else if (bumpHSpeed == 10) {
       //puts("TO ZERO");
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
         PostKeyWithModifiers(Direction, 0x38);

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

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
  if (type == kCGEventTapDisabledByTimeout) {
    puts("Event Taps Disabled! Re-enabling");
    CGEventTapEnable(event, true);
    return event;
  }

  //if (type == kCGEventKeyDown) { puts("keyDown"); }
  //if (type == kCGEventKeyUp) { puts("keyUp"); }

  // (type != kCGEventKeyUp) &&
  if (type != kCGEventKeyDown && type != kCGEventKeyUp)
    return event;

  // The incoming keycode.
  CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
							     event, kCGKeyboardEventKeycode);

  int keyUp = 149;
  int keyDown = 150;
  int keyLeft = 151;
  int keyRight = 152;

  CGEventFlags flags = CGEventGetFlags(event);
  bool controlDown = (flags & kCGEventFlagMaskControl) == kCGEventFlagMaskControl;
  bool shiftDown   = (flags & kCGEventFlagMaskShift) == kCGEventFlagMaskShift;
  bool commandDown = (flags & kCGEventFlagMaskCommand) == kCGEventFlagMaskCommand;
  bool altDown     = (flags & kCGEventFlagMaskAlternate) == kCGEventFlagMaskAlternate;

  if (type == kCGEventKeyDown) {
    printf("DOWN:[%i] FLAGS[%i] [%i %i %i %i]\n", keycode, flags, controlDown, shiftDown, commandDown, altDown);
  }
  if (type == kCGEventKeyUp) {
    printf("DOWN:[%i] FLAGS[%i]\n", keycode, flags);
  }


  int retEvt = 1;
  int arrowKeyJustPressed = 0;
  if (keycode == (CGKeyCode)53) {
    pthread_cancel(my_thread);
    exit(0);
  } else if (keycode == (CGKeyCode)keyUp ||
             keycode == (CGKeyCode)keyDown ||
             keycode == (CGKeyCode)keyLeft ||
             keycode == (CGKeyCode)keyRight) {
    if (ArrowKeyPressed == 0 && type == kCGEventKeyDown) {
      arrowKeyJustPressed = 1;
      ArrowKeyPressed = 1;
    }
    if (type == kCGEventKeyUp && ArrowKeyPressed == 1) {
      ArrowKeyPressed = 0;
      printf("STOP %i\n", keycode);

      bumpHSpeed = 10;
    }

    pthread_mutex_lock(&lock); ////////
    if (arrowKeyJustPressed && keycode == (CGKeyCode)keyUp) {
      puts("D");
      Direction = UpArrow;
      bumpHSpeed = 1;
    }
    if (arrowKeyJustPressed && keycode == (CGKeyCode)keyDown) {
      puts("C");
      Direction = DownArrow;
      bumpHSpeed = 1;
    }
    if (arrowKeyJustPressed && keycode == (CGKeyCode)keyLeft) {
      puts("A");
      Direction = LeftArrow;
      bumpHSpeed = 1;
    }
    if (arrowKeyJustPressed && keycode == (CGKeyCode)keyRight) {
      puts("B");
      Direction = RightArrow;
      bumpHSpeed = 1;
    }
    pthread_mutex_unlock(&lock); ////////
    retEvt = 0;
  }

  return retEvt == 1 ? event : NULL;
}

void start_event_look(void) {
  CFMachPortRef      eventTap;
  CGEventMask        eventMask;
  CFRunLoopSourceRef runLoopSource;

  // Create an event tap. We are interested in key presses.
  eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
  eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0,
			      eventMask, myCGEventCallback, NULL);
  if (!eventTap) {
    fprintf(stderr, "failed to create event tap\n");
    exit(1);
  }

  // Create a run loop source.
  runLoopSource = CFMachPortCreateRunLoopSource(
						kCFAllocatorDefault, eventTap, 0);

  // Add to the current run loop.
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,
		     kCFRunLoopCommonModes);

  // Enable the event tap.
  CGEventTapEnable(eventTap, true);

  // Set it all running.
  CFRunLoopRun();

  // In a real program, one would have arranged for cleaning up.
  exit(0);
}

int main(int argc, char *argv[]) {
  startThread();
  start_event_look();
}


//int main_socket(int argc, char *argv[]) {
//  if (argc < 2) on_error("Usage: %s [port]\n", argv[0]);
//
//  startThread();
//
//  while (1) {
//
//    // if (client_fd < 0) on_error("Could not establish new connection\n");
//
//    while (1) {
//
//      puts("Received:");
//      puts(buf);
//      printf("%i\n", i);
//      printf("%i\n", j);
//      puts("====================");
//      pthread_mutex_lock(&lock); ////////
//      if (strcmp(buf, "left\n") == 0) {
//        printf("LEFT: (%s)", buf);
//        bumpHSpeed = 1; ////////
//      } else if (strcmp (buf, "right\n") == 0) {
//        printf("RIGHT: (%s)", buf);
//      } else if (strcmp (buf, "zero\n") == 0) {
//        printf("ZERO: (%s)", buf);
//        bumpHSpeed = 10; ////////
//      }
//
//      pthread_mutex_unlock(&lock); ////////
//
////      return 0;
//    }
//  }
//
//  return 0;
//}