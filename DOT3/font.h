#ifndef __font_h_
#define __font_h_
#ifdef __cplusplus
extern "C" {
#endif

unsigned int Font_Build(void);
void Font_Print(int x, int y, unsigned int base, unsigned int texture, char *string, ...);

#ifdef __cplusplus
}
#endif
#endif
