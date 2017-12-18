// alterkeys.c
// http://osxbook.com
//
// Complile using the following command line:
//     gcc -Wall -o alterkeys alterkeys.c -framework ApplicationServices
//
// You need superuser privileges to create the event tap, unless accessibility
// is enabled. To do so, select the "Enable access for assistive devices"
// checkbox in the Universal Access system preference pane.

// start with: gcc -Wall -o alterkeys alterkeys.c -framework ApplicationServices && sudo ./alterkeys

#include <ApplicationServices/ApplicationServices.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>                // for gettimeofday()

pthread_mutex_t lock;
int just_pressed = 0;
int pressing_up = 0;    // int releasing_up;
int pressing_down = 0;  // int releasing_down;
int pressing_left = 0;  // int releasing_left;
int pressing_right = 0; // int releasing_right;

int gamma_state = 0;
int alpha_state = 0;
int beta_state = 0;
int current_layout = 2;

#define AZERTY 1
#define KEYBEST 2
#define CAESAR 3
#define KEY_A 12
#define KEY_B 11
#define KEY_C 8
#define KEY_D 2
#define KEY_E 14
#define KEY_F 3
#define KEY_G 5
#define KEY_H 4
#define KEY_I 34
#define KEY_J 38
#define KEY_K 40
#define KEY_L 37
#define KEY_M 41
#define KEY_N 45
#define KEY_O 31
#define KEY_P 35
#define KEY_Q 0
#define KEY_R 15
#define KEY_S 1
#define KEY_T 17
#define KEY_U 32
#define KEY_V 9
#define KEY_W 6
#define KEY_X 7
#define KEY_Y 16
#define KEY_Z 13

#define A__ (CGKeyCode)12
#define B__ (CGKeyCode)11
#define C__ (CGKeyCode)8
#define D__ (CGKeyCode)2
#define E__ (CGKeyCode)14
#define F__ (CGKeyCode)3
#define G__ (CGKeyCode)5
#define H__ (CGKeyCode)4
#define I__ (CGKeyCode)34
#define J__ (CGKeyCode)38
#define K__ (CGKeyCode)40
#define L__ (CGKeyCode)37
#define M__ (CGKeyCode)41
#define N__ (CGKeyCode)45
#define O__ (CGKeyCode)31
#define P__ (CGKeyCode)35
#define Q__ (CGKeyCode)0
#define R__ (CGKeyCode)15
#define S__ (CGKeyCode)1
#define T__ (CGKeyCode)17
#define U__ (CGKeyCode)32
#define V__ (CGKeyCode)9
#define W__ (CGKeyCode)6
#define X__ (CGKeyCode)7
#define Y__ (CGKeyCode)16
#define Z__ (CGKeyCode)13
#define __0 (CGKeyCode)29
#define __1 (CGKeyCode)18
#define __2 (CGKeyCode)19
#define __3 (CGKeyCode)20
#define __4 (CGKeyCode)21
#define __5 (CGKeyCode)23
#define __6 (CGKeyCode)22
#define __7 (CGKeyCode)26
#define __8 (CGKeyCode)28
#define __9 (CGKeyCode)25

#define KC(x) (CGKeyCode)x
#define SET_KC(x) keycode = (CGKeyCode)x
#define __NULL (CGKeyCode)NULL

#define __SHIFT (flags & NX_SHIFTMASK)
#define __CONTROL (flags & NX_CONTROLMASK)
#define __ALT (flags & NX_ALTERNATEMASK)
#define __CMD (flags & NX_COMMANDMASK)

#define SET_NONE CGEventSetFlags(*event, 0)
#define SET_SHIFT CGEventSetFlags(*event, *flags | NX_SHIFTMASK)
#define SET_CONTROL CGEventSetFlags(*event, *flags | NX_CONTROLMASK)
#define SET_ALT CGEventSetFlags(*event, *flags | NX_ALTERNATEMASK)
#define SET_CMD CGEventSetFlags(*event, *flags | NX_COMMANDMASK) // *flags |
#define SET_SHIFT_ALT CGEventSetFlags(*event, *flags | (NX_SHIFTMASK | NX_ALTERNATEMASK))
#define SET_FN CGEventSetFlags(*event, *flags | NX_SECONDARYFNMASK)

