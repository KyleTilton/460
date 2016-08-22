
int prints(char *s) { while(*s) putc(*s++); }

int gets() {}

int printf(char *fmt, ...) 
{
  char * = fmt;
  u16 *ip = (u16 *)&fmt + 1;
  u32 *up;
  while (*cp) {
    if (*cp != '%') {
      putc(*cp);
      if(*cp=='\n')
        putc('\r');
      cp++; continue;
    }
    cp++;
    switch(*cp) {
      case 'c' :   putc(*ip); break;
      case 's' : prints(*ip); break;
      case 'u' : printu(*ip); break;
      case 'd' : printd(*ip); break;
      case 'x' : printx(*ip); break;
      case 'l' : printl(*ip); break;
      case 'X' : printX(*ip); break;
    }
    cp++; ip++;
  }
}
