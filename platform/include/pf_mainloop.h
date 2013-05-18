#ifndef __PF_MAINLOOP___
#define __PF_MAINLOOP___

typedef void (*LOOP)(void);

#define mainLoop(x)  __main_loop_##x_(void);\
                     volatile const LOOP __loop_##x @ "mainloop" = __main_loop_##x_ ;\
                     void __main_loop_##x_(void)
                       
                       
                       


#endif