void PostKeyWithModifiers(CGKeyCode key, CGEventFlags modifiers)
{
        CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);

        CGEventRef keyDown = CGEventCreateKeyboardEvent(source, key, TRUE);
        CGEventSetFlags(keyDown, modifiers);
        CGEventRef keyUp = CGEventCreateKeyboardEvent(source, key, FALSE);

        CGEventPost(kCGAnnotatedSessionEventTap, keyDown);
        CGEventPost(kCGAnnotatedSessionEventTap, keyUp);

        CFRelease(keyUp);
        CFRelease(keyDown);
        CFRelease(source);
}

void* keyThread(void *threadid)
{
   long tid;
   tid = (long)threadid;
   struct timeval t1, t2;
   double lastTickSince = 0;

   //printf("Hello World! It's me, thread #%ld!\n", tid);
   gettimeofday(&t1, NULL);

   while(1) {
     pthread_mutex_lock(&lock);

     gettimeofday(&t1, NULL);

     pthread_mutex_unlock(&lock);

     lastTickSince = (t1.tv_sec - t2.tv_sec) * 1000.0;
     lastTickSince += (t1.tv_usec - t2.tv_usec) / 1000.0;

     if (just_pressed || lastTickSince > (50)) {
       just_pressed = 0;
       gettimeofday(&t2, NULL);
       if (pressing_up) { PostKeyWithModifiers(KC(126), 0); }
       if (pressing_down) { PostKeyWithModifiers(KC(125), 0); }
       if (pressing_left) { PostKeyWithModifiers(KC(123), 0); }
       if (pressing_right) { PostKeyWithModifiers(KC(124), 0); }
     }
     if (current_layout == CAESAR) {
       usleep(10000);
     } else {
       usleep(900000);//0
     }

   }
   pthread_exit(NULL);
}

void startThread() {
    pthread_t my_thread;
    int rc;
    void *t = (void *)NULL;

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
}

int noModifierMapping(int keycode, CGEventRef* event, CGEventFlags* flags) {
// TOP ROW
    if (keycode == A__) keycode = Q__;
    else if (keycode == Z__) keycode = G__;
    else if (keycode == E__) keycode = M__;
    else if (keycode == R__) keycode = L__;
    else if (keycode == T__) keycode = W__;
    else if (keycode == Y__) keycode = Y__;
    else if (keycode == U__) keycode = F__;
    else if (keycode == I__) keycode = U__;
    else if (keycode == O__) keycode = B__;
    else if (keycode == P__) keycode = O__;//?
    else if (keycode == KC(33)) keycode = O__;//?
    else if (keycode == KC(30)) keycode = O__;//?

    // MIDDLE ROW
//    else if (keycode == KC(71)) keycode = NULL;//?
    else if (keycode == Q__) keycode = D__;
    else if (keycode == S__) keycode = S__;
    else if (keycode == D__) keycode = T__;
    else if (keycode == F__) keycode = N__;
    else if (keycode == G__) keycode = R__;
    else if (keycode == H__) keycode = I__;
    else if (keycode == J__) keycode = A__;
    else if (keycode == K__) keycode = E__;
    else if (keycode == L__) keycode = O__;
//    else if (keycode == M__) keycode = O__;///////
    else if (keycode == KC(39)) keycode = H__;
    else if (keycode == KC(42)) keycode = O__;//?

    // BOTTOM ROW
//    else if (keycode == KC(50)) keycode = O__;//? BETA
    else if (keycode == W__) keycode = Z__;
    else if (keycode == X__) keycode = X__;
    else if (keycode == C__) keycode = C__;
    else if (keycode == V__) keycode = V__;
    else if (keycode == B__) keycode = J__;
    else if (keycode == N__) keycode = K__;
    else if (keycode == KC(46)) keycode = P__;
    else if (keycode == KC(43)) {keycode = KC(43);SET_SHIFT;}
    else if (keycode == KC(47)) keycode = KC(46);
    //else if (keycode == KC(44)) keycode = KC(51);//backspace

    // NUMBERS ROW
    else if (keycode == KC(10)) {SET_KC(48);}
    else if (keycode == __0) {SET_SHIFT;}
    else if (keycode == __1) {SET_SHIFT;}
    else if (keycode == __2) {SET_SHIFT;}
    else if (keycode == __3) {SET_SHIFT;}
    else if (keycode == __4) {SET_SHIFT;}
    else if (keycode == __5) {SET_SHIFT;}
    else if (keycode == __6) {SET_SHIFT;}
    else if (keycode == __7) {SET_SHIFT;}
    else if (keycode == __8) {SET_SHIFT;}
    else if (keycode == __9) {SET_SHIFT;}

    return keycode;
}

