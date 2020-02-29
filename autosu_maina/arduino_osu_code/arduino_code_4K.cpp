

enum{
    AUTO,
    KEYBOARD
};
unsigned long ret_time, start_time, pattern_start, get_timing, time_gap, offset;
long smart_person, idiot_person;
uint16_t line_pattern[4] = {0};
int start_bit = 1;
int i=0;
int jitter;
int mode;

void setup()
{
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, OUTPUT);
    if(mode == AUTO)
    {
        digitalWrite(6, HIGH);
        attachInterrupt(digitalPinToInterrupt(2), Rbutton_press, FALLING);
        attachInterrupt(digitalPinToInterrupt(3), Lbutton_press, FALLING);
    }
    else if(mode == KEYBOARD)
    {
    }
    randomSeed(analogRead(0));
    Serial.begin(9600);
    Mouse.begin();
    Keyboard.begin();
    pattern_start = pgm_read_word_near(timing + 0);
    jitter = 70;
    mode = AUTO;
    offset = 0;
}


void loop()
{ 
    if(mode == KEYBOARD)
    {
        while(1)
        {
            if(digitalRead(5))
                Keyboard.release('d');
            if(digitalRead(4))
                Keyboard.release('f');
            if(digitalRead(3))
                Keyboard.release('k');
            if(digitalRead(2))
                Keyboard.release('l');
            
            if(!digitalRead(5))
            {
                Keyboard.press('d');
                delay(16);
            }
            if(!digitalRead(4))
            {
                Keyboard.press('f');
                delay(16);
                if(digitalRead(4))
                    Keyboard.release('f');
            }
            if(!digitalRead(3))
            {
                Keyboard.press('k');
                delay(16);
                if(digitalRead(3))
                    Keyboard.release('k');
            }
            if(!digitalRead(2))
            {
                Keyboard.press('l');
                delay(16);
                if(digitalRead(2))
                    Keyboard.release('l');
            }
        }
    }

    
    smart_person = random(-jitter, jitter);
    if(i != 0)
        smart_person -= jitter;
    ret_time = (unsigned long)millis() - start_time + pattern_start - offset + smart_person;
        
    line_pattern[0] = pgm_read_byte_near(pattern[i] + 0);
    line_pattern[1] = pgm_read_byte_near(pattern[i] + 1);
    line_pattern[2] = pgm_read_byte_near(pattern[i] + 2);
    line_pattern[3] = pgm_read_byte_near(pattern[i] + 3);
    get_timing = pgm_read_dword_near(timing + i);
    
    while(start_bit)
    {
        if(digitalRead(5) == 0)
        {
          start_bit = 0;
          start_time = (unsigned long)millis();
        }
        
    }

    time_gap = abs(get_timing - ret_time);

    if(time_gap < 2)
    {
        i++;
        if(line_pattern[0] == 3)
        {
            Keyboard.release('d');
        }
        if(line_pattern[1] == 3)
        {
            Keyboard.release('f');
        }
        if(line_pattern[2] == 3)
        {
            Keyboard.release('k');
        }
        if(line_pattern[3] == 3)
        {
            Keyboard.release('l');
        }
        
        if(line_pattern[0] == 1 || line_pattern[0] == 2)
        {
            Keyboard.release('d');
            Keyboard.press('d');
        }
        if(line_pattern[1] == 1 || line_pattern[1] == 2)
        {
            Keyboard.release('f');
            Keyboard.press('f');
        }
        if(line_pattern[2] == 1 || line_pattern[2] == 2)
        {
            Keyboard.release('k');
            Keyboard.press('k');
        }
        if(line_pattern[3] == 1 || line_pattern[3] == 2)
        {           
            Keyboard.release('l');
            Keyboard.press('l');
        }
  
        delay(16);
  
        if(line_pattern[0] == 1)
        {
            Keyboard.release('d');
        }
        if(line_pattern[1] == 1)
        {
            Keyboard.release('f');
        }
        if(line_pattern[2] == 1)
        {
            Keyboard.release('k');
        }
        if(line_pattern[3] == 1)
        {
            Keyboard.release('l');
        }
    }

    while(!digitalRead(4))
    {
        Keyboard.releaseAll();
        start_bit = 1;
        i = 0;
    }
    if(i == END)
    {
        while(digitalRead(4))
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