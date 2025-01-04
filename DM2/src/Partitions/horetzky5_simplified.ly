\version "2.22.1"

\relative c'' {
  \tempo 4 = 3
  \time 2/4
  \key c \major
  \clef treble

  c4 c |
  g' g |
  a a |
  g g |
  f f |
  e e |
  d d |
  c2 |

  g'4 g |
  f f |
  e e |
  d2 |
  <g e>4 <g e> |
  <f d> <f d> |
  <e c> <e c> |
  <d b>2 |
  c4 c |
  <g' c,> g |
  <a c,> a |
  <g e c>2 |
  f4 f |
  <e c> <e c> |
  <d c> <d b> |
  c2 |

  c4. c8 |
  g'4. g8 |
  a4. a8 |
  g4. <g e>8 |
  r <f e d> r <f d> |
  r <e d> r <e c> |
  r <d c> r <d b> |
  c2 |

  g'4. g8 |
  fis4. f8 |
  e4. e8 |
  d4. g,8 |
  g'4. g16 fis |
  f4. f16 e |
  e4. fis16 g |
  d4 r8 f!16 d |
}

\relative c' {
  \time 2/4
  \key c \major

  c2 | e4 c | f c | e c | d b | c a | f g | c2 |

  e4 g, | d' g, | c g | b2 |
  c4 cis | d b | c f | g2 |
  <e c> | e4 c | f c | c2 |
  <b' g>4 <b g> | a e | f g | <e c>2 |

  r8 c e r | r e c r | r f c r | r e c r |
  d4 b | c a | f' g | e c |
  r8 e c r | r d b r | r c a4 | r8 b g4 |
  r8 e' c r | r d b r | r c g r | r b g r |
}
