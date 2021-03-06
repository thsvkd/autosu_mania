

enum {
	AUTO,
	KEYBOARD
};
unsigned long ret_time, start_time, pattern_start, get_timing, time_gap, offset;
long smart_person, idiot_person;
uint16_t line_pattern = 0;
int start_bit = 1;
int i = 0;
int jitter;
int mode;
char key_mapping[] = "";

void setup()
{
	pinMode(2, INPUT_PULLUP);
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	/*if (strlen(key_mapping) > 4)
		pinMode(7, INPUT_PULLUP);
	if (strlen(key_mapping) > 5)
		pinMode(8, INPUT_PULLUP);
	if (strlen(key_mapping) > 6)
		pinMode(9, INPUT_PULLUP);*/
	pinMode(6, OUTPUT);
	if (mode == AUTO)
	{
		digitalWrite(6, HIGH);
		attachInterrupt(digitalPinToInterrupt(2), Rbutton_press, FALLING);
		attachInterrupt(digitalPinToInterrupt(3), Lbutton_press, FALLING);
	}
	else if (mode == KEYBOARD)
	{
	}
	randomSeed(analogRead(0));
	Serial.begin(9600);
	Mouse.begin();
	Keyboard.begin();
	pattern_start = pgm_read_word_near(timing + 0);

	jitter = 0;
	mode = AUTO;
	offset = 0;
}


void loop()
{
	/*if (mode == KEYBOARD)
	{
		while (1)
		{
			if (digitalRead(2))
				Keyboard.release(key_mapping[3]); 
			if (digitalRead(3))
				Keyboard.release(key_mapping[2]);
			if (digitalRead(4))
				Keyboard.release(key_mapping[1]);
			if (digitalRead(5))
				Keyboard.release(key_mapping[0]);


			if (!digitalRead(2))
			{
				Keyboard.press(key_mapping[3]);
				delay(16);
				if (digitalRead(2))
					Keyboard.release(key_mapping[3]);
			}
			if (!digitalRead(3))
			{
				Keyboard.press(key_mapping[2]);
				delay(16);
				if (digitalRead(3))
					Keyboard.release(key_mapping[2]);
			}
			if (!digitalRead(4))
			{
				Keyboard.press(key_mapping[1]);
				delay(16);
				if (digitalRead(4))
					Keyboard.release(key_mapping[1]);
			}
			if (!digitalRead(5))
			{
				Keyboard.press(key_mapping[0]);
				delay(16);
			}
		}
	}*/

	if (jitter != 0)
	{
		smart_person = random(-jitter, jitter);			//Give a random delay to make it look like a person is playing.
		if (i != 0)
			smart_person -= jitter;

		ret_time = (unsigned long)millis() - start_time + pattern_start - offset + smart_person;
	}
	else
		ret_time = (unsigned long)millis() - start_time + pattern_start - offset;

	line_pattern = pgm_read_word_near(pattern[i]);
	get_timing = pgm_read_dword_near(timing + i);

	while (start_bit)
	{
		if (digitalRead(5) == 0)
		{
			start_bit = 0;
			start_time = (unsigned long)millis();
		}
	}

	time_gap = abs(get_timing - ret_time);

	if (time_gap <= 2)
	{
		i++;
		if (((line_pattern & 0x0003) >> 0) == 3)
			Keyboard.release(key_mapping[0]);

		if (((line_pattern & 0x000c) >> 2) == 3)
			Keyboard.release(key_mapping[1]);

		if (((line_pattern & 0x0030) >> 4) == 3)
			Keyboard.release(key_mapping[2]);

		if (((line_pattern & 0x00c0) >> 6) == 3)
			Keyboard.release(key_mapping[3]);

		if (((line_pattern & 0x0300) >> 8) == 3)
			Keyboard.release(key_mapping[4]);

		if (((line_pattern & 0x0c00) >> 10) == 3)
			Keyboard.release(key_mapping[5]);

		if (((line_pattern & 0x3000) >> 12) == 3)
			Keyboard.release(key_mapping[6]);

		if (((line_pattern & 0x0003) >> 0) == 1 || ((line_pattern & 0x0003) >> 0) == 2)
		{
			Keyboard.release(key_mapping[0]);
			Keyboard.press(key_mapping[0]);
		}
		if (((line_pattern & 0x000c) >> 2) == 1 || ((line_pattern & 0x000c) >> 2) == 2)
		{
			Keyboard.release(key_mapping[1]);
			Keyboard.press(key_mapping[1]);
		}
		if (((line_pattern & 0x0030) >> 4) == 1 || ((line_pattern & 0x0030) >> 4) == 2)
		{
			Keyboard.release(key_mapping[2]);
			Keyboard.press(key_mapping[2]);
		}
		if (((line_pattern & 0x00c0) >> 6) == 1 || ((line_pattern & 0x00c0) >> 6) == 2)
		{
			Keyboard.release(key_mapping[3]);
			Keyboard.press(key_mapping[3]);
		}
		if (((line_pattern & 0x0300) >> 8) == 1 || ((line_pattern & 0x0300) >> 8) == 2)
		{
			Keyboard.release(key_mapping[4]);
			Keyboard.press(key_mapping[4]);
		}
		if (((line_pattern & 0x0c00) >> 10) == 1 || ((line_pattern & 0x0c00) >> 10) == 2)
		{
			Keyboard.release(key_mapping[5]);
			Keyboard.press(key_mapping[5]);
		}
		if (((line_pattern & 0x3000) >> 12) == 1 || ((line_pattern & 0x3000) >> 12) == 2)
		{
			Keyboard.release(key_mapping[6]);
			Keyboard.press(key_mapping[6]);
		}

		delay(16);

		if (((line_pattern & 0x0003) >> 0) == 1)
			Keyboard.release(key_mapping[0]);

		if (((line_pattern & 0x000c) >> 2) == 1)
			Keyboard.release(key_mapping[1]);

		if (((line_pattern & 0x0030) >> 4) == 1)
			Keyboard.release(key_mapping[2]);

		if (((line_pattern & 0x00c0) >> 6) == 1)
			Keyboard.release(key_mapping[3]);

		if (((line_pattern & 0x0300) >> 8) == 1)
			Keyboard.release(key_mapping[4]);

		if (((line_pattern & 0x0c00) >> 10) == 1)
			Keyboard.release(key_mapping[5]);

		if (((line_pattern & 0x3000) >> 12) == 1)
			Keyboard.release(key_mapping[6]);
	}

	while (!digitalRead(4))
	{
		Keyboard.releaseAll();
		start_bit = 1;
		i = 0;
	}
	if (i == END)
	{
		while (digitalRead(4))
		{
			start_bit = 1;
			i = 0;
		}
	}
}

void Lbutton_press()
{
	offset--;
}

void Rbutton_press()
{
	offset++;
}