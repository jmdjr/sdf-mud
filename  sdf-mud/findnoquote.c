/****************************************************************************
 * ResortMUD Version 5.0 was mainly programmed by Ntanel, Garinan, Josh,    *
 * Badastaz, Digifuzz, Senir, Kratas, Scion, Shogar and Tagith.             *
 * ------------------------------------------------------------------------ *
 * Copyright (C) 1996 - 2001 Haslage Net Electronics: MudWorld of Lorain,   *
 * Ohio.    ALL RIGHTS RESERVED    See /doc/RMLicense.txt for more details. *
 ****************************************************************************/

char buf[8000];
main()
{
	int dq=0;
	int sq=0;
	int lcnt=0;
	char *s;
	while (gets(buf))
	{

		s=buf;
		dq=0;	
		sq=0;
		lcnt++;
		while(*s)
		{
			switch(*s++)
			{
				case '\'':
					if(!(dq %2))
						sq++;
					break;
				case '"':
					dq++;
					break;
				default:
					break;
			}
		}
		if(sq % 2 || dq % 2)
		{
			printf("line: %d\n\t%s\n",lcnt,buf);
		}
	}
}
