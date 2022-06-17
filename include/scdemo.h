#pragma once 

#include "global.h"

typedef u32 Beatscript[3];
struct Scene_ {
    void (*initFunc)(void *);
    void *initParam;
    void (*unkFunc)(void *);
    void *unkParam;
    void (*loopFunc)(void *);
    void *loopParam;
    void (*endFunc)(void *);
    void *endParam;
    Beatscript *beatscript;
};

struct _Scene {
	void (*initFunc)(void *);
	struct Scene_ *initParam;
	u32 (*loopFunc)(void *);
	void *loopParam;
	void (*endFunc)(void *);
	void *endParam;
	u16 unk18;
};


const Beatscript demo_beatscript[];
const struct Scene_ demo_scene_;
const struct _Scene demo_scene;

struct DemoSceneInfo { // Size = 0x10
    u32 notsure;
    s16 circle;
    s16 circleX;
    s16 circleY;
};

extern struct DemoSceneInfo *D_030046a4_d;
#define gDemo D_030046a4_d