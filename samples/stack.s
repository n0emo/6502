.segment "code"
start:
  ldx #$00
  ldy #$00

firstloop:
  txa
  sta $0200,y
  pha
  inx
  iny
  cpy #$10
  bne firstloop ;loop until y is $10

secondloop:
  pla
  sta $0200,y
  iny
  cpy #$20      ;loop until y is $20
  bne secondloop

.segment "vectors"
    .word 0
    .word start
    .word 0