//<!--SPEED-->
//    <autogen>
//      __KeyToKey__ KeyCode::D
//     __{ KeyCode::KEY_5, ModifierFlag::SHIFT_L, ModifierFlag::OPTION_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::F
//      __{ KeyCode::MINUS, ModifierFlag::SHIFT_L, ModifierFlag::OPTION_L }__
//    </autogen>
//
//    <!--TOP ROW-->
//    <autogen>
//      __KeyToKey__ KeyCode::Q
//      __{ KeyCode::KEY_1, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::W
//      __{ KeyCode::KEY_2, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::E
//      __{ KeyCode::KEY_3, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::R
//      __{ KeyCode::KEY_4, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::T
//      __{ KeyCode::KEY_5, ModifierFlag::SHIFT_L }__
//    </autogen>
//
//    <!--MIDDLE ROW-->
//    <autogen>
//      __KeyToKey__ KeyCode::A
//      __{ KeyCode::KEY_9, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::S
//      __{ KeyCode::KEY_0, ModifierFlag::SHIFT_L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::G
//      __{ KeyCode::SLASH }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::H
//      __{ KeyCode::P }__
//    </autogen>
//
//    <!--BOTTOM ROW-->
//    <autogen>
//      __KeyToKey__ KeyCode::BACKQUOTE
//      __{ KeyCode::L }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::X
//      __{ KeyCode::T }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::V
//      __{ KeyCode::F }__
//    </autogen>
//    <autogen>
//      __KeyToKey__ KeyCode::B
//      __{ KeyCode::D }__
//    </autogen>
int caesarMapping(int keycode, CGEventRef* event, CGEventFlags* flags, int type, int *doReturn) {
  // top row
  if (0) {}
  else if (keycode == A__) {SET_KC(18);SET_SHIFT;}
  else if (keycode == Z__) {//SET_KC(19);SET_SHIFT;
    SET_KC(23);SET_SHIFT_ALT;
  }
  else if (keycode == E__) {//SET_KC(20);SET_SHIFT; --
    pthread_mutex_lock(&lock);
    pressing_up = type == kCGEventKeyDown ? 1 : 0;just_pressed = 1;
    pthread_mutex_unlock(&lock);
    *doReturn = 0;
  }
  else if (keycode == R__) {//SET_KC(21);SET_SHIFT;
    SET_KC(27);SET_SHIFT_ALT;
  }
  else if (keycode == T__) {SET_KC(23);SET_SHIFT;}

  // middle row
  else if (keycode == Q__) {SET_KC(20);SET_SHIFT;}
  else if (keycode == S__) {//SET_KC(29);SET_SHIFT;
    pthread_mutex_lock(&lock);
    pressing_left = type == kCGEventKeyDown ? 1 : 0;just_pressed = 1;
    pthread_mutex_unlock(&lock);
    *doReturn = 0;
  }
  else if (keycode == D__) {//SET_KC(23);SET_SHIFT_ALT; ---
    pthread_mutex_lock(&lock);
    pressing_down = type == kCGEventKeyDown ? 1 : 0;just_pressed = 1;
    pthread_mutex_unlock(&lock);
    *doReturn = 0;
  }
  else if (keycode == F__) {//SET_KC(27);SET_SHIFT_ALT; --
    pthread_mutex_lock(&lock);
    pressing_right = type == kCGEventKeyDown ? 1 : 0;just_pressed = 1;
    pthread_mutex_unlock(&lock);
    *doReturn = 0;
  }
  else if (keycode == G__) {SET_KC(44);}
  else if (keycode == H__) {SET_KC(35);}

  // bottom row
  else if (keycode == W__) {keycode = W__;}
  else if (keycode == X__) {keycode = T__;}
  else if (keycode == V__) {keycode = F__;}
  else if (keycode == B__) {keycode = D__;}

  else if (keycode == __1) {SET_KC(29);SET_SHIFT;}
  else if (keycode == __2) {SET_KC(19);SET_SHIFT;}
  else if (keycode == __3) {SET_KC(21);SET_SHIFT;}
  else if (keycode == __4) {SET_KC(21);SET_SHIFT;}
  return keycode;
}

