#include "scdemo.h"
#include "main.h"
#include "code_08001360.h"
#include "code_0801d860.h"

asm(".include \"include/gba.inc\"");//Temporary

void demo_stub() {

}

void demo_init_gfx() {
    func_0801cdc0(); // Title Screen GFX (it has a circle)
}

void demo_create() {
    func_08007324(0); // (Un)Freeze visuals
    func_080073f0();  // Reset the GFX System
    D_030046a4_d->circle = func_0804d160(D_03005380, (u32 *)0x0890d4f0, 0, 0x90, 0x90, 0x4800, 0, 0, 0x8000);
    demo_init_gfx();
}

void demo_update() {
    if ((D_03004afc & 4) != 0) { // Reset on pressing SELECT
        func_0800bd04(0);
    }
}

void demo_stop() {
    func_08008628();
    func_08004058();
}

////////////////////////////////////////////////////////////

void demo_bs_init() {
    func_0804d770(D_03005380, D_030046a4_d->circle, 1); // Make the circle visible
}

void demo_bs_update() {
    func_0804d5d4(D_03005380, D_030046a4_d->circle, 0x40, 0x40); // Set X and Y
}

////////////////////////////////////////////////////////////////

const struct Scene_ demo_scene_ = {
    demo_create,
    NULL,
    demo_stub,
    NULL,
    demo_update,
    NULL,
    demo_stop,
    NULL,
    demo_beatscript,
};

const struct _Scene demo_scene = {
    func_0801d86c,
    &demo_scene_,
    func_0801d8d8,
    NULL,
    NULL,
    NULL,
    sizeof(struct DemoSceneInfo),
};

const Beatscript demo_beatscript[] = {
    {3, 0x0800bdf9, 0x78},      // Tempo
    {3, 0x0800bd05, 1},         // Set some transition flag
    {3, 0x08007325, 1},         // Visuals
    {3, demo_bs_init, 0},       // Init
    {0x28, 0xff, 0x08a9b078},   // Play Lesson1
    {3, demo_bs_update, 0},     // Update
    {3, 0x0800bd05, 1},         // More update
    {0x136, 0, 0x30},           // Fade out music
    {0x14a, 0xc, 0},            // Fade out screen
    {0, 0, 0xc},                // Rest
    {1, 0, 0},                  // Exit
};