#define MAXLEN 128

int prints(char *s) { while(*s) putc(*s++); }

int gets(char s[ ]) 
{
  char c, *t = s; int len=0;
  while( (c=getc()) != '\r' && len < MAXLEN-1) {
    *t++ = c; putc(c); len++;
  }
  *t = 0; return s;
}

int printf(char *fmt, ...) 
{
  char *cp = fmt;
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
      case 'l' : printl(*(u32 *)ip); break;
      case 'X' : printX(*(u32 U)ip); break;
    }
    cp++; ip++;
  }
}