int gammaMapping(int keycode, CGEventRef* event, CGEventFlags* flags) {
  // TOP ROW
  if (keycode == A__) {SET_KC(10);SET_SHIFT;}
  else if (keycode == Z__) {SET_KC(23);SET_SHIFT_ALT;}
  else if (keycode == E__) {SET_KC(23);SET_NONE;}
  else if (keycode == R__) {SET_KC(23);SET_ALT;}
  else if (keycode == T__) {SET_KC(50);SET_NONE;}
  else if (keycode == Y__) {SET_KC(50);SET_SHIFT;}
  else if (keycode == U__) {SET_KC(27);SET_ALT;}
  else if (keycode == I__) {SET_KC(27);SET_NONE;}
  else if (keycode == O__) {SET_KC(27);SET_SHIFT_ALT;}
  else if (keycode == P__) {SET_KC(33);SET_NONE;}
  else if (keycode == KC(33)) keycode = O__;//?
  else if (keycode == KC(30)) keycode = O__;//?

  // MIDDLE ROW
  //    else if (keycode == KC(71)) keycode = NULL;//?
  else if (keycode == Q__) {SET_KC(43);}
  else if (keycode == S__) {SET_KC(44);}
  else if (keycode == D__) {SET_KC(47);}
  else if (keycode == F__) {SET_KC(20);}
  else if (keycode == G__) {SET_KC(21);}
  else if (keycode == H__) {SET_KC(47);SET_SHIFT;}
  else if (keycode == J__) {SET_KC(24);SET_SHIFT;}
  else if (keycode == K__) {SET_KC(24);}
  else if (keycode == L__) {SET_KC(28);}
  //    else if (keycode == M__) keycode = O__;///////
  else if (keycode == KC(39)) keycode = O__;// NULL
  else if (keycode == KC(42)) keycode = O__;// NULL

  // BOTTOM ROW
  //  else if (keycode == KC(50)) keycode = O__;//? BETA
  else if (keycode == W__) {SET_KC(45);SET_ALT;}
  else if (keycode == X__) {SET_KC(46);SET_SHIFT;}
  else if (keycode == C__) {SET_KC(30);}
  else if (keycode == V__) {SET_KC(47);SET_SHIFT_ALT;}
  else if (keycode == B__) {SET_KC(18);}
  else if (keycode == N__) {SET_KC(44);SET_SHIFT;}
  else if (keycode == KC(46)) {SET_KC(30);SET_SHIFT;}
  else if (keycode == KC(43)) {SET_KC(43);SET_SHIFT;}
  else if (keycode == KC(47)) {SET_KC(46);SET_NONE;}
  //  else if (keycode == KC(44)) keycode = O__;//? DELTA

  // NUMBERS ROW
//  else if (keycode == __0) {SET_SHIFT;}
  else if (keycode == __1) {SET_KC(33);} // ^
  else if (keycode == __2) {SET_KC(37);SET_SHIFT_ALT;} // |
  else if (keycode == __3) {SET_KC(42);} // `
  else if (keycode == __4) {SET_KC(10);} // @
  else if (keycode == __5) {SET_KC(39);SET_SHIFT;} // %
  //else if (keycode == __6) {SET_SHIFT;}
  //else if (keycode == __7) {SET_SHIFT;}
  //else if (keycode == __8) {SET_SHIFT;}
  //else if (keycode == __9) {SET_SHIFT;}

  return keycode;
}

