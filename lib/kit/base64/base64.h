#ifndef ___MM_KIT_BASE64_H___
#define ___MM_KIT_BASE64_H___

#ifdef __cplusplus
extern "C" {
#endif

    int base64_encode(char* _coded, char* _plain, int _length);
    int base64_decode(unsigned char* _plain, char* _coded, int _length);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_BASE64_H___
