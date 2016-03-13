class PrivatexmlController < ApplicationController
  respond_to :xml


  def index
    @dvorak_mapping = qgmly_mapping
    @event_mapping = event_mapping

    respond_to do |format|
      format.xml # index.xml.erb
    end
  end

  def dvorak_mapping
    {Q: 'KEY_4', W: 'M', E: ['COMMA', 'SHIFT_L'], R: 'P', T: 'Y', Y: 'F', U: 'G', I: 'C', O: 'R', P: 'L',
     A: 'Q', S: 'O', D: 'E', F: 'U', G: 'I', H: 'D', J: 'H', K: 'T', L: 'N', SEMICOLON: 'S',
     X: 'A', C: 'J', V: 'K', B: 'X', N: 'B', M: 'SEMICOLON', COMMA: 'Z', DOT: 'V', SLASH: 'W'
    # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def qgmly_mapping
    {Q: 'A', W: 'G', E: 'SEMICOLON', R: 'L', T: 'Z', Y: 'Y', U: 'F', I: 'U', O: 'B', P: 'A',
     A: 'D', S: 'S', D: 'T', F: 'N', G: 'R', H: 'I', J: 'Q', K: 'E', L: 'O', SEMICOLON: 'H',
     Z: 'W', X: 'X', C: 'C', V: 'V', B: 'J', N: 'K', M: 'P', COMMA: 'M', DOT: ['COMMA', 'SHIFT_L'], SLASH: ['COMMA', 'SHIFT_L']
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def custom_mapping
    {   }
  end

  def event_mapping
    {'@' => 'DANNISH_DOLLAR', '&' => 'KEY_1', 'é' => 'KEY_2', '"' => 'KEY_3', "'" => 'KEY_4', '(' => 'KEY_5', '§' => 'KEY_6', 'è' => 'KEY_7', '!' => 'KEY_8', 'c' => 'KEY_9', 'à' => 'KEY_0', ')' => 'MINUS', '-' => 'EQUAL', 'backspace' => 'DELETE',
     'tab' => 'TAB', 'a' => 'Q', 'z' => 'W', 'e' => 'E', 'r' => 'R', 't' => 'T', 'y' => 'Y', 'u' => 'u', 'i' => 'I', 'o' => 'O', 'p' => 'P', '^' => 'BRACKET_LEFT', '$' => 'BRACKET_RIGHT', 'enter' => 'RETURN',
     'caps' => 'JIS_EISUU', 'q' => 'A', 's' => 'S', 'd' => 'D', 'f' => 'F', 'g' => 'G', 'h' => 'H', 'j' => 'J', 'k' => 'K', 'l' => 'L', 'm' => 'SEMICOLON', 'ù' => 'QUOTE', '`' => 'BACKSLASH',
     'shift_l' => 'SHIFT_L', '<' => 'BACKQUOTE', 'w' => 'Z', 'x' => 'X', 'c' => 'C', 'v' => 'V', 'b' => 'B', 'n' => 'N', ',' => 'M', ';' => 'COMMA', ':' => 'DOT', '=' => 'SLASH', 'shift_r' => 'SHIFT_R',
     'fn' => 'FN', 'crtl_l' => 'CONTROL_L', 'alt_l' => 'OPTION_L', 'cmd_l' => 'COMMAND_L', 'space' => 'SPACE', 'cmd_r' => 'COMMAND_R', 'OPTION_R' => 'a'
    }.with_indifferent_access
  end
end

__END__
======================FR LAYOUT======================
          @  &  é  "  '  (  §  è  !  ç  à  )  -     backspace
          tab a  z  e  r  t  y  u  i  o  p  ^  $    ----
          caps q  s  d  f  g  h  j  k  l  m  ù  `  enter
     shift_l <  w  x  c  v  b  n  ,  ;  :  =   shift_r
fn ctrl_l alt_l cmd_l [           ] cmd_r alt_r

======================DVORAK LAYOUT======================
          §  1  2  3  4  5  6  7  8  9  0  [  ]     backspace
          tab '  ,  .  p  y  f  g  c  r  l  /  =    ----
          caps a  o  e  u  i  d  h  t  n  s  -  \  enter
     shift_l `  ;  q  j  k  x  b  m  w  v  z   shift_r
fn ctrl_l alt_l cmd_l [           ] cmd_r alt_r

======================EVENTS=========================
ESCAPE
@             &     é     "     '     (     §     è     !     ç     à     )     -      <--
DANISH_DOLLAR KEY_1 KEY_2 KEY_3 KEY_4 KEY_5 KEY_6 KEY_7 KEY_8 KEY_9 KEY_0 MINUS EQUAL DELETE

->  a         z     e     r     t     y     u     i     o     p     ^            $             <-
TAB Q         W     E     R     T     Y     U     I     O     P     BRACKET_LEFT BRACKET_RIGHT RETURN

[^]           q     s     d     f     g     h     j     k     l     m          ù     `         <-
JIS_EISUU     A     S     D     F     G     H     J     K     L     SEMICOLON  QUOTE BACKSLASH RETURN

^       <         w x     c     v     b     n     ,     ;     :     =          [  ^  ]
SHIFT_L BACKQUOTE Z X     C     V     B     N     M     COMMA DOT   SLASH      SHIFT_R

n  ^         \        .          [           ]  .         \
FN CONTROL_L OPTION_L COMMAND_L  SPACE          COMMAND_R OPTION_R          CURSOR_LEFT UP RIGHT DOWN
