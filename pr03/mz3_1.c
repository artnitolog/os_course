#define STYPE int
#define UTYPE unsigned

int bitcount(STYPE value)
{
    int cnt = 0;
    UTYPE uvalue = (UTYPE) value;
    while (uvalue) {
        cnt += uvalue & 1;
        uvalue >>= 1;
    }
    return cnt;
}