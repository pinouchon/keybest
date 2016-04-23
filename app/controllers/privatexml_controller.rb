class PrivatexmlController < ApplicationController
  respond_to :xml

  attr_reader :shift, :opt, :cmd, :shift_opt, :left

  def index
    define_helper_keys
    @dvorak_mapping = qgmly_mapping
    # @event_mapping = event_mapping
    @alpha_mapping = alpha_mapping
    @beta_mapping = beta_mapping
    @gamma_mapping = gamma_mapping
    @delta_mapping = delta_mapping

    respond_to do |format|
      format.xml # index.xml.erb
    end
  end

  # Deprecated
  def dvorak_mapping
    {Q: 'KEY_4', W: 'M', E: ['COMMA', 'SHIFT_L'], R: 'P', T: 'Y', Y: 'F', U: 'G', I: 'C', O: 'R', P: 'L',
     A: 'Q', S: 'O', D: 'E', F: 'U', G: 'I', H: 'D', J: 'H', K: 'T', L: 'N', SEMICOLON: 'S',
     X: 'A', C: 'J', V: 'K', B: 'X', N: 'B', M: 'SEMICOLON', COMMA: 'Z', DOT: 'V', SLASH: 'W'
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def qgmly_mapping
    {Q: 'A', W: 'G', E: 'SEMICOLON', R: 'L', T: 'Z', Y: 'Y', U: 'F', I: 'U', O: 'B', P: 'FORWARD_DELETE',
     A: 'D', S: 'S', D: 'T', F: 'N', G: 'R', H: 'I', J: 'Q', K: 'E', L: 'O', QUOTE: 'H',
     Z: 'W', X: 'X', C: 'C', V: 'V', B: 'J', N: 'K', M: 'P', COMMA: %w(COMMA SHIFT_L), DOT: 'M', SLASH: 'F'
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def alpha_mapping
    {# 1st line
     Q: nil, W: nil, E: nil, R: nil, T: nil, Y: nil,
     U: "__{ KeyCode::CURSOR_LEFT, #{cmd} }__",
     I: '__{ KeyCode::CURSOR_UP }__',
     O: "__{ KeyCode::CURSOR_RIGHT, #{cmd} }__",
     P: nil,

     # 2nd line
     A: "__{ KeyCode::BACKQUOTE, KeyCode::BACKQUOTE, #{shift}, #{left} }__",
     S: "__{ KeyCode::KEY_5, #{shift_opt}, KeyCode::MINUS, #{shift_opt}, #{left} }__",
     D: "__{ KeyCode::KEY_5, #{opt}, KeyCode::MINUS, #{opt}, #{left} }__",
     F: "__{ KeyCode::KEY_5, KeyCode::MINUS, #{left} }__",
     G: "__{ KeyCode::KEY_1, #{shift} }__",
     H: "__{ KeyCode::KEY_0, #{shift} }__",
     J: '__{ KeyCode::CURSOR_LEFT, KeyCode::VK_OPEN_URL_SHELL_nc_zero }__',
     K: '__{ KeyCode::CURSOR_DOWN, KeyCode::VK_OPEN_URL_SHELL_nc_zero }__',
     L: '__{ KeyCode::CURSOR_RIGHT, KeyCode::VK_OPEN_URL_SHELL_nc_zero }__',
     QUOTE: nil,

     # 3rd line
     Z: "__{ KeyCode::BACKSLASH, KeyCode::BACKSLASH, #{left} }__",
     X: "__{ KeyCode::KEY_4, KeyCode::KEY_4, #{left} }__",
     C: "__{ KeyCode::KEY_3, KeyCode::KEY_3, #{left} }__",
     V: '__{ KeyCode::RETURN }__',
     B: "__{ KeyCode::L, #{shift_opt}, KeyCode::L, #{shift_opt} }__",
     N: '__{ KeyCode::KEY_1, KeyCode::KEY_1 }__',
     M: "__{ KeyCode::CURSOR_LEFT, #{opt} }__",
     COMMA: nil,
     DOT: "__{ KeyCode::CURSOR_RIGHT, #{opt} }__"
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def beta_mapping
    {# 1st line
     Q: nil, W: nil, E: nil, R: nil, T: nil, Y: nil, U: nil, I: nil, O: nil, P: nil,

     # 2nd line
     A: '__{ KeyCode::BACKQUOTE }__',
     S: "__{ KeyCode::BACKQUOTE, #{shift} }__",
     D: "__{ KeyCode::KEY_5, #{opt} }__",
     F: "__{ KeyCode::MINUS, #{opt} }__",
     G: "__{ KeyCode::KEY_1, #{shift} }__",
     H: "__{ KeyCode::KEY_0, #{shift} }__",
     J: nil, K: nil, L: nil, QUOTE: nil,

     # 3rd line
     Z: "__{ KeyCode::KEY_5, #{shift_opt} }__",
     X: "__{ KeyCode::MINUS, #{shift_opt} }__",
     C: '__{ KeyCode::KEY_5 }__',
     V: '__{ KeyCode::MINUS }__',
     B: nil, N: nil, M: nil, COMMA: nil, DOT: nil
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def gamma_mapping
    {# 1st line
     Q: "__{ KeyCode::KEY_9, #{shift} }__",
     W: "__{ KeyCode::KEY_7, #{shift} }__",
     E: "__{ KeyCode::KEY_5, #{shift} }__",
     R: "__{ KeyCode::KEY_3, #{shift} }__",
     T: "__{ KeyCode::COMMA, #{shift} }__",
     Y: '__{ KeyCode::M }__',
     U: "__{ KeyCode::KEY_2, #{shift} }__",
     I: "__{ KeyCode::KEY_4, #{shift} }__",
     O: "__{ KeyCode::KEY_6, #{shift} }__",
     P: "__{ KeyCode::KEY_8, #{shift} }__",

     # 2nd line
     A: "__{ KeyCode::DOT, #{shift} }__",
     S: '__{ KeyCode::SLASH }__',
     D: '__{ KeyCode::EQUAL }__',
     F: "__{ KeyCode::EQUAL, #{shift} }__",
     G: "__{ KeyCode::KEY_1, #{shift} }__",
     H: "__{ KeyCode::KEY_0, #{shift} }__",
     J: '__{ KeyCode::COMMA }__',
     K: '__{ KeyCode::DOT }__',
     L: '__{ KeyCode::KEY_8 }__',
     QUOTE: nil,

     # 3rd line
     Z: '__{ KeyCode::BACKSLASH }__',
     X: '__{ KeyCode::KEY_4 }__',
     C: '__{ KeyCode::KEY_3 }__',
     V: "__{ KeyCode::SLASH, #{shift} }__",
     B: "__{ KeyCode::L, #{shift_opt} }__",
     N: '__{ KeyCode::KEY_1 }__',
     M: "__{ KeyCode::BRACKET_RIGHT, #{shift} }__",
     COMMA: "__{ KeyCode::QUOTE, #{shift} }__",
     DOT: "__{ KeyCode::N, #{opt} }__"
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def delta_mapping
    {# 1st line
     Q: nil,
     W: "__{ KeyCode::MINUS, #{shift} }__",
     E: "__{ KeyCode::BRACKET_LEFT, #{shift} }__",
     R: '__{ KeyCode::BRACKET_LEFT }__',
     T: '__{ KeyCode::KEY_6 }__',
     Y: nil,
     U: nil,
     I: '__{ KeyCode::QUOTE }__',
     O: nil,
     P: nil,

     # 2nd line
     A: "__{ KeyCode::DOT, #{shift_opt} }__",
     S: "__{ KeyCode::DANISH_DOLLAR, #{shift} }__",
     D: '__{ KeyCode::BRACKET_RIGHT }__',
     F: "__{ KeyCode::M, #{shift} }__",
     G: "__{ KeyCode::KEY_1, #{shift} }__",
     H: "__{ KeyCode::KEY_0, #{shift} }__",
     J: '__{ KeyCode::DANISH_DOLLAR }__',
     K: '__{ KeyCode::KEY_2 }__',
     L: '__{ KeyCode::BRACKET_LEFT, KeyCode::O }__',
     QUOTE: nil,

     # 3rd line
     Z: "__{ KeyCode::BACKSLASH, #{shift} }__",
     X: "__{ KeyCode::BRACKET_RIGHT, #{opt} }__",
     C: '__{ KeyCode::KEY_9 }__',
     V: '__{ KeyCode::RETURN }__',
     B: nil,
     N: nil,
     M: '__{ KeyCode::KEY_0 }__',
     COMMA: '__{ KeyCode::KEY_7 }__',
     DOT: nil
     # BACKQUOTE: 'BACKSLASH', Z: 'COMMA',
    }.with_indifferent_access
  end

  def custom_mapping
    {}
  end

  # Deprecated
  def event_mapping
    {'@' => 'DANNISH_DOLLAR', '&' => 'KEY_1', 'é' => 'KEY_2', '"' => 'KEY_3', "'" => 'KEY_4', '(' => 'KEY_5', '§' => 'KEY_6', 'è' => 'KEY_7', '!' => 'KEY_8', 'c' => 'KEY_9', 'à' => 'KEY_0', ')' => 'MINUS', '-' => 'EQUAL', 'backspace' => 'DELETE',
     'tab' => 'TAB', 'a' => 'Q', 'z' => 'W', 'e' => 'E', 'r' => 'R', 't' => 'T', 'y' => 'Y', 'u' => 'u', 'i' => 'I', 'o' => 'O', 'p' => 'P', '^' => 'BRACKET_LEFT', '$' => 'BRACKET_RIGHT', 'enter' => 'RETURN',
     'caps' => 'JIS_EISUU', 'q' => 'A', 's' => 'S', 'd' => 'D', 'f' => 'F', 'g' => 'G', 'h' => 'H', 'j' => 'J', 'k' => 'K', 'l' => 'L', 'm' => 'SEMICOLON', 'ù' => 'QUOTE', '`' => 'BACKSLASH',
     'shift_l' => 'SHIFT_L', '<' => 'BACKQUOTE', 'w' => 'Z', 'x' => 'X', 'c' => 'C', 'v' => 'V', 'b' => 'B', 'n' => 'N', ',' => 'M', ';' => 'COMMA', ':' => 'DOT', '=' => 'SLASH', 'shift_r' => 'SHIFT_R',
     'fn' => 'FN', 'crtl_l' => 'CONTROL_L', 'alt_l' => 'OPTION_L', 'cmd_l' => 'COMMAND_L', 'space' => 'SPACE', 'cmd_r' => 'COMMAND_R', 'OPTION_R' => 'a'
    }.with_indifferent_access
  end

  def define_helper_keys
    @shift = 'ModifierFlag::SHIFT_L'
    @opt = 'ModifierFlag::OPTION_L'
    @cmd = 'ModifierFlag::COMMAND_L'
    @shift_opt = 'ModifierFlag::SHIFT_L, ModifierFlag::OPTION_L'
    @left = 'KeyCode::CURSOR_LEFT'
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