int alphaMapping(int keycode, CGEventRef* event, CGEventFlags* flags, int type) {
  // TOP ROW
  if (keycode == A__) {KC(10);SET_SHIFT;}
  else if (keycode == Z__) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(23), NX_SHIFTMASK | NX_ALTERNATEMASK);
      PostKeyWithModifiers(KC(27), NX_SHIFTMASK | NX_ALTERNATEMASK);
    }
    SET_KC(123);SET_NONE;
  } // []
  else if (keycode == E__) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(23), 0);
      PostKeyWithModifiers(KC(27), 0);
    }
    SET_KC(123);SET_NONE;
  } // ()
  else if (keycode == R__) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(23), NX_ALTERNATEMASK);
      PostKeyWithModifiers(KC(27), NX_ALTERNATEMASK);
    }
    SET_KC(123);SET_NONE;
  } // {}
  else if (keycode == T__) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(50), 0);
      PostKeyWithModifiers(KC(50), NX_SHIFTMASK);
    }
    SET_KC(123);SET_NONE;
  } // <>
  else if (keycode == Y__) {SET_KC(50);SET_SHIFT;}
  else if (keycode == U__) {SET_KC(123);SET_CMD;} // home
  else if (keycode == I__) {SET_KC(126);} // up
  else if (keycode == O__) {SET_KC(124);SET_CMD;} // end
  else if (keycode == P__) {SET_KC(33);SET_NONE;}
  else if (keycode == KC(33)) keycode = O__;//?
  else if (keycode == KC(30)) keycode = O__;//?

  // MIDDLE ROW
  //    else if (keycode == KC(71)) keycode = NULL;//?
  else if (keycode == Q__) {SET_KC(43);}
  else if (keycode == S__) {SET_KC(44);}
  else if (keycode == D__) {SET_KC(47);}
  else if (keycode == F__) {
    if (type == kCGEventKeyDown) {PostKeyWithModifiers(KC(20), 0);PostKeyWithModifiers(KC(20), 0);}
    SET_KC(123);SET_NONE;
  } // "
  else if (keycode == G__) {
    if (type == kCGEventKeyDown) {PostKeyWithModifiers(KC(21), 0);PostKeyWithModifiers(KC(21), 0);}
    SET_KC(123);SET_NONE;
  } // '
  else if (keycode == H__) keycode = O__;// NULL
  else if (keycode == J__) {SET_KC(123);} // left
  else if (keycode == K__) {SET_KC(125);} // down
  else if (keycode == L__) {SET_KC(124);} // right
  //    else if (keycode == M__) keycode = O__;///////
  else if (keycode == KC(39)) {SET_KC(51);SET_ALT;}// delete word
  else if (keycode == KC(42)) keycode = O__;// NULL

  // BOTTOM ROW
  //  else if (keycode == KC(50)) keycode = O__;//? BETA
  else if (keycode == W__) {SET_KC(45);SET_ALT;}
  else if (keycode == X__) {SET_KC(46);SET_SHIFT;}
  else if (keycode == C__) {SET_KC(36);}
  else if (keycode == V__) {SET_KC(47);SET_SHIFT_ALT;}
  else if (keycode == B__) {SET_KC(18);}
  else if (keycode == N__) {SET_KC(2);SET_CONTROL;} // right delete
  else if (keycode == KC(46)) {SET_KC(123);SET_ALT;} // prev word
  else if (keycode == KC(43)) {SET_KC(51);SET_ALT;} // delete word
  else if (keycode == KC(47)) {SET_KC(124);SET_ALT;} // next word
  else if (keycode == KC(44)) {SET_KC(51);}// backspace

  // NUMBERS ROW
