// compile and run from the commandline with:
//    clang  -framework coreFoundation  -framework IOKit  ./HID.c  -o hid
//    sudo ./hid

// This code works with the IOHID library to get notified of keys.
//   Still haven't figured out how to truly intercept with
//   substitution.

#include <IOKit/hid/IOHIDValue.h>
#include <IOKit/hid/IOHIDManager.h>

void myHIDKeyboardCallback( void* context,  IOReturn result,  void* sender,  IOHIDValueRef value )
{
    IOHIDElementRef elem = IOHIDValueGetElement( value );

    if (IOHIDElementGetUsagePage(elem) != 0x07)
        return;

    uint32_t scancode = IOHIDElementGetUsage( elem );

    if (scancode < 4 || scancode > 231)
        return;

    long pressed = IOHIDValueGetIntegerValue( value );

    printf( "scancode: %d, pressed: %ld\n", scancode, pressed );
}


CFMutableDictionaryRef myCreateDeviceMatchingDictionary( UInt32 usagePage,  UInt32 usage )
{
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(
                                                            kCFAllocatorDefault, 0
                                                        , & kCFTypeDictionaryKeyCallBacks
                                                        , & kCFTypeDictionaryValueCallBacks );
    if ( ! dict )
        return NULL;

    CFNumberRef pageNumberRef = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, & usagePage );
    if ( ! pageNumberRef ) {
        CFRelease( dict );
        return NULL;
    }

    CFDictionarySetValue( dict, CFSTR(kIOHIDDeviceUsagePageKey), pageNumberRef );
    CFRelease( pageNumberRef );

    CFNumberRef usageNumberRef = CFNumberCreate( kCFAllocatorDefault, kCFNumberIntType, & usage );

    if ( ! usageNumberRef ) {
        CFRelease( dict );
        return NULL;
    }

    CFDictionarySetValue( dict, CFSTR(kIOHIDDeviceUsageKey), usageNumberRef );
    CFRelease( usageNumberRef );

    return dict;
}


int main(void)
{
    IOHIDManagerRef hidManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDOptionsTypeNone );

    CFArrayRef matches;
    {
        CFMutableDictionaryRef keyboard = myCreateDeviceMatchingDictionary( 0x01, 6 );
        CFMutableDictionaryRef keypad   = myCreateDeviceMatchingDictionary( 0x01, 7 );

        CFMutableDictionaryRef matchesList[] = { keyboard, keypad };

        matches = CFArrayCreate( kCFAllocatorDefault, (const void **)matchesList, 2, NULL );
    }

    IOHIDManagerSetDeviceMatchingMultiple( hidManager, matches );

    IOHIDManagerRegisterInputValueCallback( hidManager, myHIDKeyboardCallback, NULL );

    IOHIDManagerScheduleWithRunLoop( hidManager, CFRunLoopGetMain(), kCFRunLoopDefaultMode );

    IOHIDManagerOpen( hidManager, kIOHIDOptionsTypeNone );

    CFRunLoopRun(); // spins
}