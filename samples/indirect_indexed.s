.segment "code"
start:
    ldy #$01
    lda #$03
    sta $01
    lda #$07
    sta $02
    ldx #$0a
    stx $0704
    lda ($01),Y

.segment "vectors"
    .word 0
    .word start
    .word 0
