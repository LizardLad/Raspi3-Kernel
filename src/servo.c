
	/*gpio_setup(3, GPIO_OUTPUT);
	
	while(1)
	{
		int32_t k = 0;
		for(int32_t pos = 0; pos <= 180; pos++)
		{
			k = 13.333333333333 * pos;
			gpio_output(3, true);
			wait_usec(k); //Goto 0 degrees if 180 is desired use 2400us
			gpio_output(3, false);
			wait_usec(15000 - k);
			//wait_usec(15000);
		}
		int32_t i = 0;
		for(int32_t pos = 180; pos >= 0; pos --)
		{
			i = 13.3333333333333 * pos;
			gpio_output(3, true);
			wait_usec(i); //Goto 0 degrees if 180 is desired use 2400us
			gpio_output(3, false);
			wait_usec(15000 - i);
			//wait_usec(15000);
		}
	}
	*/
