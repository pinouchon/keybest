
#include <ApplicationServices/ApplicationServices.h>
#include <stdlib.h>

int selectState = 0;

//void sendKeyCode(int code) {
//    CGEventRef event1, event2, e5;
//    event1 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)code, true);//'z' keydown event
//    //CGEventSetFlags(event1, kCGEventFlagMaskAlternate | kCGEventFlagMaskShift);//set shift key down for above event
//    CGEventPost(kCGSessionEventTap, event1);//post event
//    event2 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)code, false);
//    //CGEventSetFlags(event2, kCGEventFlagMaskAlternate | kCGEventFlagMaskShift);
//    CGEventPost(kCGSessionEventTap, event2);
//    e5 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)56, false);
//    CGEventPost(kCGSessionEventTap, e5);
//}

// This callback will be invoked every time there is a keystroke.
//
CGEventRef
myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
                  CGEventRef event, void *refcon)
{
  if (type == kCGEventTapDisabledByTimeout) {
    puts("Event Taps Disabled! Re-enabling");
    CGEventTapEnable(event, true);
    return event;
  }
  // (type != kCGEventKeyUp) &&
  if (type != kCGEventKeyDown)
    return event;

  // The incoming keycode.
  CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
							     event, kCGKeyboardEventKeycode);
  printf("key:[%i]\n", keycode);
  int retEvt = 1;
  if (keycode == (CGKeyCode)24) {
    exit(0);
  }
  else if (keycode == (CGKeyCode)4) { return event; }


  return event;
  //return retEvt == 1 ? event : NULL;
}

int
main(void)
{
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
