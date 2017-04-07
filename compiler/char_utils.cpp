#include "common.hpp"

static uint32_t props[256] = {
    0x00000000, //  NUL
    0x00000000, //  SOH
    0x00000000, //  STX
    0x00000000, //  ETX
    0x00000000, //  EOT
    0x00000000, //  ENQ
    0x00000000, //  ACK
    0x00000000, //  BEL
    0x00000000, //  BS
    0x00000000, //  TAB
    0x00000000, //  LF
    0x00000000, //  VT
    0x00000000, //  FF
    0x00000000, //  CR
    0x00000000, //  SO
    0x00000000, //  SI
    0x00000000, //  DLE
    0x00000000, //  DC1
    0x00000000, //  DC2
    0x00000000, //  DC3
    0x00000000, //  DC4
    0x00000000, //  NAK
    0x00000000, //  SYN
    0x00000000, //  ETB
    0x00000000, //  CAN
    0x00000000, //  EM
    0x00000000, //  SUB
    0x00000000, //  ESC
    0x00000000, //  FS
    0x00000000, //  GS
    0x00000000, //  RS
    0x00000000, //  US
    0x00000000, //  Space
    0x00000000, //  !
    0x00000000, //  "
    0x00000000, //  #
    0x00000000, //  $
    0x00000000, //  %
    0x00000000, //  &
    0x00000000, //  '
    0x00000000, //  (
    0x00000000, //  )
    0x00000000, //  *
    0x00000000, //  +
    0x00000000, //  ,
    0x00000000, //  -
    0x00000000, //  .
    0x00000000, //  /
    0x0000001F, //  0
    0x0000001F, //  1
    0x0000001E, //  2
    0x0000001E, //  3
    0x0000001E, //  4
    0x0000001E, //  5
    0x0000001E, //  6
    0x0000001E, //  7
    0x0000001C, //  8
    0x0000001C, //  9
    0x00000000, //  :
    0x00000000, //  ;
    0x00000000, //  <
    0x00000000, //  =
    0x00000000, //  >
    0x00000000, //  ?
    0x00000000, //  @
    0x00000038, //  A
    0x00000038, //  B
    0x00000038, //  C
    0x00000038, //  D
    0x00000038, //  E
    0x00000038, //  F
    0x00000030, //  G
    0x00000030, //  H
    0x00000030, //  I
    0x00000030, //  J
    0x00000030, //  K
    0x00000030, //  L
    0x00000030, //  M
    0x00000030, //  N
    0x00000030, //  O
    0x00000030, //  P
    0x00000030, //  Q
    0x00000030, //  R
    0x00000030, //  S
    0x00000030, //  T
    0x00000030, //  U
    0x00000030, //  V
    0x00000030, //  W
    0x00000030, //  X
    0x00000030, //  Y
    0x00000030, //  Z
    0x00000000, //  [
    0x00000000, //  BACKSLASH
    0x00000000, //  ]
    0x00000000, //  ^
    0x0000003F, //  _
    0x00000000, //  `
    0x00000038, //  a
    0x00000038, //  b
    0x00000038, //  c
    0x00000038, //  d
    0x00000038, //  e
    0x00000038, //  f
    0x00000030, //  g
    0x00000030, //  h
    0x00000030, //  i
    0x00000030, //  j
    0x00000030, //  k
    0x00000030, //  l
    0x00000030, //  m
    0x00000030, //  n
    0x00000030, //  o
    0x00000030, //  p
    0x00000030, //  q
    0x00000030, //  r
    0x00000030, //  s
    0x00000030, //  t
    0x00000030, //  u
    0x00000030, //  v
    0x00000030, //  w
    0x00000030, //  x
    0x00000030, //  y
    0x00000030, //  z
    0x00000000, //  {
    0x00000000, //  |
    0x00000000, //  }
    0x00000000, //  ~
    0x00000000, //  DEL
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000010,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
    0x00000030,
};

bool verm::is_digit_bin(char c)
{
    return 0 != (props[(uint8_t)c] & 1);
}

bool verm::is_digit_oct(char c)
{
    return 0 != (props[(uint8_t)c] & 2);
}

bool verm::is_digit_dec(char c)
{
    return 0 != (props[(uint8_t)c] & 4);
}

bool verm::is_digit_hex(char c)
{
    return 0 != (props[(uint8_t)c] & 8);
}

bool verm::can_start_identifier(char c)
{
    return 0 != (props[(uint8_t)c] & 0x20);
}

bool verm::can_be_in_identifier(char c)
{
    return 0 != (props[(uint8_t)c] & 0x10);
}
