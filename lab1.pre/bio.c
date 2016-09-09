#define MAXLEN 128
char *ctable = "0123456789ABCDEF";
u16 BASE = 10; //for decimal numbers

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
      case 'X' : printX(*(u32 *)ip); break;
    }
    cp++; ip++;
  }
}

int rpu(u16 x)
{
  char c;
  if (x) {
    c = ctable[x % BASE];
    rpu(x / BASE);
    putc(c);
  }
}

int printu(u16 u)
{
  (u==0)? putc('0') : rpu(u);
  putc(' ');
}

int printd(u16 d)
{
  if(d < 0)
  {
    putc('-');
    d = -d;
  }
  printu(d);
}

int printx(u16 x)
{
  BASE = 16;
  printu(x);
  BASE = 10;
}

int printX(u32 x)
{
  BASE = 16;
  printl(x);
  BASE = 10;
}

int rpl(u32 x)
{
  char c;
  if (x==0)
    return;
  c = ctable[x % BASE];
  rpl(x / BASE);
  putc(c);
}

int printl(u32 x)
{
  // if (ALIGN)    // global 0 => no alignment
  //     align(x);
  if (x==0){
    putc('0'); putc(' ');
    return;
  }
  rpl(x);
  putc(' ');
}
