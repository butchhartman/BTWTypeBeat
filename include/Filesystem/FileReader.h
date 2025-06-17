#ifndef ENGINE_FILEREADER_H_
#define ENGINE_FILEREADER_H_

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char* readBMP(char* filepath, uint32_t* bmpWidth, uint32_t* bmpHeight);

#ifdef __cplusplus
};
#endif

#endif // ENGINE_FILEREADER_H_