
double press_time = 428.571428;

enum{
    AUTO,
    KEYBOARD
};
unsigned long ret_time, start_time, pattern_start, get_timing, time_gap, offset;
long smart_person, idiot_person;
uint16_t line_pattern = 0;
int start_bit = 1;
int i=0;
int jitter;
int mode;
char key_mapping[] = { 'a','b','c','d','f','k','l' };

void setup()
{
    mode = KEYBOARD;
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
        digitalWrite(6, LOW);
    }
    randomSeed(analogRead(0));
    Serial.begin(9600);
    Mouse.begin();
    Keyboard.begin();
    pattern_start = pgm_read_word_near(timing + 0);
    jitter = 70;
    offset = 0;
}


void loop()
{ 
    ////////////////////////////////////////////////////
    if(mode == KEYBOARD)
    {
        int c = 0;
        int cnt[4] = {0};
        while(1)
        {
            while(start_bit)
            {
                if(digitalRead(5) == 0)
                {
                  start_bit = 0;
                  start_time = (unsigned long)millis();
                }
            }

            while(!digitalRead(4))
            {
                Keyboard.releaseAll();
                start_bit = 1;
                i = 0;
                c = 0;
            }
            
            ret_time = (unsigned long)millis() - start_time;// + pattern_start;
            
            if(digitalRead(5))
            {
                Keyboard.release('d');
                cnt[0] = 0;
            }
                
            if(digitalRead(4))
            {
                Keyboard.release('f');
                cnt[1] = 0;
            }
                
            if(digitalRead(3))
            {
                Keyboard.release('k');
                cnt[2] = 0;
            }
                
            if(digitalRead(2))
            {
                Keyboard.release('l');
                cnt[3] = 0;
            }
                
            
            if(!digitalRead(5))
            {
                if(cnt[0] == 0)
                {
                    Serial.print("1 ");
                    Serial.println(ret_time - press_time * c);
                    c++;
                    cnt[0]++;
                }
                Keyboard.press('d');
                if(digitalRead(5))
                    Keyboard.release('d');
            }
            if(!digitalRead(4))
            {
                if(cnt[1] == 0)
                {
                    Serial.print("2 ");
                    Serial.println(ret_time - press_time * c);
                    c++;
                    cnt[1]++;
                }
                Keyboard.press('f');
                if(digitalRead(4))
                    Keyboard.release('f');
            }
            if(!digitalRead(3))
            {
                if(cnt[2] == 0)
                {
                    Serial.print("3 ");
                    Serial.println(ret_time - press_time * c);
                    c++;
                    cnt[2]++;
                }
                Keyboard.press('k');
                if(digitalRead(3))
                    Keyboard.release('k');
            }
            if(!digitalRead(2))
            {
                if(cnt[3] == 0)
                {
                    Serial.print("4 ");
                    Serial.println(ret_time - press_time * c);
                    c++;
                    cnt[3]++;
                }
                Keyboard.press('l');
                if(digitalRead(2))
                    Keyboard.release('l');
            }
        }
    }
    ////////////////////////////////////////////////////
    
    smart_person = random(-jitter, jitter);
    if(i != 0)
        smart_person -= jitter;
    ret_time = (unsigned long)millis() - start_time + pattern_start - offset + smart_person;
        
    line_pattern = pgm_read_byte_near(pattern[i]);
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
        if(((line_pattern & 0x0003) >> 0) == 3)
        {
            Keyboard.release('s');
        }
        if(((line_pattern & 0x000c) >> 2) == 3)
        {
            Keyboard.release('d');
        }
        if(((line_pattern & 0x0030) >> 4) == 3)
        {
            Keyboard.release('f');
        }
        if(((line_pattern & 0x00c0) >> 6) == 3)
        {
            Keyboard.release(' ');
        }
        if(((line_pattern & 0x0300) >> 8) == 3)
        {
            Keyboard.release('j');
        }
        if(((line_pattern & 0x0c00) >> 10) == 3)
        {
            Keyboard.release('k');
        }
        if(((line_pattern & 0x3000) >> 12) == 3)
        {
            Keyboard.release('l');
        }
        
        if(((line_pattern & 0x0003) >> 0) == 1 || ((line_pattern & 0x0003) >> 0) == 2)
        {
            Keyboard.release('s');
            Keyboard.press('s');
        }
        if(((line_pattern & 0x000c) >> 2) == 1 || ((line_pattern & 0x000c) >> 2) == 2)
        {
            Keyboard.release('d');
            Keyboard.press('d');
        }
        if(((line_pattern & 0x0030) >> 4) == 1 || ((line_pattern & 0x0030) >> 4) == 2)
        {
            Keyboard.release('f');
            Keyboard.press('f');
        }
        if(((line_pattern & 0x00c0) >> 6) == 1 || ((line_pattern & 0x00c0) >> 6) == 2)
        {
            Keyboard.release(' ');
            Keyboard.press(' ');
        }
        if(((line_pattern & 0x0300) >> 8) == 1 || ((line_pattern & 0x0300) >> 8) == 2)
        {           
            Keyboard.release('j');
            Keyboard.press('j');
        }
        if(((line_pattern & 0x0c00) >> 10) == 1 || ((line_pattern & 0x0c00) >> 10) == 2)
        {
            Keyboard.release('k');
            Keyboard.press('k');
        }
        if(((line_pattern & 0x3000) >> 12) == 1 || ((line_pattern & 0x3000) >> 12) == 2)
        {           
            Keyboard.release('l');
            Keyboard.press('l');
        }
  
        delay(16);
  
        if(((line_pattern & 0x0003) >> 0) == 1)
        {
            Keyboard.release('s');
        }
        if(((line_pattern & 0x000c) >> 2) == 1)
        {
            Keyboard.release('d');
        }
        if(((line_pattern & 0x0030) >> 4) == 1)
        {
            Keyboard.release('f');
        }
        if(((line_pattern & 0x00c0) >> 6) == 1)
        {
            Keyboard.release(' ');
        }
        if(((line_pattern & 0x0300) >> 8) == 1)
        {
            Keyboard.release('j');
        }
        if(((line_pattern & 0x0c00) >> 10) == 1)
        {
            Keyboard.release('k');
        }
        if(((line_pattern & 0x3000) >> 12) == 1)
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