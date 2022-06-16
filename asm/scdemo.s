.syntax unified
.code 16

.include "include/gba.inc"
.section .text


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
glabel demo_stub
bx lr

glabel demo_initgfx
push {lr}
bl func_0801cdc0  @ init Title Screen GFX
pop {r0}
bx r0

@ 0x0890d4f0 - black circle 
glabel demo_create
push {r4, lr}
sub sp, #0x14 
ldr r4, =0x030046a4 @ gScene
movs r0, #0
bl func_08007324  @ (un)freeze visuals
bl func_080073f0  @ reset GFX system
@ create black circle sprite
ldr r0, =0x03005380 @ gGFX
ldr r0, [r0]
ldr r1, =0x0890d4f0 @ black circle
movs r2, #0x90 @ Y
str r2, [sp, #0]
ldr r2, =0x4800 @ layer
str r2, [sp, #4]
movs r2, #0 @ continue flag
str r2, [sp, #8]
movs r2, #0 @ 
str r2, [sp, #0xc] @ speed
movs r2, #0x80
lsls r2, #8
str r2, [sp, 0x10] @ unknown
movs r2, #0 @ frame
movs r3, #0x90 @ X
bl func_0804d160 @ create a sprite
ldr r1, [r4]
strh r0, [r1, #4] 

bl demo_initgfx

add sp, #0x14
pop {r4}
pop {r0}
bx r0

glabel demo_update
push {lr}
ldr r0, =0x03004afc @ current input
ldrh r1, [r0]
movs r0, #4
ands r0, r1 @ SELECT_BUTTON
cmp r0, #0
beq du_ret
movs r0, #0
bl func_0800bd04 @ reset the game if select is pressed
du_ret:
pop {r0}
bx r0

glabel demo_stop
push {lr}
bl func_08008628
bl func_08004058
pop {r0}
bx r0
.ltorg
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

glabel demo_bs_init
push {lr}
ldr r0, =0x03005380
ldr r0, [r0]
ldr r1, =0x030046a4
ldr r1, [r1]
movs r2, #4
ldrsh r1, [r1, r2]
movs r2, #1
bl func_0804d770 @ set visibility
pop {r0}
bx r0


glabel demo_bs_update
push {r4, lr}
ldr r4, =0x030046a4 @ gScene
ldr r2, =0x03005380 

ldr r0, [r2]
ldr r2, [r4]
movs r1, #4
ldrsh r1, [r2, r1]
movs r2, #0x40
movs r3, #0x40
bl func_0804d5d4 @ set X and Y

pop {r4}
pop {r0}
bx r0
.ltorg

.section .rodata
demo_scene_:
.word demo_create+1 @ init
.word 0
.word demo_stub+1   @ unknown
.word 0
.word demo_update+1 @ update
.word 0
.word demo_stop+1   @ stop
.word 0
.word demo_beatscript @ BeatScript

glabel demo_scene
.word 0x0801d86d
.word demo_scene_
.word 0x0801d8d9
.word 0
.word 0
.word 0
.short 0x10 @ scene struct size
.short 0


glabel demo_beatscript
@ initialize
.word 3
.word 0x0800bdf9 
.word 0x78 @ set tempo
.word 3
.word 0x0800bd05
.word 1
.word 3
.word 0x08007325
.word 1
.word 3
.word demo_bs_init+1
.word 0 @ init the sprite

.word 0x28
.word 0xff
.word 0x08a9b078 @ Lesson1

.word 3
.word demo_bs_update+1
.word 0

.word 0x3
.word 0x0800bd05
.word 1

.word 0x136
.word 0
.word 0x30
.word 0x14a
.word 0xc
.word 0

.word 0
.word 0
.word 0xc

.word 1
.word 0
.word 0 @ end