//  else if (keycode == __0) {SET_SHIFT;}
  else if (keycode == __1) {SET_KC(33);} // ^
  else if (keycode == __2) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(37), NX_SHIFTMASK | NX_ALTERNATEMASK);
      PostKeyWithModifiers(KC(37), NX_SHIFTMASK | NX_ALTERNATEMASK);
    }
    SET_KC(123);SET_NONE;
  } // |
  else if (keycode == __3) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(42), 0);
      PostKeyWithModifiers(KC(42), 0);
    }
    SET_KC(123);SET_NONE;
  } // `
  else if (keycode == __4) {SET_KC(10);} // @
  else if (keycode == __5) {SET_KC(39);SET_SHIFT;} // %
  //else if (keycode == __6) {SET_SHIFT;}
  //else if (keycode == __7) {SET_SHIFT;}
  //else if (keycode == __8) {SET_SHIFT;}
  //else if (keycode == __9) {SET_SHIFT;}
  else if (keycode == KC(36)) {
    if (type == kCGEventKeyDown) {
      PostKeyWithModifiers(KC(36), *flags);
      PostKeyWithModifiers(KC(36), *flags);
    }
    SET_KC(123);SET_NONE;
  } // enter

  return keycode;
}

int betaMapping(int keycode, CGEventRef* event, CGEventFlags* flags, int type) {
  if (0) {}
  else if (keycode == J__) {SET_KC(29);}
  else if (keycode == K__) {SET_KC(19);}
  else if (keycode == L__) {
    if (type == kCGEventKeyDown) {PostKeyWithModifiers(KC(33), 0);}
    SET_KC(31);
  }
  else if (keycode == I__) {SET_KC(39);}
  else if (keycode == H__) {
    if (type == kCGEventKeyDown) {PostKeyWithModifiers(KC(33), 0);}
    SET_KC(34);
  }
  else if (keycode == KC(43)) {SET_KC(26);}
  else if (keycode == KC(47)) {
    if (type == kCGEventKeyDown) {PostKeyWithModifiers(KC(33), 0);}
    SET_KC(14);
  }

  return keycode;
}

/////////////////////////////////////////////////////////////////////////////////////
// CONTROLLER
/////////////////////////////////////////////////////////////////////////////////////

