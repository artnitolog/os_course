const char template[] = "rwxrwxrwx";

int parse_rwx_permissions(const char *str)
{
    if (!str ) {
        return -1;
    }
    int i = 0;
    unsigned ans = 0;
    while (template[i] && str[i]) {
        ans <<= 1;
        if (str[i] == template[i]) {
            ans++;
        } else if (str[i] != '-') {
            return -1;
        }
        i++;
    }
    if (template[i] || str[i]) {
        return -1;
    }
    return ans;
}
