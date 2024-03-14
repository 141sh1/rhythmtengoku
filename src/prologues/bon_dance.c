#include "prologues/bon_dance.h"

#include "src/code_08001360.h"
#include "src/code_08007468.h"
#include "src/code_0800b778.h"
#include "src/lib_0804ca80.h"

// For readability.
#define gPrologueEngineData ((struct BonDancePrologueEngineData *)gCurrentEngineData)


  //  //  //  PROLOGUE: THE BON DANCE  //  //  //


// [func_08045304] GFX_INIT Func_02
void func_08045304(void) {
    func_0800c604(0);
    gameplay_start_screen_fade_in();
}


// [func_08045314] GFX_INIT Func_01
void func_08045314(void) {
    u32 task;

    func_0800c604(0);
    task = func_08002ee0(get_current_mem_id(), bon_dance_prologue_gfx_table, 0x2000);
    run_func_after_task(task, func_08045304, 0);
}


// [func_08045344] GFX_INIT Func_00
void func_08045344(void) {
    u32 task;

    func_0800c604(0);
    func_08006d80();
    task = start_new_texture_loader(get_current_mem_id(), bon_dance_prologue_buffered_textures);
    run_func_after_task(task, func_08045314, 0);
}


// [func_08045374] MAIN - Init
void bon_dance_prologue_engine_start(u32 ver) {
    gPrologueEngineData->ver = ver;

    func_08045344();
    scene_show_obj_layer();
    scene_set_bg_layer_display(BG_LAYER_1, TRUE, 0, 0, 0, 29, 1);

    gPrologueEngineData->textSprite = sprite_create(gSpriteHandler, anim_bon_dance_prologue_title, 0, 80, 64, 0, 0, 0x7f, 0);
}


// [func_080453d0] ENGINE Func_00
void func_080453d0(void) {
}


// [func_080453d4] MAIN - Update
void bon_dance_prologue_engine_update(void) {
}


// [func_080453d8] MAIN - Close
void bon_dance_prologue_engine_stop(void) {
}


// [func_080453dc] Event 0 - Play Animation (Text)
void func_080453dc(void) {
    sprite_set_playback(gSpriteHandler, gPrologueEngineData->textSprite, 1, 0x7f, 0);
}
