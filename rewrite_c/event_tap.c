// #import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
  //0x0b is the virtual keycode for "b"
  //0x09 is the virtual keycode for "v"
  printf("KEY: %i\n", CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode));
  if (CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode) == 0x0B) {
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 0x09);
  }

  return event;
}

int main(int argc, char *argv[]) {
  //NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  CFRunLoopSourceRef runLoopSource;

  CFMachPortRef eventTap = CGEventTapCreate(
    kCGHIDEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault, kCGEventMaskForAllEvents, myCGEventCallback, NULL
  );

  if (!eventTap) {
    printf("Couldn't create event tap!\n");
    exit(1);
  }

  runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

  CGEventTapEnable(eventTap, true);

  CFRunLoopRun();

  CFRelease(eventTap);
  CFRelease(runLoopSource);
  //pool.release();

  exit(0);
}