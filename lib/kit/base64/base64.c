static const char MimeBase64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

int base64_encode(char* _coded, char* _plain, int _length)
{
    unsigned char input[3] = { 0, 0, 0 };
    unsigned char output[4] = { 0, 0, 0, 0 };
    int ii = 0;
    int jj = 0;
    int size = (4 * (_length / 3)) + (_length % 3 ? 4 : 0) + 1;
    for (char* plain_current = _plain; plain_current < _plain + _length; ++ii, ++plain_current) {
        int index = ii % 3;
        input[index] = *plain_current;
        if (index == 2 || plain_current == _plain + _length - 1) {
            output[0] = ((input[0] & 0xFC) >> 2);
            output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
            output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
            output[3] = (input[2] & 0x3F);
            _coded[jj++] = MimeBase64[output[0]];
            _coded[jj++] = MimeBase64[output[1]];
            _coded[jj++] = index == 0 ? '=' : MimeBase64[output[2]];
            _coded[jj++] = index <  2 ? '=' : MimeBase64[output[3]];
            input[0] = input[1] = input[2] = 0;
        }
    }
    _coded[jj] = '\0';

    return size;
}

static int DecodeMimeBase64[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

int base64_decode(unsigned char* _plain, char* _coded, int _length)
{
    int index_space = 0;
    int phase = 0;
    int decode_previous = 0;
    unsigned char character;
    for (const char* plain_current = _coded; *plain_current != '\0'; ++plain_current) {
        int decode = DecodeMimeBase64[(int)*plain_current];
        if (decode != -1) {
            switch (phase) {
            case 0:
                ++phase;
                break;
            case 1:
                character = ((decode_previous << 2) | ((decode & 0x30) >> 4));
                if (index_space < _length) {
                    _plain[index_space++] = character;
                }
                ++phase;
                break;
            case 2:
                character = (((decode_previous & 0xf) << 4) | ((decode & 0x3c) >> 2));
                if (index_space < _length) {
                    _plain[index_space++] = character;
                }
                ++phase;
                break;
            case 3:
                character = (((decode_previous & 0x03) << 6) | decode);
                if (index_space < _length) {
                    _plain[index_space++] = character;
                }
                phase = 0;
                break;
            }
            decode_previous = decode;
        }
    }

    return index_space;
}
