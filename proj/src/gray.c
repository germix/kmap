

unsigned int GrayEncode(unsigned int g)
{
	switch(g)
	{
		case 0: return 0;
		case 1: return 1;
		case 2: return 3;
		case 3: return 2;

		case 4: return 4;
		case 5: return 5;
		case 6: return 7;
		case 7: return 6;

		case 8: return 12;
		case 9: return 13;
		case 10: return 15;
		case 11: return 14;

		case 12: return 8;
		case 13: return 9;
		case 14: return 11;
		case 15: return 10;
	}
}

