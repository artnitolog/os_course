void normalize_path(char *buf)
{
    char *fast = buf, *slow = buf;
    while (*fast) {
        if (*fast == '/') {
            if (*(fast + 1) != '/') {
                *slow++ = '/';
            }
        } else {
            *slow++ = *fast;
        }
        fast++;
    }
    *slow = '\0';
}