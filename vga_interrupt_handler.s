.equ T3IF, 8
.equ OC1IF, 2
.equ OC4IF, 10
.equ CHEN, 15
.equ SPIEN, 15

.equ VSYNC_START, 0
.equ VSYNC_END, 2

.equ VIDEO_START, 35
.equ VIDEO_END, 514

.text

; ***************
; * T3Interrupt *
; ***************
.global __T3Interrupt
__T3Interrupt:
    
	bclr IFS0, #T3IF ; Clear Timer3 Interrupt Flag

    ; Save w0 and w1
    push w0
    push w1

    ; IF TMR4 = VSYNC_START THEN VSYNC = 1
    mov #VSYNC_START, w0 ; w0 = VSYNC_START
    mov TMR4, w1; w1 = TMR4
    CPSNE w0, w1
    bset PORTA, #0 ; VSYNC = 1
    ; END IF

    ; IF TMR4 = VSYNC_START THEN vsync_flag = 0
    CPSNE w0, w1
    clr.b _vsync_flag
    ; END IF

    ; If TMR4 = VSYNC_END then VSYNC = 0
    mov #VSYNC_END, w0 ; w0 = VSYNC_END
    CPSNE w0, w1
    bclr PORTA, #0 ; VSYNC = 0
    ; END IF

    ; IF TMR4 < VIDEO_START THEN clear DMA buffer
    mov #VIDEO_START, w0 ; w0 = VIDEO_START
    cp TMR4 ;
    bra ltu, clear_dma_buffer ; Branch if unsigned less than

    ; ELSE IF TMR4 > VIDEO_END THEN clear DMA buffer
    mov #VIDEO_END, w0 ; w0 = VIDEO_END
    cp TMR4 ;
    bra gtu, clear_dma_buffer ; Branch if unsigned greater than

    ; ELSE IF TMR4 = VIDEO_START THEN reset frame_ptr and load dma buffer
    mov #VIDEO_START, w0 ; w0 = VIDEO_START
    cp TMR4 ;
    bra z, reset_frame_ptr ; Branch if zero

    ; ELSE IF (TMR4-VIDEO_START) is odd THEN load dma buffer
    mov TMR4, w0 ; w0 = TMR4
    sub #VIDEO_START, w0 ; w0 = w0 ? VIDEO_START (10 bits)
    btsc w0, #0 ; Bit Test, Skip if set
    bra load_dma_buffer

    ; ELSE inc frame ptr
    mov #40, w0
    add _frame_ptr ; _frame_ptr = _frame_ptr + 40
    bra load_dma_buffer
    ; END IF

reset_frame_ptr:
    ; frame_ptr = frame
    mov #edsoffset(_frame), w0 ; w0 = &frame
    mov w0, _frame_ptr ; frame_ptr = w0

load_dma_buffer:
    mov #edsoffset(_dma_scanline_buffer), w0 ; w0 = &dma_scanline_buffer
	mov _frame_ptr, w1 ; w1 = frame_ptr

    ; copy the active line to the dma_buffer
	push RCOUNT
	repeat #19
	mov [w1++],[w0++]
	pop RCOUNT

    ; Restore w0 and w1
	pop w1
	pop w0

    retfie

clear_dma_buffer:
	; TODO clear the buffer
    mov #edsoffset(_dma_scanline_buffer), w0 ; w0 = &dma_scanline_buffer

    ; clear the dma buffer
    push RCOUNT
	repeat #19
	clr [w0++]
	pop RCOUNT

    ; Restore w0 and w1
	pop w1
	pop w0

    retfie

; ****************
; * OC1Interrupt *
; ****************
.global __OC1Interrupt
__OC1Interrupt:
    bclr IFS0, #OC1IF ; Clear Output Compare Module 1 Interrupt Flag
    
    ; Start SPI
    bset DMA0CON, #CHEN ; DMA channel 0 enabled
    bset DMA1CON, #CHEN ; DMA channel 1 enabled
    bset SPI1STAT, #SPIEN ; SPI module enabled

    retfie

; ****************
; * OC4Interrupt *
; ****************
.global __OC4Interrupt
__OC4Interrupt:
    bclr IFS1, #OC4IF ; Clear Output Compare Module 1 Interrupt Flag

    ; Stop SPI
    bclr DMA0CON, #CHEN ; DMA channel 0 disabled
    bclr DMA1CON, #CHEN ; DMA channel 1 disabled
    bclr SPI1STAT, #SPIEN ; SPI module disabled

    retfie

.end