// This callback will be invoked every time there is a keystroke.
CGEventRef
myCGEventCallback(CGEventTapProxy proxy, CGEventType type,
                  CGEventRef event, void *refcon)
{
    // Paranoid sanity check.
    if ((type != kCGEventKeyDown) && (type != kCGEventKeyUp))
        return event;

    // The incoming keycode.
    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(
      event, kCGKeyboardEventKeycode
    );
    CGEventFlags flags = CGEventGetFlags(event);
    int doReturn = 1;


    // Swap 'a' (keycode=0) and 'z' (keycode=6).
    // printf("KEY: %i (%i)\n", keycode, current_layout);
//    printf("shift?: %i (%i)\n", flags & NX_SHIFTMASK);
//    printf("opt?: %i (%i)\n", flags & NX_ALTERNATEMASK);
//    printf("com?: %i (%i)\n", flags & NX_COMMANDMASK);


    if (keycode == __1 && __CONTROL != 0) {
      current_layout = AZERTY;
      return NULL;
    }
    if (keycode == __2 && __CONTROL != 0) {
      current_layout = KEYBEST;
      return NULL;
    }
    if (keycode == __3 && __CONTROL != 0) {
      current_layout = CAESAR;
      return NULL;
    }
    if (current_layout == AZERTY) {
      return event;
    }

    //printf("  --\n");
    if (__SHIFT != 0 && __CMD != 0 &&
        (keycode == __0 || keycode == __1 || keycode == __2 || keycode == __3 || keycode == __4 ||
         keycode == __5 || keycode == __6 || keycode == __7 || keycode == __8 || keycode == __9)) {
      return event;
    }



    if (keycode == (CGKeyCode)50 && __CMD != 0) { // cmd+tab = cmd+`
      keycode = (CGKeyCode)29;
      CGEventSetFlags(event, NX_COMMANDMASK);
      CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
      return event;
    }

    if (keycode == (CGKeyCode)48) { // ALPHA HYPER KEY
      doReturn = 0;
      if (type == kCGEventKeyDown) alpha_state = 1;
      if (type == kCGEventKeyUp) alpha_state = 0;
      return NULL;
    }
    if (keycode == (CGKeyCode)50) { // BETA HYPER KEY
      doReturn = 0;
      if (type == kCGEventKeyDown) {beta_state = 1;}
      if (type == kCGEventKeyUp) {beta_state = 0;}
      return NULL;
    }
    if (keycode == (CGKeyCode)41) { // GAMMA HYPER KEY
      doReturn = 0;
      if (type == kCGEventKeyDown) {gamma_state = 1;}
      if (type == kCGEventKeyUp) {gamma_state = 0;}
      return NULL;
    }

//printf("debug %i %i\n ", alpha_state, __CMD);
    if (alpha_state == 1 && __CMD != 0) {
      CGEventSetFlags(event, (~NX_COMMANDMASK & flags) | NX_SHIFTMASK);
      flags = CGEventGetFlags(event);
    }
    if (keycode == KC(44) && __CMD == 0) keycode = KC(51);//backspace

    // __SHIFT == 0 && __CONTROL == 0 && __ALT == 0 && __CMD == 0 &&
    if (gamma_state == 0 && alpha_state == 0 && beta_state == 0) {
      if (keycode == E__ && __CMD != 0) { // search
        keycode = F__;
      }
      else if (keycode == __1 && __CMD != 0) { // launch app
        keycode = T__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      }
      else if (keycode == __2 && __CMD != 0) { // launch app
        keycode = R__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      }
      else if (keycode == __3 && __CMD != 0) { // launch app
        keycode = C__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      }
      else if (keycode == __4 && __CMD != 0) { // launch app
        keycode = S__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      }
      else if (keycode == __5 && __CMD != 0) { // launch app
        keycode = N__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      }
      else if (keycode == __6 && __CMD != 0) { // launch app
        keycode = F__;
        CGEventSetFlags(event, NX_CONTROLMASK | NX_ALTERNATEMASK | NX_COMMANDMASK);
      } else {
        if (current_layout == KEYBEST) {
          keycode = noModifierMapping(keycode, &event, &flags);
        } else if (current_layout == CAESAR) {
          keycode = caesarMapping(keycode, &event, &flags, type, &doReturn);
        }
      }

//    } else if (__SHIFT != 0 && __CONTROL == 0 && __ALT == 0 && __CMD == 0 && gamma_state == 0 && alpha_state == 0) {
//      printf(" SHIFT ");
//      keycode = noModifierMapping(keycode, &event, &flags);
    } else if (gamma_state == 1) {
      //printf(" GAMMA ");
      keycode = gammaMapping(keycode, &event, &flags);
    } else if (alpha_state == 1) {
      //printf(" ALPHA");
      keycode = alphaMapping(keycode, &event, &flags, type);
    } else if (beta_state == 1) {
      keycode = betaMapping(keycode, &event, &flags, type);
    } else {
    }

     //flags = CGEventGetFlags(event);
     //printf("FLAGS: shift:%i control:%i alt:%i cmd:%i\n ", __SHIFT != 0, __CONTROL != 0, __ALT != 0, __CMD != 0);
    // Set the modified keycode field in the event.
    CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, (int64_t)keycode);
//    if (alpha_state == 1) {
//      CGEventSetFlags(event, flags | NX_SHIFTMASK); // flags |
//    }
    //CGEventSetFlags(keyDown, modifiers);

    // We must return the event for it to be useful.
    if (doReturn == 0) {return NULL;}
    return event;
}

int
main(void)
{
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    startThread();

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
